#include <gtest/gtest.h>
#include <sys/types.h>
#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <string>
#include "data_producer.h"
#include "data_sorter.h"

class SortFileTest : public ::testing::Test {
 protected:
  static uint FileSize(const std::string &file_path) {
    std::ifstream file(file_path);
    uint file_size = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
    ++file_size;
    return file_size;
  }

  static bool IsSorted(const std::string &file_path) {
    std::ifstream file(file_path);
    int64_t prev = 0;
    int64_t curr = 0;
    file >> prev;
    while (file >> curr) {
      if (prev > curr) {
        return false;
      }
      prev = curr;
    }
    return true;
  }
};

TEST_F(SortFileTest, TestProducer) {
  ProduceInfo info = {1, 100, 200, "./sort_single_file_test_files"};
  DataProducer producer(info);
  producer.Produce();

  const auto entry = std::filesystem::directory_iterator(info.dir_path_);
  // SortSingleFile() returns true if the task is finished successfully
  EXPECT_TRUE(DataSorter::SortSingleFile(*entry));
  // Check if the file is sorted
  EXPECT_TRUE(IsSorted(entry->path()));

  // Clear the directory
  producer.Clear();
}

TEST_F(SortFileTest, TestSortSingleFile) {
  ProduceInfo info = {1, 100, 200, "./sort_single_file_test_files"};
  DataProducer producer(info);
  producer.Produce();

  const auto entry = std::filesystem::directory_iterator(info.dir_path_);
  // SortSingleFile() returns true if the task is finished successfully
  EXPECT_TRUE(DataSorter::SortSingleFile(*entry));
  // Check if the file is sorted
  EXPECT_TRUE(IsSorted(entry->path()));

  // Clear the directory
  producer.Clear();
}

TEST_F(SortFileTest, TestMergeTwoFiles) {
  ProduceInfo info = {1, 100, 200, "./sort_single_file_test_files"};
  DataProducer producer(info);
  producer.Produce();

  const auto entry = std::filesystem::directory_iterator(info.dir_path_);
  // SortSingleFile() returns true if the task is finished successfully
  EXPECT_TRUE(DataSorter::SortSingleFile(*entry));
  // Check if the file is sorted
  EXPECT_TRUE(IsSorted(entry->path()));

  // Clear the directory
  producer.Clear();
}

TEST_F(SortFileTest, TestDoSort) {
  ProduceInfo info = {5, 100, 200, "./produce_file_test_files"};
  DataProducer producer(info);
  producer.Produce();

  // Check if the correct number of files are produced
  auto num_files = std::distance(std::filesystem::directory_iterator(info.dir_path_),
                                 std::filesystem::end(std::filesystem::directory_iterator{}));
  EXPECT_EQ(num_files, info.num_files_);

  // Check if each file size is within the specified range and the content is random
  for (const auto &entry : std::filesystem::directory_iterator(info.dir_path_)) {
    auto file_size = FileSize(entry.path());
    EXPECT_GE(file_size, info.min_file_size_);
    EXPECT_LE(file_size, info.max_file_size_);
  }
  producer.Clear();
}
