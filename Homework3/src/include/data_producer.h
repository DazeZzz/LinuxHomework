#include <random>
#include <string>

struct ProduceInfo {
  int num_files_;
  int min_file_size_;
  int max_file_size_;
  std::string dir_path_;
};

class DataProducer {
  using ProduceInfo = struct ProduceInfo;

 public:
  explicit DataProducer(ProduceInfo produce_info);

  /**
   * @brief Produce files using info in the struct ProduceInfo
   * @return true if produce files successfully
   */
  auto Produce() -> bool;

  /**
   * @brief Delete all files in the directory specified in the struct ProduceInfo
   * @return true if delete successfully
   */
  auto Clear() const -> bool;

 private:
  std::default_random_engine generator_;
  ProduceInfo produce_info_;
};