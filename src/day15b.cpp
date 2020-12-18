#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
  auto seq = std::vector<long>{};
  auto last_loc = std::unordered_map<long, long>{};
  auto str = std::ifstream{"data15.txt"};
  auto i = 0;
  auto pos = 0;

  while (str >> i) {
    seq.push_back(i);
    auto ch = ' ';
    str >> ch; // eat the comma character if it is there
    if (pos > 0) {
      last_loc[seq[pos - 1]] = pos - 1;
    }
    pos++;
  }

  while (pos < 30000000) {
    auto number = 0;
    auto it = last_loc.find(seq[pos - 1]);
    if (it == last_loc.end()) {
      number = 0;
    } else {
      number = pos - 1 - it->second;
    }
    seq.push_back(number);
    last_loc[seq[pos - 1]] = pos - 1;
    pos++;
  }

  std::cout << "30000000th digit = " << seq[30000000-1] << std::endl;

  return 0;
}
