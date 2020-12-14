#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

template <typename T>
void erase_one(std::unordered_multiset<T> &set, const T &value) {
  set.erase(set.find(value));
}

int find_first_pos_without_sum_in_lookback(const std::vector<int> &sequence,
                                           int lookback) {
  auto last_n = std::unordered_multiset<int>{};
  auto i = 0;
  for (auto num : sequence) {
    if (i >= lookback + 1) { // start erasing at n+2'th number
      erase_one(last_n, sequence[i - (lookback + 1)]);
    }
    if (i >= lookback) { // start testing at n+1'th number
      auto found = false;
      for (int j = i - lookback; j < i; j++) {
        auto n1 = sequence[j];
        auto n2 = num - n1;
        if (n1 != n2 and last_n.find(n2) != last_n.end()) {
          found = true;
        }
      }
      if (!found) {
        return i;
      }
    }
    last_n.insert(num);
    i++;
  }
  return -1;
}

int main() {
  auto sequence = std::vector<int>{};
  auto str = std::ifstream{"data09.txt"};
  auto const lookback = 25;
  auto num = 0;
  while (str >> num) {
    sequence.push_back(num);
  }

  auto pos = find_first_pos_without_sum_in_lookback(sequence, lookback);
  auto result = sequence[pos];
  std::cout << "Number that fails test = " << result << std::endl;

  return 0;
}
