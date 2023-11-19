#include "data_producer.h"
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <filesystem>

DataProducer::DataProducer(ProduceInfo produce_info)
    : produce_info_(std::move(produce_info)) {}

auto DataProducer::Produce() -> bool {
  // Specify the maximum and minimum values of the generated data.
  // The data type is std::int64_t
  std::uniform_int_distribution<std::int64_t> data_distribution(INT64_MIN,
                                                                INT64_MAX);
  // Specify the maximum and minimum amount of data in a file.
  std::uniform_int_distribution<int> file_size_distribution(
      produce_info_.min_file_size_, produce_info_.max_file_size_);

  // true if every file been produced successfully.
  bool is_success = true;

  // Begin to produce files.
  for (int i = 0; i < produce_info_.num_files_; ++i) {
    std::ofstream file(produce_info_.dir_path_ + '/' + std::to_string(i));

    // If the file has been produced successfully, fill it with a random amount
    // of data.
    if (file) {
      int file_size = file_size_distribution(generator_);
      for (int j = 0; j < file_size; ++j) {
        file << data_distribution(generator_) << '\n';
      }
    } else {
      is_success = false;
      std::cerr << "couldn't produce file: " + std::to_string(i);
    }
  }

  return is_success;
}

auto DataProducer::Clear() const -> bool {
  return std::filesystem::remove_all(produce_info_.dir_path_) != 0U;
}