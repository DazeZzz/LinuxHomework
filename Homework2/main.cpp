#include <dirent.h>
#include <sys/types.h>
#include <iostream>
#include <ostream>
#include <string>
#include "file_status.h"

int main(int argc, char *argv[]) {
  DIR *dir = nullptr;
  if (argv[1] != nullptr) {
    dir = opendir(argv[1]);
  } else {
    dir = opendir(".");
  }
  if (dir != nullptr) {
    std::string dir_name;
    std::string full_filename;
    FileStatus files;
    if (argv[1] != nullptr) {
      dir_name = argv[1];
      dir_name += "/";
    }
    dirent *dent = readdir(dir);
    while (dent != nullptr) {
      full_filename = dir_name + dent->d_name;
      files.GetStatus(full_filename.c_str());
      files.StatusTranslator();
      files.PrintStatus();
      dent = readdir(dir);
    }
  } else {
    std::cout << "The folder was not found!\n";
  }
}