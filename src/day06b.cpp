#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

int main() {
  auto questions = std::set<char>{};
  auto str = std::ifstream{"data06.txt"};
  auto line = std::string{};

  auto sum_of_questions_in_groups = 0;
  auto first_set_seen = false;

  while (std::getline(str, line)) {
    if (line.size() == 0) {
      sum_of_questions_in_groups += questions.size();
      first_set_seen = false;
    } else {
      auto answered_questions = std::set<char>(line.begin(), line.end());
      if (!first_set_seen) {
        questions = answered_questions;
        first_set_seen = true;
      } else {
        auto intersection = std::set<char>{};
        std::set_intersection(
            questions.begin(), questions.end(), answered_questions.begin(),
            answered_questions.end(),
            std::inserter(intersection, intersection.begin()));
        questions = intersection;
      }
    }
  }
  if (first_set_seen) {
    sum_of_questions_in_groups += questions.size();
  }

  std::cout << "Sum of unique questions answered by all, in all groups = "
            << sum_of_questions_in_groups << std::endl;

  return 0;
}
