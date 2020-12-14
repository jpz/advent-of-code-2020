#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

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

  auto ids = std::unordered_set<int>{};
  auto min_id = std::numeric_limits<int>::max();
  auto max_id = std::numeric_limits<int>::min();

  while (str >> s) {
    auto id = row(s) * 8 + column(s);
    ids.insert(id);
    min_id = std::min(min_id, id);
    max_id = std::max(max_id, id);
  }

  // assuming that there is one id missing between [min_id, max_id]
  for (auto i = min_id + 1; i < max_id; i++) {
    if (ids.find(i) == ids.end()) {
      std::cout << "Missing seat ID = " << i << "\n";
    }
  }
  return 0;
}
