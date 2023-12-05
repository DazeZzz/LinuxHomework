#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

//文件的四种权限
enum class Permission { EXECUTE, WRITE, READ, NO_PERMISSION };

//每种文件类型对应的字符
static const std::unordered_map<unsigned int, char> FILE_TYPE = {{__S_IFDIR, 'd'}, {__S_IFCHR, 'c'}, {__S_IFBLK, 'b'},
                                                                 {__S_IFREG, '-'}, {__S_IFIFO, 'p'}, {__S_IFLNK, 'l'},
                                                                 {__S_IFSOCK, 's'}};

//每种权限对应的字符
static const std::unordered_map<Permission, char> FILE_PERMISSION = {
    {Permission::READ, 'r'}, {Permission::WRITE, 'w'}, {Permission::EXECUTE, 'x'}, {Permission::NO_PERMISSION, '-'}};

//月份对应的字符
static const std::unordered_map<int, std::string_view> MONTH = {{0, "Jan"},  {1, "Feb"}, {2, "Mar"},  {3, "Apr"},
                                                                {4, "May"},  {5, "Jun"}, {6, "Jul"},  {7, "Aug"},
                                                                {8, "Sept"}, {9, "Oct"}, {10, "Nov"}, {11, "Dec"}};

//文件大小的单位对应的字符
static const std::unordered_map<int, char> UNIT_OF_SIZE = {{1, 'K'}, {2, 'M'}, {3, 'G'}, {4, 'T'}};

//用来打印文件信息的结构体，通过linux系统文件结构体struct stat翻译而来，其中每一项都是要打印的信息的字符串
struct StatP {
  std::string mode_;
  std::string link_num_;
  std::string owner_;
  std::string group_;
  std::string size_;
  std::string t_mon_;
  std::string t_day_;
  std::string t_hour_;
  std::string t_min_;
  std::string name_;
};

class FileStatus {
 public:
  /**
   * @brief 通过文件路径获取文件信息结构体struct stat
   *
   * @param path 文件绝对路径，或者相对于当前执行文件夹的相对路径
   * @return 获取成功返回true，否则返回false
   */
  bool GetStatus(const char *path);

  /**
   * @brief 将linux系统api提供的文件结构体struct stat翻译为便于打印信息的struct StatP
   */
  void StatusTranslator();

  /**
   * @brief 格式化打印文件信息
   */
  void PrintStatus() const;

 private:
  // linux系统api提供的文件结构体struct stat
  std::shared_ptr<struct stat> file_status_ = std::make_shared<struct stat>();
  //便于打印的结构体struct StatP
  std::shared_ptr<struct StatP> file_status_to_print_ = std::make_shared<struct StatP>();
};