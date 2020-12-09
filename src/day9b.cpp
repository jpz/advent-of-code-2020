#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>

// return a pair of the positions of first and last of the sequence that adds to
// target_sum
std::pair<int, int> find_contiguous_sequence(const std::vector<int> &sequence,
                                             int target_sum) {
  auto head = 0;
  auto tail = 0;
  auto sum = sequence[0];
  while (sum != target_sum) {
    if (sum < target_sum) {
      sum += sequence[++tail];
    } else {
      sum -= sequence[head++];
    }
  }
  return std::make_pair(head, tail);
}

int main() {
  auto sequence = std::vector<int>{};
  auto str = std::ifstream{"data9.txt"};
  auto const target = 375054920;
  auto num = 0;
  while (str >> num) {
    sequence.push_back(num);
  }

  auto [head, tail] = find_contiguous_sequence(sequence, target);
  auto min =
      *std::min_element(sequence.begin() + head, sequence.begin() + tail + 1);
  auto max =
      *std::max_element(sequence.begin() + head, sequence.begin() + tail + 1);

  std::cout << "Sum of min and max that of contiguous sequence = " << min + max
            << std::endl;

  return 0;
}
