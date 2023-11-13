#include <algorithm>
#include <memory>
#include <new>
#include <optional>

// 缓冲区大小
#define SIZE_OF_BUFF 4096

class File {
public:
  File() { data_ = new char[SIZE_OF_BUFF]; }

  ~File() {
    if (data_) {
      delete[] data_;
    }
  }

  /**
   * 此类对象不可拷贝或赋值，但可通过移动转交归属权
   */
  File(const File &) = delete;
  File &operator=(const File &) = delete;

  /**
   * 移动构造函数
   */
  File(File &&f) noexcept
      : data_(f.data_), valid_data_count_(f.valid_data_count_),
        offset_(f.offset_), fd_(f.fd_) {
    f.data_ = nullptr;
    f.valid_data_count_ = 0;
    f.fd_ = -1;
    f.offset_ = 0;
  }

  /**
   * 移动赋值运算符
   */
  File &operator=(File &&f) noexcept {
    if (this != &f) {
      CloseFile(); //关闭当前对象所关联的文件
      if (data_) {
        delete[] data_;    //释放当前的缓冲区
      }
      data_ = f.data_;
      valid_data_count_ = f.valid_data_count_;
      offset_ = f.offset_;
      fd_ = f.fd_;
      f.data_ = nullptr;
      f.valid_data_count_ = 0;
      f.fd_ = -1;
      f.offset_ = 0;
    }
    return *this;
  }

  /**
   * @brief 打开一个文件
   *
   * @param pathname 要打开的文件的路径
   * @param flags flags参数与open()函数中的flags参数相同
   * @return 文件打开成功返回true，否则返回false
   */
  bool OpenFile(const char *pathname, int flags);

  /**
   * @brief 读取文件中的数据
   *
   * 先读取内存缓冲区中剩余的数据，内存缓冲区中的数据读完之后，比较 内存缓冲区
   * 与 还需读取的数据量 的大小，
   * 若还需读取的数据量大于内存缓冲区的大小，则直接调用read()读取到指定位置，否则先调用read()读取到
   * 内存缓冲区中，再从缓冲区读取到指定位置
   *
   * @param buff 将读取到的数据存储到buff所指的内存中
   * @param count 要读取的字节数
   * @return
   * 返回成功读取的字节数，如在未打开文件之前调用此函数，则返回std::nullopt
   */
  std::optional<size_t> ReadFile(char *buff, size_t count);

  /**
   * @brief 向文件中写入数据
   *
   * 先写入内存缓冲区，缓冲区写满则调用File::FlushBuff()将缓冲区中的数据刷入内核中，
   * 比较 内存缓冲区 与 还需写入的数据量
   * 的大小，若还需写入的数据量大于内存缓冲区的大小，
   * 则直接调用write()写入内核中，否则先写入内存缓冲区中，缓冲区满后再写入内核
   *
   * @param buff 指向要写入文件中的数据
   * @param count 要写入文件的字节数
   * @return
   * 返回成功写入的字节数，如在未打开文件之前调用此函数，则返回std::nullopt
   */
  std::optional<size_t> WriteFile(const char *buff, size_t count);

  /**
   * @brief 修改文件偏移量
   *
   * @param offset 相对偏移量
   * @param whence lseek()函数的三个常量之一
   * @return 如在未打开文件之前调用此函数，则返回std::nullopt
   */
  std::optional<off_t> LseekFile(off_t offset, int whence);

  /**
   * @brief 将缓冲区的所有数据写回内核缓冲区
   *
   * 先将偏移量设置在缓冲区开始处，再将缓冲区中的所有数据写回
   */
  void FlushBuff();

  /**
   * @brief 关闭文件
   *
   * @return 关闭成功返回true，失败返回false
   */
  bool CloseFile();

private:
  //指向内存缓冲区
  char *data_;
  //读写指针
  size_t offset_ = 0;
  //指向当前缓冲区中合法数据的末尾
  size_t valid_data_count_ = 0;
  //此对象所关联的文件句柄
  int fd_ = -1;
};