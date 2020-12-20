#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

long evaluate(std::istream &is);

long get_term_value(std::istream &is) {
  auto i = long{};
  is >> i;
  if (!is.fail()) {
    return i;
  }

  is.clear();
  auto ch = char{};
  is >> ch;
  if (ch == '(') {
    // note evaluate will consume the matching closing paren
    auto retval = evaluate(is);
    return retval;
  } else {
    throw std::runtime_error("unexpected input");
  }
}

long evaluate(std::istream &is) {
  auto op = char{};
  auto accumulator = get_term_value(is);
  is >> op;

  while (!is.eof()) {
    auto num = get_term_value(is);
    switch (op) {
    case '+':
      accumulator += num;
      break;
    case '-':
      accumulator -= num;
      break;
    case '*':
      accumulator *= num;
      break;
    default:
      throw std::runtime_error("unexpected operator " + op);
    }
    is >> op;
    if (op == ')') {
      return accumulator;
    }
  }
  return accumulator;
}

int main() {
  auto str = std::ifstream{"data18.txt"};
  auto line = std::string{};

  long sum = 0;
  while (std::getline(str, line)) {
    auto ss = std::stringstream{line};
    auto result = evaluate(ss);
    // std::cout << line << " = " << result << std::endl;
    sum += result;
  }
  std::cout << "sum of equations = " << sum << std::endl;
  return 0;
}
