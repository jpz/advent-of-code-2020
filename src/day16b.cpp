#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Criteria {
  void initialise_from_string(const std::string &line) {
    auto colon_pos = line.find(':');
    name = line.substr(0, colon_pos);
    auto or_pos = line.find(" or ");
    auto range1 = line.substr(colon_pos + 2, or_pos - colon_pos - 2);
    auto range2 = line.substr(or_pos + 4);

    auto dash1_pos = range1.find("-");
    auto dash2_pos = range2.find("-");

    lower1 = std::atoi(range1.data());
    upper1 = std::atoi(range1.data() + dash1_pos + 1);
    lower2 = std::atoi(range2.data());
    upper2 = std::atoi(range2.data() + dash2_pos + 1);
  }

  std::string name;
  int lower1 = 0;
  int upper1 = -1;
  int lower2 = 0;
  int upper2 = -1;

  bool in_range(int i) const {
    return (lower1 <= i and i <= upper1) || (lower2 <= i and i <= upper2);
  }
};

struct Ticket {
  void initialise_from_string(const std::string &line) {
    auto ss = std::stringstream{line};
    auto i = 0;
    while (ss >> i) {
      numbers.push_back(i);
      auto ch = ' ';
      ss >> ch; // skip comma as necessary
    }
  }

  std::vector<int> numbers;
};

// return all tickets which are valid (each number there exists a criteria which
// matches)
std::vector<Ticket> filter_tickets(const std::vector<Ticket> &tickets,
                                   const std::vector<Criteria> &criteria) {
  auto result = std::vector<Ticket>{};
  for (auto &tk : tickets) {
    bool matched_all = true;
    for (auto number : tk.numbers) {
      bool found_matching_criteria = false;
      for (auto &crit : criteria) {
        found_matching_criteria =
            found_matching_criteria || crit.in_range(number);
        if (found_matching_criteria) {
          break;
        }
      }
      if (!found_matching_criteria) {
        matched_all = false;
        break;
      }
    }
    if (matched_all) {
      result.push_back(tk);
    }
  }
  return result;
}

bool is_compatible(int column, const std::vector<Ticket> &tickets,
                   const Criteria &criteria) {
  for (auto &ticket : tickets) {
    if (!criteria.in_range(ticket.numbers[column])) {
      return false;
    }
  }
  return true;
}

int main() {
  auto tickets = std::vector<Ticket>{};
  auto criteria = std::vector<Criteria>{};
  auto str = std::ifstream{"data16.txt"};
  auto your_ticket = Ticket{};

  auto s = std::string{};

  bool initialising_criteria = true;
  bool reading_your_ticket = false;
  bool reading_nearby_tickets = false;

  while (std::getline(str, s)) {
    if (s.length() > 0) {
      if (initialising_criteria) {
        auto crit = Criteria();
        crit.initialise_from_string(s);
        criteria.push_back(crit);
      } else if (reading_your_ticket) {
        your_ticket.initialise_from_string(s);
      } else if (reading_nearby_tickets) {
        auto tk = Ticket();
        tk.initialise_from_string(s);
        tickets.push_back(tk);
      } else if (s == "your ticket:") {
        reading_your_ticket = true;
      } else if (s == "nearby tickets:") {
        reading_nearby_tickets = true;
      }
    } else {
      // first blank line switches this mode off
      if (initialising_criteria)
        initialising_criteria = false;
      if (reading_your_ticket)
        reading_your_ticket = false;
    }
  }

  tickets = filter_tickets(tickets, criteria);

  auto column_compatibilities =
      std::unordered_map<int, std::unordered_set<int>>{};

  for (auto column = 0; column < tickets[0].numbers.size(); column++) {
    auto compatibilities = std::unordered_set<int>{};
    for (auto crit_idx = 0; crit_idx < criteria.size(); crit_idx++) {
      if (is_compatible(column, tickets, criteria[crit_idx])) {
        compatibilities.insert(crit_idx);
      }
    }
    column_compatibilities[column] = compatibilities;
  }

  auto solutions = std::unordered_map<int, int>{};

  while (solutions.size() != tickets[0].numbers.size()) {
    for (auto column = 0; column < tickets[0].numbers.size(); column++) {
      if (solutions.find(column) != solutions.end()) {
        continue;
      }
      if (column_compatibilities[column].size() == 1) {
        auto row = *(column_compatibilities[column].begin());
        solutions[column] = row;
        for (auto c2 = 0; c2 < tickets[0].numbers.size(); c2++) {
          column_compatibilities[c2].erase(row);
        }
      }
    }
  }

  auto departure_product = 1l;
  auto const departure = std::string{"departure"};
  for (auto kv : solutions) {
    auto n = criteria[kv.second].name;
    auto ns = n.substr(0, departure.size());
    if (ns == departure) {
      departure_product *= your_ticket.numbers[kv.first];
    }
  }

  std::cout << "departure product = " << departure_product << std::endl;

  return 0;
}
