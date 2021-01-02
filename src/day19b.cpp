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

using RuleContainer = std::unordered_map<RuleId, Rule>;
using RuleStringContainer =
    std::unordered_map<RuleId, std::unordered_set<std::string>>;


bool starts_with(const std::string &s, const std::string &prefix) {
  return s.size() >= prefix.size() && s.find(prefix) == 0;
}


// Applies the rule to some set of strings, and returns a set of all
// possible string remainders when all rule possibilities are applied.
// Note that while the order of the algorithm seems to grow geometrically
// with recursive descent, the targets implicitly restrict the computation.
std::unordered_set<std::string>
consume(int rule_id, const RuleContainer &rule_map,
        const std::unordered_set<std::string> &targets) {
  if (targets.size() == 0) {
    return {};
  }
  auto &rule = rule_map.at(rule_id);
  auto remainders = std::unordered_set<std::string>{};
  for (auto &sub_rule : rule.sub_rules) {
    if (std::holds_alternative<std::string>(sub_rule)) {
      auto prefix = std::get<std::string>(sub_rule);
      for (auto &target : targets) {
        if (starts_with(target, prefix)) {
          remainders.insert(target.substr(prefix.size()));
        }
      }
    } else {
      auto &rule_id_sequence = std::get<RuleIdSequence>(sub_rule);
      auto new_targets = targets;
      for (auto rule_id : std::get<RuleIdSequence>(sub_rule)) {
        if (new_targets.size() == 0) {
          break;
        }
        new_targets = consume(rule_id, rule_map, new_targets);
      }
      remainders.insert(new_targets.begin(), new_targets.end());
    } 
  }
  return remainders;
}

// Rather and build an expression tree to solve this problem, we just apply
// reductions to the token list using some basic rules
int main() {
  auto str = std::ifstream{"data19.txt"};

  auto rules = read_rules(str);

  // update rule 8 and rule 11
  auto &rule8 = *(std::find_if(rules.begin(), rules.end(),
                              [](auto &r) { return r.rule_id == 8; }));
  auto &rule11 = *(std::find_if(rules.begin(), rules.end(),
                                [](auto &r) { return r.rule_id == 11; }));

  rule8.sub_rules.push_back(RuleIdSequence{42});
  rule8.sub_rules.push_back(RuleIdSequence{42, 8});
  rule11.sub_rules.push_back(RuleIdSequence{42, 31});
  rule11.sub_rules.push_back(RuleIdSequence{42, 11, 31});

  auto rule_map = RuleContainer{};
  for (auto &rule : rules) {
    rule_map[rule.rule_id] = rule;
  }

  auto line = std::string{};
  auto count = 0;

  while (std::getline(str, line)) {
    if (line.size() == 0) {
      continue;
    }
    auto remainders =
        consume(0, rule_map, std::unordered_set<std::string>{line});
    if (remainders.find("") != remainders.end()) {
      count++;
    }
  }

  std::cout << "matches = " << count << std::endl;

  return 0;
}
