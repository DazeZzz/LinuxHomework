#include <fstream>
#include <iostream>

int main() {
  std::fstream file_stream("./zss.txt");
  if (!file_stream) {
    std::cerr << "Couldn't open file: "
              << "zss/zss/zss.txt" << '\n';
  }
  int num = 0;
  for (int i = 0; i < 10; ++i) {
    file_stream >> num;
  }
  file_stream.seekp(0, std::fstream::beg);

  for (int i = 0; i < 30; ++i) {
    file_stream << i + 100 << '\n';
  }
}