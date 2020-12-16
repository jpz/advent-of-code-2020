#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
  auto joltages = std::set<int>{};
  auto stream = std::ifstream{"data10.txt"};
  auto i = int{};

  joltages.insert(0);

  // sort the numbers with a set
  while (stream >> i) {
    joltages.insert(i);
  }

  // number of ways to reach joltage
  auto ways = std::map<int, long>{};

  ways[*joltages.rbegin()] = 1;

  // simple dynamic programming solution, sum ways to final adapter, from
  // largest to smallest
  for (auto it = std::next(joltages.rbegin()); it != joltages.rend(); it++) {
    auto joltage = *it;
    auto w = 0l;
    for (int offset = 1; offset <= 3; offset++) {
      if (joltages.find(joltage + offset) != joltages.end()) {
        w += ways[joltage + offset];
      }
    }
    ways[joltage] = w;
  }

  std::cout << "ways to get to the final voltage = " << ways[0] << std::endl;

  return 0;
}
