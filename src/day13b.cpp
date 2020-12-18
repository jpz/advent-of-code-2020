#include <boost/multiprecision/cpp_int.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using boost::multiprecision::int1024_t;

using bigint_t = int1024_t;

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

  auto digits = std::vector<int>{};
  auto offsets = std::vector<int>{};
  auto distance = 0;

  for (auto f : fields) {
    if (std::isdigit(f[0])) {
      digits.push_back(std::atol(f.data()));
      offsets.push_back(distance);
    }
    distance++;
  }

  bigint_t A = digits[0];
  bigint_t X = bigint_t{0};
  for (auto i = 1; i < digits.size(); i++) {
    auto B = digits[i];
    auto Y = bigint_t{offsets[i]};
    // Let us take for instance digits 3 and 7 with offsets of 0 and 1,
    // e.g. A = 3, B = 7, X = 0, Y = 1
    //
    // We are searching for solutions of Ak + X = Bj - Y, for
    // the lowest value of k.
    //
    // We can find a solution for k = 2.
    //
    // 2*3+0 = 1*7-1
    //
    // And we can note this equivalence will repeat every 3*7
    //
    // e.g. 6, 27, 48, ...
    //
    // Thus our new sequence is 6 + 21.k, or A = 21, X = 6
    // which reduces the problem.
    //
    // So, generalising, coprime A,B with offsets (X,Y) has a minimal
    // solution kA + X = jB - Y for lowest k >= 1
    //
    // This generates a new sequence repeating every AB with offset j.A + X
    //
    auto j = 1l;
    while ((j * A + X + Y) % B) {
      j++;
    }
    X = j * A + X;
    A = A * B;
  }

  std::cout << "digit " << X << std::endl;

  return 0;
}
