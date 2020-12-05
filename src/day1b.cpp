#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

int main() {
  auto singles = std::unordered_set<int>{};
  auto pairs =
      std::unordered_map<int, std::pair<int, int>>{}; // sum -> pair mapping
  auto str = std::ifstream{"data1.txt"};
  auto i = int{};

  while (str >> i && str.good()) {
    auto it = pairs.find(2020 - i);
    if (it != pairs.end()) {
      std::cout << it->second.first * it->second.second * i << std::endl;
      return 0;
    } else {
      for (auto j : singles) {
        pairs[i + j] = std::make_pair(i, j);
      }
      singles.insert(i);
    }
  }
  return 1;
}
