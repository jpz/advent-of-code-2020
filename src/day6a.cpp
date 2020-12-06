#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

int main() {
  auto questions = std::unordered_set<char>{};
  auto str = std::ifstream{"data6.txt"};
  auto line = std::string{};

  auto sum_of_questions_in_groups = 0;

  while (std::getline(str, line)) {
    if (line.size() == 0) {
      sum_of_questions_in_groups += questions.size();
      questions.clear();
    } else {
      for (auto ch : line) {
        questions.insert(ch);
      }
    }
  }
  sum_of_questions_in_groups += questions.size();

  std::cout << "Sum of unique questions in all groups = "
            << sum_of_questions_in_groups << std::endl;

  return 0;
}
