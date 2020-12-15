#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split_on_char(const std::string &text, char tok) {
  auto iss = std::istringstream{text};
  auto buf = std::string{};
  auto result = std::vector<std::string>{};

  while (std::getline(iss, buf, tok)) {
    result.push_back(buf);
  }
  return result;
}

int main() {
  auto str = std::ifstream{"data13.txt"};
  auto buf = std::string{};

  std::getline(str, buf);
  auto time = std::atol(buf.data());

  std::getline(str, buf);
  auto fields = split_on_char(buf, ',');

  auto min_wait = time;
  auto earliest_bus_number = 0l;

  for (auto f : fields) {
    if (std::isdigit(f[0])) {
      auto bus_number = std::atol(f.data());
      auto m = bus_number - time % bus_number;
      if (m < min_wait) {
        min_wait = m;
        earliest_bus_number = bus_number;
      }
    }
  }

  std::cout << "Earliest bus is " << earliest_bus_number
            << " with waiting time " << min_wait
            << ", product = " << earliest_bus_number * min_wait << std::endl;

  return 0;
}
