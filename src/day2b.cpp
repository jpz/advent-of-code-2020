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
  auto str = std::ifstream{"data2.txt"};
  auto pp = password_policy{};
  auto eat = char{};
  auto password = std::string{};
  auto count_valid = 0;
  while ((str >> pp >> eat >> password) && str.good()) {
    auto count = std::count(password.begin(), password.end(), pp.ch);
    if (password.size() >= pp.upper) {
      if (1 == (password[pp.lower - 1] == pp.ch) +
                   (password[pp.upper - 1] == pp.ch)) {
        count_valid++;
      }
    }
  }
  std::cout << "valid passwords = " << count_valid << std::endl;
}
