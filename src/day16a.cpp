#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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

  bool in_range(int i) {
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

int main() {
  auto tickets = std::vector<Ticket>{};
  auto criteria = std::vector<Criteria>{};
  auto str = std::ifstream{"data16.txt"};

  auto s = std::string{};

  bool initialising_criteria = true;
  bool reading_nearby_tickets = false;

  while (std::getline(str, s)) {
    if (s.length() > 0) {
      if (initialising_criteria) {
        auto crit = Criteria();
        crit.initialise_from_string(s);
        criteria.push_back(crit);
      } else if (reading_nearby_tickets) {
        auto tk = Ticket();
        tk.initialise_from_string(s);
        tickets.push_back(tk);
      } else if (s == "nearby tickets:") {
        reading_nearby_tickets = true;
      }
    } else {
      // first blank line switches this mode off
      initialising_criteria = false;
    }
  }

  auto sum_failures = 0;
  for (auto &tk : tickets) {
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
        sum_failures += number;
      }
    }
  }

  std::cout << "ticket scanning error rate = " << sum_failures << std::endl;

  return 0;
}
