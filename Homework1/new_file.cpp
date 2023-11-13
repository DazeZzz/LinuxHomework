#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <optional>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "new_file.h"

bool File::OpenFile(const char *pathname, int flags) {
  fd_ = open(pathname, flags);
  if (fd_ != -1) {
    return true;
  }
  return false;
}

std::optional<size_t> File::ReadFile(char *buff, size_t required_count) {
  if (fd_ == -1) {
    std::cout << "Haven't opened any files yet!" << std::endl;
    return std::nullopt;
  }
  size_t not_provided_count = required_count;
  if (offset_ + not_provided_count <= valid_data_count_) {
    std::memcpy(buff, data_ + offset_, not_provided_count);
    offset_ += not_provided_count;
    not_provided_count = 0;
  }
  for (; offset_ < valid_data_count_; ++offset_, --not_provided_count, ++buff) {
    *buff = *(data_ + offset_);
  }
  if (not_provided_count >= SIZE_OF_BUFF) {
    not_provided_count -= read(fd_, buff, not_provided_count);
  } else {
    valid_data_count_ = read(fd_, data_, SIZE_OF_BUFF);
    if (valid_data_count_ >= not_provided_count) {
      std::memcpy(buff, data_, not_provided_count);
      offset_ = not_provided_count;
      not_provided_count = 0;
    } else {
      std::memcpy(buff, data_, valid_data_count_);
      offset_ = valid_data_count_;
      not_provided_count -= valid_data_count_;
    }
  }
  return required_count - not_provided_count;
}

std::optional<size_t> File::WriteFile(const char *buff, size_t required_count) {
  if (fd_ == -1) {
    std::cout << "Haven't opened any files yet!" << std::endl;
    return std::nullopt;
  }
  size_t not_provided_count = required_count;
  while (offset_ < SIZE_OF_BUFF && not_provided_count > 0) {
    *(data_ + offset_) = *buff;
    ++offset_;
    ++buff;
    --not_provided_count;
  }
  if (valid_data_count_ < offset_) {
    valid_data_count_ = offset_;
  }
  if (not_provided_count > 0) {
    FlushBuff();
    if (not_provided_count >= SIZE_OF_BUFF) {
      not_provided_count -= write(fd_, buff, not_provided_count);
      offset_ = 0;
      valid_data_count_ = 0;
    } else {
      std::memcpy(data_, buff, not_provided_count);
      offset_ = not_provided_count;
      valid_data_count_ = not_provided_count;
      not_provided_count = 0;
    }
  }
  return required_count - not_provided_count;
}

std::optional<off_t> File::LseekFile(off_t offset, int whence) {
  if (fd_ == -1) {
    std::cout << "Haven't opened any files yet!" << std::endl;
    return std::nullopt;
  }
  offset_ = 0;
  valid_data_count_ = 0;
  return lseek(fd_, offset, whence);
}

void File::FlushBuff() {
  lseek(fd_, -valid_data_count_, SEEK_CUR);
  write(fd_, data_, valid_data_count_);
}

bool File::CloseFile() {
  FlushBuff();
  if (!close(fd_)) {
    fd_ = -1;
    offset_ = 0;
    valid_data_count_ = 0;
    return true;
  }
  return false;
}
