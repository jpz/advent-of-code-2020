#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

bool is_valid_passport(
    const std::unordered_map<std::string, std::string> &tokens) {
  if (tokens.find("byr") == tokens.end() or
      tokens.find("iyr") == tokens.end() or
      tokens.find("eyr") == tokens.end() or
      tokens.find("hgt") == tokens.end() or
      tokens.find("hcl") == tokens.end() or
      tokens.find("ecl") == tokens.end() or tokens.find("pid") == tokens.end())
    return false;
  else
    return true;
}

std::vector<std::pair<std::string, std::string>>
get_tokens_from_line(const std::string &line) {
  auto ss = std::stringstream{line};
  auto result = std::vector<std::pair<std::string, std::string>>{};
  auto keyvalue = std::string{};
  while (ss >> keyvalue) {
    auto colon_location = keyvalue.find(':');
    result.push_back(std::make_pair(std::string(keyvalue, 0, colon_location),
                                    std::string(keyvalue, colon_location + 1)));
  }
  return result;
}

int main() {
  auto tokens = std::unordered_map<std::string, std::string>{};
  auto str = std::ifstream{"data04.txt"};
  auto line = std::string{};

  auto valid_passport_count = 0;

  while (std::getline(str, line)) {
    if (line.size() == 0) {
      valid_passport_count += is_valid_passport(tokens);
      tokens.clear();
    } else {
      for (auto &[k, v] : get_tokens_from_line(line)) {
        tokens[k] = v;
      }
    }
  }
  valid_passport_count += is_valid_passport(tokens);

  std::cout << "Valid passport count = " << valid_passport_count << std::endl;

  return 0;
}
