#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <optional>
#include <vector>

enum class FileMode { READ_ONLY, WRITE_ONLY };

class BufferedFile {
 private:
  int fd_{};
  std::vector<char> buffer_;
  size_t buffer_size_;
  size_t data_size_{};
  FileMode mode_{};  // READ_ONLY or WRITE_ONLY

 public:
  explicit BufferedFile(size_t buffer_size) : buffer_size_(buffer_size) { buffer_.resize(buffer_size_); }

  bool OpenFile(const char *pathname, FileMode mode) {
    int flags = 0;
    switch (mode) {
      case FileMode::READ_ONLY:
        flags = O_RDONLY;
        break;
      case FileMode::WRITE_ONLY:
        flags = O_WRONLY | O_CREAT;
        break;
    }
    fd_ = open(pathname, flags);
    if (fd_ == -1) {
      perror("open");
      return false;
    }
    mode_ = mode;
    return true;
  }

  bool CloseFile() {
    if (fd_ != -1 && mode_ == FileMode::WRITE_ONLY && data_size_ > 0) {
      write(fd_, buffer_.data(), data_size_);  // write remaining data in buffer to file
      close(fd_);
      fd_ = -1;
      data_size_ = 0;
    }
    return true;
  }

  std::optional<size_t> ReadFile(char *buff, size_t required_count) {
    if (fd_ == -1 || mode_ == FileMode::WRITE_ONLY) {
      return std::nullopt;
    }

    size_t user_read_count = 0;

    if (required_count > buffer_size_) {
      // Copy remaining data in buffer to buff
      if (data_size_ > 0) {
        memcpy(buff, buffer_.data(), data_size_);
        user_read_count += data_size_;
        data_size_ = 0;
      }

      // Directly read from file if required_count is larger than buffer size
      ssize_t file_read_count = read(fd_, buff + user_read_count, required_count - user_read_count);
      if (file_read_count <= 0) {
        if (file_read_count == -1) {
          perror("read");
        }
        return std::nullopt;
      }
      user_read_count += file_read_count;
      return user_read_count;
    }

    while (data_size_ < required_count) {  // if buffer does not have enough data, read from file
      ssize_t file_read_count = read(fd_, buffer_.data() + data_size_, buffer_size_ - data_size_);
      if (file_read_count <= 0) {
        if (file_read_count == -1) {
          perror("read");
        }
        break;
      }
      data_size_ += file_read_count;
    }

    user_read_count = std::min(required_count, data_size_);
    memcpy(buff, buffer_.data(), user_read_count);  // copy data from buffer to buff
    memmove(buffer_.data(), buffer_.data() + user_read_count,
            data_size_ - user_read_count);  // move remaining data to front
    data_size_ -= user_read_count;

    return user_read_count;
  }

  std::optional<size_t> WriteFile(const char *buff, size_t count) {
    if (fd_ == -1 || mode_ == FileMode::READ_ONLY) {
      return std::nullopt;
    }

    if (count > buffer_size_) {
      // Write remaining data in buffer to file
      if (data_size_ > 0) {
        ssize_t written_count = write(fd_, buffer_.data(), data_size_);
        if (written_count == -1) {
          perror("write");
          return std::nullopt;
        }
        data_size_ = 0;
      }

      // Directly write to file if count is larger than buffer size
      ssize_t written_count = write(fd_, buff, count);
      if (written_count == -1) {
        perror("write");
        return std::nullopt;
      }
      return written_count;
    }

    if (data_size_ + count > buffer_size_) {  // if buffer does not have enough space, write to file
      ssize_t written_count = write(fd_, buffer_.data(), data_size_);
      if (written_count == -1) {
        perror("write");
        return std::nullopt;
      }
      data_size_ = 0;
    }

    memcpy(buffer_.data() + data_size_, buff, count);  // copy data from buff to buffer
    data_size_ += count;

    return count;
  }

  off_t LseekFile(off_t offset, int whence) {
    if (fd_ == -1) {
      return -1;
    }

    if (mode_ == FileMode::WRITE_ONLY) {
      FlushBuff();
    } else if (whence == SEEK_CUR) {
      offset -= data_size_;  // adjust offset considering data in buffer
      data_size_ = 0;        // discard data in buffer
    }

    off_t new_offset = lseek(fd_, offset, whence);
    if (new_offset == -1) {
      perror("lseek");
      return -1;
    }

    return new_offset;
  }

  bool FlushBuff() {
    if (fd_ == -1) {
      return false;
    }

    if (mode_ == FileMode::WRITE_ONLY) {
      ssize_t written_count = write(fd_, buffer_.data(), data_size_);
      if (written_count == -1) {
        perror("write");
        return false;
      }
      data_size_ = 0;
    }

    return true;
  }
};
