#include <fstream>
#include <iostream>
#include <map>
#include <string>

std::pair<uint64_t, uint64_t> extract_masks(const std::string &s) {
  auto bit = 1ul;
  auto off_mask = ~(0l);
  auto on_mask = 0l;
  for (auto it = s.rbegin(); it != s.rend(); it++, bit <<= 1) {
    if (*it == '1') {
      on_mask |= bit;
    }
    if (*it == '0') {
      off_mask -= bit;
    }
  }
  return std::make_pair(off_mask, on_mask);
}

int main() {
  auto str = std::ifstream{"data14.txt"};
  auto memory = std::map<uint64_t, uint64_t>{};
  auto i = int{};
  std::string buffer;
  auto masks = std::make_pair(0ul, 0ul);
  auto &off_mask = masks.first;
  auto &on_mask = masks.second;

  while (std::getline(str, buffer)) {
    if (buffer.substr(0, 4) == "mask") {
      buffer = buffer.substr(7);
      masks = extract_masks(buffer);
    } else {
      auto address = std::stol(buffer.data() + buffer.find('[') + 1);
      auto value = std::stol(buffer.data() + buffer.find('=') + 2);
      value |= on_mask;
      value &= off_mask;
      memory[address] = value;
    }
  }
  auto sum = 0l;
  for (auto &kv : memory) {
    sum += kv.second;
  }

  std::cout << "Sum of memory = " << sum << std::endl;

  return 0;
}
