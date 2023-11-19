#include "data_sorter.h"
#include <future>
#include <vector>

auto DataSorter::DoSort() -> bool {
  std::filesystem::directory_iterator dir_it(dir_path_);
  // Store the results of all sort tasks.
  std::vector<std::future<bool>> task_results;
  
  // Get all files in the directory.
  for (const auto &file : dir_it) {
    if (file.is_directory()) {
      std::cerr << "The path: " << file.path() << " is a directory\n";
      return false;
    }
    // Sort each file in a separate thread, and store the result of each task.
    std::future<bool> res =
        thread_pool_.SubmitTask([&file] { return SortSingleFile(file); });
    task_results.emplace_back(std::move(res));
  }

  // Store the paths of all files to be merged.
  std::vector<std::string> files_to_merge;
  do {
    dir_it = std::filesystem::directory_iterator(dir_path_);
    // Go through all files in the directory.
    for (const auto &file : dir_it) {
      // If the file has been merged, skip it.
      if (file.path().string().find("_merged") != std::string::npos) {
        continue;
      }
      // Rename the file to avoid being merged again.
      std::filesystem::rename(file.path(), file.path().string() + "_merged");
      files_to_merge.emplace_back(file.path());
    }
    // Begin to merge files.
    auto file_it = files_to_merge.begin();
    while (file_it != files_to_merge.end()) {
      auto next_file_it = file_it + 1;
      if (next_file_it == files_to_merge.end()) {
        break;
      }
      // Submit the merge tasks.
      std::future<bool> res = thread_pool_.SubmitTask([file_it, next_file_it] {
        return MergeTwoFiles(*file_it, *next_file_it);
      });
      // Store the result of each merge task.
      task_results.emplace_back(std::move(res));
      file_it = next_file_it + 1;
    }
  } while (files_to_merge.size() > 1); // If there is only one file left, stop.

  // Wait for all tasks to finish.
  for (auto &res : task_results) {
    // If any task fails, return false.
    if (!res.get()) {
      return false;
    }
  }
  return true;
}

auto DataSorter::SortSingleFile(const std::filesystem::directory_entry &file)
    -> bool {
  std::fstream file_stream(file.path());
  if (!file_stream) {
    std::cerr << "Couldn't open file: " << file.path() << '\n';
    return false;
  }
  // Store all data in the file to the buffer.
  std::vector<std::uint64_t> buffer;
  uint64_t data = 0;
  while (file_stream >> data) {
    buffer.emplace_back(data);
  }

  // Sort the buffer.
  std::sort(buffer.begin(), buffer.end());

  // Write the sorted data back to the file.
  for (const auto &data : buffer) {
    file_stream << data << '\n';
  }
  if (!file_stream) {
    std::cerr << "Couldn't write to file: " << file.path() << '\n';
    return false;
  }
  return true;
}

auto MergeTwoFiles(const std::string &file_path_1,
                   const std::string &file_path_2) -> bool {
  std::fstream file_stream_1(file_path_1);
  std::fstream file_stream_2(file_path_2);
  // If any file can't be opened, return false.
  if (!file_stream_1 || !file_stream_2) {
    std::cerr << "Couldn't open file: " << file_path_1 << " or " << file_path_2
              << '\n';
    return false;
  }

  // Merge the two files into a new file.
  std::fstream output_file_stream(file_path_1 + "_merged");
  if (!output_file_stream) {
    std::cerr << "Couldn't open file: " << file_path_1 + "_merged" << '\n';
    return false;
  }

  // Merge the two files.
  std::uint64_t data_1 = 0;
  std::uint64_t data_2 = 0;
  while (file_stream_1 >> data_1 && file_stream_2 >> data_2) {
    if (data_1 < data_2) {
      output_file_stream << data_1 << '\n';
      file_stream_2.seekg(-1, std::fstream::cur);
    } else {
      output_file_stream << data_2 << '\n';
      file_stream_1.seekg(-1, std::fstream::cur);
    }
  }

  // Write the remaining data to the new file.
  while (file_stream_1 >> data_1) {
    output_file_stream << data_1 << '\n';
  }
  while (file_stream_2 >> data_2) {
    output_file_stream << data_2 << '\n';
  }

  // Delete the old files and rename the new file.
  std::filesystem::remove(file_path_1);
  std::filesystem::remove(file_path_2);
  std::filesystem::rename(file_path_1 + "_merged", file_path_1);

  // If the new file has errors, return false.
  if (!output_file_stream) {
    std::cerr << "Couldn't write to file: " << file_path_1 << '\n';
    return false;
  }
  return true;
}
