#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

// Rule:
//  rule_id and one or more SubRule (any of which are valid)
// SubRule: either a sequence of rule_ids, or a string (typically "a" or
// "b") e.g. 1: 2 3 | 4 5 6

using RuleId = int;
using RuleIdSequence = std::vector<RuleId>; // a sequence of nodes
using SubRule = std::variant<RuleIdSequence, std::string>;

struct Rule {
  RuleId rule_id;
  std::vector<SubRule> sub_rules;
};

SubRule parse_subrule(const std::string &s) {
  // subrule either is a list of numbers, or it is a double-quote surrounded
  // string
  auto quote_pos = s.find('"');
  if (quote_pos != std::string::npos) {
    auto close_quote_pos = s.find('"', quote_pos + 1);
    return SubRule{s.substr(quote_pos + 1, close_quote_pos - quote_pos - 1)};
  } else {
    auto ss = std::stringstream{s};
    auto i = 0;
    auto seq = RuleIdSequence{};
    while (ss >> i) {
      seq.push_back(i);
    }
    return SubRule{seq};
  }
}

Rule parse_rule_line(const std::string &line) {
  auto rule = Rule{};
  rule.rule_id = std::stoi(line);
  auto pos = line.find(':') + 2;
  auto end_pos = line.find('|', pos);
  while (end_pos != std::string::npos) {
    rule.sub_rules.push_back(
        parse_subrule(line.substr(pos, end_pos - pos - 1)));
    pos = end_pos + 1;
    end_pos = line.find('|', pos);
  }
  rule.sub_rules.push_back(parse_subrule(line.substr(pos)));
  return rule;
}

std::vector<Rule> read_rules(std::istream &is) {
  auto line = std::string{};
  auto rules = std::vector<Rule>{};
  while (std::getline(is, line)) {
    if (line.size() > 0) {
      rules.push_back(parse_rule_line(line));
    } else {
      // blank line indicates end of rules;
      break;
    }
  }
  return rules;
}

void print_rule(const Rule &rule) {
  std::cout << rule.rule_id << ": ";
  auto first_sub_rule = true;
  for (auto &sub_rule : rule.sub_rules) {
    if (first_sub_rule) {
      first_sub_rule = false;
    } else {
      std::cout << " | ";
    }
    if (std::holds_alternative<RuleIdSequence>(sub_rule)) {
      auto first_seq = true;
      for (auto i : std::get<RuleIdSequence>(sub_rule)) {
        if (first_seq) {
          first_seq = false;
          std::cout << i;
        } else {
          std::cout << " " << i;
        }
      }
    } else { // holds std::string
      std::cout << '"' << std::get<std::string>(sub_rule) << '"';
    }
  }
}

using RuleContainer = std::unordered_map<RuleId, Rule>;
using RuleStringContainer =
    std::unordered_map<RuleId, std::unordered_set<std::string>>;

bool contains(const std::string &s, const std::string &suffix) {
  return s.find(suffix) != std::string::npos;
}

std::unordered_set<std::string>
compute_strings(const Rule &rule,
                const RuleStringContainer &rule_container_string,
                const std::string &target) {
  auto strings = std::unordered_set<std::string>{};
  for (auto &sub_rule : rule.sub_rules) {
    if (std::holds_alternative<std::string>(sub_rule)) {
      auto &s = std::get<std::string>(sub_rule);
      if (contains(target, s)) {
        strings.insert(s);
      }
    } else {
      auto &rule_id_sequence = std::get<RuleIdSequence>(sub_rule);
      if (rule_id_sequence.size() == 1) {
        for (auto &s : rule_container_string.at(rule_id_sequence[0])) {
          if (contains(target, s)) {
            strings.insert(s);
          }
        }
      }
      if (rule_id_sequence.size() == 2) {
        for (auto &s1 : rule_container_string.at(rule_id_sequence[0])) {
          for (auto &s2 : rule_container_string.at(rule_id_sequence[1])) {
            auto s = s1 + s2;
            if (contains(target, s)) {
              strings.insert(s);
            }
          }
        }
      }
    }
  }
  return strings;
}

bool match_exists(const std::vector<Rule> &rules, const std::string &target) {
  auto rule_map = RuleContainer{};
  auto unprocessed_rules = std::unordered_set<RuleId>{};
  for (auto &rule : rules) {
    rule_map[rule.rule_id] = rule;
    unprocessed_rules.insert(rule.rule_id);
  }

  // build up a list of strings for each rule
  // omit any strings which cannot exist in the target
  auto rule_strings = RuleStringContainer{};

  while (unprocessed_rules.size()) {
    for (auto &rule_id : unprocessed_rules) {
      auto &rule = rule_map[rule_id];
      auto computable = true;
      for (auto &sub_rule : rule.sub_rules) {
        if (std::holds_alternative<std::string>(sub_rule)) {
          // no-op
        } else {
          for (auto rule_id : std::get<RuleIdSequence>(sub_rule)) {
            if (rule_strings.find(rule_id) == rule_strings.end()) {
              computable = false;
            }
          }
        }
      }
      if (computable) {
        auto strings = compute_strings(rule, rule_strings, target);
        rule_strings[rule.rule_id] = strings;
        unprocessed_rules.erase(rule_id);
        break; // start for loop again
      }
    }
  }

  auto &solutions_for_zero = rule_strings[0];
  return solutions_for_zero.find(target) != solutions_for_zero.end();
}

// Rather and build an expression tree to solve this problem, we just apply
// reductions to the token list using some basic rules
int main() {
  auto str = std::ifstream{"data19.txt"};

  auto rules = read_rules(str);

  auto line = std::string{};
  auto count = 0;

  while (std::getline(str, line)) {
    if (line.size() == 0) {
      continue;
    }
    count += match_exists(rules, line);
  }

  std::cout << "matches = " << count << std::endl;

  return 0;
}
