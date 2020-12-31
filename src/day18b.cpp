#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Token {
  bool isValue; // is true if value is integer value, is false if it's a symbol
  long value;   
};

using TokenList = std::vector<Token>;

void print_tokens(const TokenList &tokens) {
  for (auto tok : tokens) {
    if (tok.isValue) {
      std::cout << tok.value;
    } else {
      std::cout << char(tok.value);
    }
  }
}

TokenList tokenise(std::istream &is) {
  auto result = TokenList{};
  auto next = int{};
  while ((next = is.peek()) != std::istream::traits_type::eof()) {
    if (std::isspace(next)) {
      is.ignore(); // skip a character
    } else if (std::isdigit(next)) {
      auto i = 0;
      is >> i;
      result.push_back(Token{true, i});
    } else {
      is.ignore(); // skip a character
      result.push_back(Token{false, next});
    }
  }
  return result;
}

bool reduce_parens(TokenList &tokens) {
  for (auto i = 0; i + 2 < tokens.size(); i++) {
    if (!tokens[i].isValue && tokens[i].value == '(' && tokens[i + 1].isValue &&
        !tokens[i + 2].isValue && tokens[i + 2].value == ')') {
      tokens.erase(tokens.begin() + i + 2);
      tokens.erase(tokens.begin() + i);
      return true;
    }
  }
  return false;
}

bool reduce_addition(TokenList &tokens) {
  auto did_somthing = false;
  for (auto i = 0; i + 2 < tokens.size(); i++) {
    if (tokens[i].isValue && !tokens[i + 1].isValue &&
        tokens[i + 1].value == '+' && tokens[i + 2].isValue) {
      auto sum = tokens[i].value + tokens[i + 2].value;
      tokens.erase(tokens.begin() + i + 2);
      tokens.erase(tokens.begin() + i + 1);
      tokens[i].value = sum;
      return true;
    }
  }
  return false;
}

bool reduce_multiplication(TokenList &tokens) {
  for (auto i = 0; i + 2 < tokens.size(); i++) {
    // If we see number * number and the symbols before and after are not plus we can do this reduction
    if (tokens[i].isValue && !tokens[i + 1].isValue &&
        tokens[i + 1].value == '*' && tokens[i + 2].isValue &&
        (i == 0 || tokens[i - 1].value != '+') &&
        (i + 3 >= tokens.size() || tokens[i + 3].value != '+')) {
      auto product = tokens[i].value * tokens[i + 2].value;
      tokens.erase(tokens.begin() + i + 2);
      tokens.erase(tokens.begin() + i + 1);
      tokens[i].value = product;
      return true;
    }
  }
  return false;
}

TokenList reduce(TokenList tokens) {
  // Try teh reductions in precedence.
  // Note precedence isn't really important, as the reduction rules can be
  // applied in any order. The Lookback/lookahead for the plus symbol in the
  // multiplication reducer ensures multiplications are not reduced where there
  // is an neighbouring operator of greater precedence.
  while (true) {
    if (!reduce_parens(tokens)) {
      if (!reduce_addition(tokens)) {
        if (!reduce_multiplication(tokens)) {
          break;
        }
      }
    }
  }

  return tokens;
}

// Rather and build an expression tree to solve this problem, we just apply reductions to the token list using some basic rules
int main() {
  auto str = std::ifstream{"data18.txt"};
  auto line = std::string{};

  auto sum = 0ul;

  while (std::getline(str, line)) {
    auto ss = std::stringstream{line};
    auto tokens = tokenise(ss);
    auto reduced = reduce(tokens);
    sum += reduced[0].value;
  }
  std::cout << "sum of equations = " << sum << std::endl;
  return 0;
}
