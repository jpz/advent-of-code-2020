#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
  auto map = std::vector<std::string>{};
  auto str = std::ifstream{"data03.txt"};
  auto row = std::string{};

  while (str >> row && str.good()) {
    map.push_back(row);
  }

  auto x = 0;
  auto y = 0;
  auto tree_encounters = 0;

  auto const velocity_x = 3;
  auto const velocity_y = 1;

  while (y < map.size()) {
    if (map[y][x] == '#')
      tree_encounters++;
    y += velocity_y;
    x += velocity_x;
    x %= map[0].size();
  }

  std::cout << "Trees encountered = " << tree_encounters << std::endl;

  return 0;
}
