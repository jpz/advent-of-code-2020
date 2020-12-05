#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

int main() {
  auto set = std::unordered_set<int>{};
  auto str = std::ifstream{"data1.txt"};
  auto i = int{};

  while (str >> i && str.good()) {
    auto it = set.find(2020 - i);
    if (it != set.end()) {
      std::cout << (*it) * i << std::endl;
      return 0;
    } else {
      set.insert(i);
    }
  }
  return 1;
}
