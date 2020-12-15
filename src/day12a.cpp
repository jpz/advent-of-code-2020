#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

int main() {
  auto set = std::unordered_set<int>{};
  auto str = std::ifstream{"data12.txt"};
  auto line = std::string{};

  // let's do directions with a (x,y) vector
  // N is (0, 1), S is (0, -1), E (1, 0), W (-1, 0)
  auto heading = std::make_pair(1, 0);
  auto position = std::make_pair(0, 0);

  while (std::getline(str, line)) {
    auto command = line[0];
    auto number = std::atoi(line.data() + 1);
    std::cout << line << " " << position.first << " " << position.second
              << " -> ";
    switch (command) {
    case 'N':
      position.second += number;
      break;
    case 'S':
      position.second -= number;
      break;
    case 'E':
      position.first += number;
      break;
    case 'W':
      position.first -= number;
      break;
    case 'F':
      position.first += heading.first * number;
      position.second += heading.second * number;
      break;
    }
    if (line == "R90" || line == "L270") {
      // clockwise rotation matrix [ [0 1] [-1 0] ]
      heading = std::make_pair(1 * heading.second, -1 * heading.first);
    }
    if (line == "L90" || line == "R270") {
      // counterclockwise rotation matrix [ [0 -1] [1 0] ]
      heading = std::make_pair(-1 * heading.second, 1 * heading.first);
    }
    if (line == "L180" || line == "R180") {
      heading = std::make_pair(-1 * heading.first, -1 * heading.second);
    }

    std::cout << position.first << " " << position.second << std::endl;
  }
  std::cout << "Manhattan distance = "
            << std::abs(position.first) + std::abs(position.second)
            << std::endl;
  return 0;
}
