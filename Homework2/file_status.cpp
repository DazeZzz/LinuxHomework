#include "file_status.h"
#include <bits/types/FILE.h>
#include <fcntl.h>
#include <libgen.h>
#include <pwd.h>
#include <sys/types.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>

bool FileStatus::GetStatus(const char *path) {
  if (lstat(path, file_status_.get()) == 0) {
    file_status_to_print_->name_ = basename(const_cast<char *>(path));
    return true;
  }
  return false;
}

void FileStatus::StatusTranslator() {
  //翻译mode信息
  file_status_to_print_->mode_ = FILE_TYPE.at(file_status_->st_mode & static_cast<unsigned>(__S_IFMT));
  mode_t mode = 0;
  for (int i = 8; i >= 0; --i) {
    mode = file_status_->st_mode >> static_cast<unsigned>(i);
    if ((mode & 1U) != 0U) {
      file_status_to_print_->mode_ += FILE_PERMISSION.at(static_cast<Permission>(i % 3));
    } else {
      file_status_to_print_->mode_ += FILE_PERMISSION.at(Permission::NO_PERMISSION);
    }
  }

  //翻译文件的link number
  file_status_to_print_->link_num_ = std::to_string(file_status_->st_nlink);

  //所有者名
  struct passwd *usr_pwd = getpwuid(file_status_->st_uid);
  file_status_to_print_->owner_ = usr_pwd->pw_name;

  //所属组名
  struct passwd *grp_pwd = getpwuid(file_status_->st_gid);
  file_status_to_print_->group_ = grp_pwd->pw_name;

  //文件大小
  auto size = static_cast<double>(file_status_->st_size);
  double unit_of_size = std::log2(size);
  unit_of_size /= 10;
  if (unit_of_size > 4) {
    file_status_to_print_->size_ = "HUGE";
  } else {
    while (size >= 1024) {
      size /= 1024;
    }
    file_status_to_print_->size_ = std::to_string(size).substr(0, 4);
    while ((file_status_to_print_->size_.at(file_status_to_print_->size_.size() - 1) == '0' ||
            file_status_to_print_->size_.at(file_status_to_print_->size_.size() - 1) == '.') &&
           file_status_to_print_->size_.size() > 1) {
      file_status_to_print_->size_.resize(file_status_to_print_->size_.size() - 1);
    }
    if (unit_of_size > 1) {
      file_status_to_print_->size_ += UNIT_OF_SIZE.at(static_cast<int>(unit_of_size));
    }
  }

  //文件修改时间
  struct tm *time = localtime(&file_status_->st_mtime);
  file_status_to_print_->t_mon_ = MONTH.at(time->tm_mon);
  file_status_to_print_->t_day_ = std::to_string(time->tm_mday);
  file_status_to_print_->t_hour_ = std::to_string(time->tm_hour);
  file_status_to_print_->t_min_ = std::to_string(time->tm_min);
}

void FileStatus::PrintStatus() const {
  std::cout << file_status_to_print_->mode_;
  std::cout << " ";
  std::cout << std::setw(2) << std::setfill(' ') << file_status_to_print_->link_num_;
  std::cout << " ";
  std::cout << file_status_to_print_->owner_;
  std::cout << " ";
  std::cout << file_status_to_print_->group_;
  std::cout << " ";
  std::cout << std::setw(5) << std::setfill(' ') << file_status_to_print_->size_;
  std::cout << " ";
  std::cout << std::setw(4) << std::setfill(' ') << file_status_to_print_->t_mon_;
  std::cout << " ";
  std::cout << std::setw(2) << file_status_to_print_->t_day_;
  std::cout << " ";
  std::cout << std::setw(2) << std::setfill('0') << file_status_to_print_->t_hour_;
  std::cout << ":";
  std::cout << std::setw(2) << std::setfill('0') << file_status_to_print_->t_min_;
  std::cout << " ";
  //如果文件类型是dir的话，蓝色加粗输出
  if (*file_status_to_print_->mode_.begin() == 'd') {
    std::cout << "\033[1m\033[34m" << file_status_to_print_->name_;
    std::cout << "\033[0m";
  } else {
    std::cout << file_status_to_print_->name_;
  }
  //如果文件类型是dir的话，文件名结尾加一个'/'字符
  if (*file_status_to_print_->mode_.begin() == 'd') {
    std::cout << "/";
  }
  std::cout << "\n";
}