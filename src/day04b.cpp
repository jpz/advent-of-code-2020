#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

bool date_is_valid(const std::string &dt, int min, int max) {
  auto i = int();
  if (dt.size() != 4)
    return false;
  std::sscanf(dt.c_str(), "%d", &i);
  return i >= min and i <= max;
}

bool height_is_valid(const std::string &h) {
  auto value = int{};
  auto result = std::from_chars(h.data(), h.data() + h.size(), value);
  return std::string("cm") == std::string(result.ptr) && value >= 150 and
             value <= 193 or
         std::string("in") == std::string(result.ptr) && value >= 59 and
             value <= 76;
}

bool is_six_char_hex(const std::string &h) {
  return h.length() == 7 and h[0] == '#' and
         std::count_if(h.begin() + 1, h.end(), [](auto ch) {
           return (ch >= '0' and ch <= '9') or (ch >= 'a' or ch <= 'f');
         }) == 6;
}

bool eye_colour_is_valid(const std::string &s) {
  return s == "amb" or s == "blu" or s == "brn" or s == "gry" or s == "grn" or
         s == "hzl" or s == "oth";
}

bool is_valid_passport_id(const std::string &s) {
  return s.size() == 9 && std::count_if(s.begin(), s.end(), [](auto ch) {
                            return ch >= '0' and ch <= '9';
                          }) == 9;
}

bool is_valid_passport(
    const std::unordered_map<std::string, std::string> &tokens) {
  if (tokens.find("byr") == tokens.end() or
      tokens.find("iyr") == tokens.end() or
      tokens.find("eyr") == tokens.end() or
      tokens.find("hgt") == tokens.end() or
      tokens.find("hcl") == tokens.end() or
      tokens.find("ecl") == tokens.end() or
      tokens.find("pid") == tokens.end()) {
    return false;
  } else {
    return date_is_valid(tokens.at("byr"), 1920, 2002) and
           date_is_valid(tokens.at("iyr"), 2010, 2020) and
           date_is_valid(tokens.at("eyr"), 2020, 2030) and
           height_is_valid(tokens.at("hgt")) and
           is_six_char_hex(tokens.at("hcl")) and
           eye_colour_is_valid(tokens.at("ecl")) and
           is_valid_passport_id(tokens.at("pid"));
  }
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
