#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
  auto iterations = 30000000;
  auto last_loc = std::vector<int>(iterations, -1);
  auto str = std::ifstream{"data15.txt"};
  auto number = 0;
  auto last_number = 0;
  auto pos = 0;

  while (str >> number) {
    auto ch = ' ';
    str >> ch; // eat the comma character if it is there
    if (pos > 0) {
      last_loc[last_number] = pos - 1;
    }
    last_number = number;
    pos++;
  }

  while (pos < iterations) {
    auto last = last_loc[last_number];
    if (last == -1) {
      number = 0;
    } else {
      number = pos - 1 - last;
    }
    last_loc[last_number] = pos - 1;
    last_number = number;
    pos++;
  }

  std::cout << iterations << "th digit = " << last_number << std::endl;

  return 0;
}
