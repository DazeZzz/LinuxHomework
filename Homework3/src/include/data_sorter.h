#include "thread_pool.h"
#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class DataSorter {
public:
  explicit DataSorter(std::string dir_path) : dir_path_(std::move(dir_path)) {}

  /**
   * @brief Sort all files in the directory specified in the constructor
   *
   * @return true if sort successfully
   */
  auto DoSort() -> bool;

  /**
   * @brief Sort a single file
   *
   * @param file the file to be sorted
   * @return true if sort successfully
   */
  static auto SortSingleFile(const std::filesystem::directory_entry &file)
      -> bool;

  /**
   * @brief Merge two files
   *
   * @param file_path_1 the path of the first file
   * @param file_path_2 the path of the second file
   * @return true if merge successfully
   */
  static auto MergeTwoFiles(const std::string &file_path_1,
                            const std::string &file_path_2) -> bool;

private:
  ThreadPool thread_pool_;
  std::string dir_path_;
};