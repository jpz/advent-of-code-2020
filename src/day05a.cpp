#include <fstream>
#include <iostream>
#include <string>

int row(const std::string &ticket) {
  int accum = 0;
  for (auto i = 0; i < 7; i++) {
    accum <<= 1;
    if (ticket.at(i) == 'B') {
      accum |= 1;
    }
  }
  return accum;
}

int column(const std::string &ticket) {
  int accum = 0;
  for (auto i = 7; i < 10; i++) {
    accum <<= 1;
    if (ticket.at(i) == 'R') {
      accum |= 1;
    }
  }
  return accum;
}

int main() {
  auto str = std::ifstream{"data05.txt"};
  auto s = std::string{};

  auto max_id = 0;

  while (str >> s) {
    max_id = std::max(max_id, row(s) * 8 + column(s));
  }
  std::cout << "highest seating ID = " << max_id << std::endl;
  return 0;
}
