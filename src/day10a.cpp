#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
  auto joltages = std::set<int>{};
  auto stream = std::ifstream{"data10.txt"};
  auto i = int{};

  // sort the numbers with a set
  while (stream >> i) {
    joltages.insert(i);
  }
  // add the final device which is +3 from highest plug
  joltages.insert(*std::prev(joltages.end()) + 3);

  // compute the count of different voltage differences
  auto difference_count = std::map<int, int>{};
  auto last_joltage = 0;
  for (auto joltage : joltages) {
    difference_count[joltage - last_joltage]++;
    last_joltage = joltage;
  }
  std::cout << "count of 1 differences times count of 3 differences = "
            << difference_count[1] * difference_count[3] << std::endl;
  return 0;
}
