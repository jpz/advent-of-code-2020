#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::pair<uint64_t, uint64_t> extract_masks(const std::string &s) {
  auto floating_mask = 0ul;
  auto on_mask = 0l;
  auto bit = 1ul;
  for (auto it = s.rbegin(); it != s.rend(); it++, bit <<= 1) {
    if (*it == '1') {
      on_mask |= bit;
    }
    if (*it == 'X') {
      floating_mask |= bit;
    }
  }
  return std::make_pair(on_mask, floating_mask);
}

template <typename T> int location_of_nth_bit(T value, int n) {
  auto position = 0;
  while (value && n) {
    if (value & 1) {
      if (n == 1) {
        return position;
      } else {
        n--;
      }
    }
    value >>= 1;
    position++;
  }
  return -1;
}

void set_memory(std::map<uint64_t, uint64_t> &memory, uint64_t address,
                uint64_t value, uint64_t floating_mask) {
  if (floating_mask == 0) {
    memory[address] = value;
  } else {
    auto loc = location_of_nth_bit(floating_mask, 1);
    auto bit = 1ul << loc;
    floating_mask = floating_mask & ~bit;
    set_memory(memory, address & ~bit, value, floating_mask);
    set_memory(memory, address | bit, value, floating_mask);
  }
}

int main() {
  auto str = std::ifstream{"data14.txt"};
  auto memory = std::map<uint64_t, uint64_t>{};
  auto i = int{};
  std::string buffer;
  auto on_mask = 0ul;
  auto floating_mask = 0ul;

  while (std::getline(str, buffer)) {
    if (buffer.substr(0, 4) == "mask") {
      buffer = buffer.substr(7);
      auto masks = extract_masks(buffer);
      on_mask = masks.first;
      floating_mask = masks.second;
    } else {
      auto address = std::stoul(buffer.data() + buffer.find('[') + 1);
      auto value = std::stoul(buffer.data() + buffer.find('=') + 2);

      // zero bits related to floating_mask
      address = address & ~(floating_mask) | on_mask;

      // could do this iteratively but the maths is a hassle
      set_memory(memory, address, value, floating_mask);
    }
  }
  auto sum = 0l;
  for (auto &kv : memory) {
    sum += kv.second;
  }

  std::cout << "Sum of memory = " << sum << std::endl;

  return 0;
}
