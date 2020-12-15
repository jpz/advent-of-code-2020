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
  auto position = std::make_pair(0, 0);
  auto waypoint = std::make_pair(10, 1);

  while (std::getline(str, line)) {
    auto command = line[0];
    auto number = std::atoi(line.data() + 1);
    switch (command) {
    case 'N':
      waypoint.second += number;
      break;
    case 'S':
      waypoint.second -= number;
      break;
    case 'E':
      waypoint.first += number;
      break;
    case 'W':
      waypoint.first -= number;
      break;
    case 'F':
      position.first += waypoint.first * number;
      position.second += waypoint.second * number;
      break;
    }
    if (line == "R90" || line == "L270") {
      // clockwise rotation matrix [ [0 1] [-1 0] ]
      waypoint = std::make_pair(1 * waypoint.second, -1 * waypoint.first);
    }
    if (line == "L90" || line == "R270") {
      // counterclockwise rotation matrix [ [0 -1] [1 0] ]
      waypoint = std::make_pair(-1 * waypoint.second, 1 * waypoint.first);
    }
    if (line == "L180" || line == "R180") {
      waypoint = std::make_pair(-1 * waypoint.first, -1 * waypoint.second);
    }
  }
  std::cout << "Manhattan distance = "
            << std::abs(position.first) + std::abs(position.second)
            << std::endl;
  return 0;
}
