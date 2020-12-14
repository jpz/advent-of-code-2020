#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

struct password_policy {
  int lower;
  int upper;
  char ch;
};

std::istream &operator>>(std::istream &in, password_policy &pp) {
  auto dash = char{};
  in >> pp.lower >> dash >> pp.upper >> pp.ch;
  if (dash != '-') {
    in.setstate(std::ios_base::failbit);
  }
  return in;
}

int main() {
  auto str = std::ifstream{"data02.txt"};
  auto pp = password_policy{};
  auto eat = char{};
  auto password = std::string{};
  auto count_valid = 0;
  while ((str >> pp >> eat >> password) && str.good()) {
    auto count = std::count(password.begin(), password.end(), pp.ch);
    if (count >= pp.lower and count <= pp.upper) {
      count_valid++;
    }
  }
  std::cout << "valid passwords = " << count_valid << std::endl;
}
