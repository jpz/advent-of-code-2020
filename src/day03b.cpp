#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main() {
  auto map = std::vector<std::string>{};
  auto str = std::ifstream{"data03.txt"};
  auto row = std::string{};

  while (str >> row && str.good()) {
    map.push_back(row);
  }

  auto const velocities =
      std::vector<std::pair<int, int>>{{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};

  auto tree_encounters = std::vector<int64_t>(velocities.size());

  for (auto i = 0; i < velocities.size(); i++) {
    auto const [velocity_x, velocity_y] = velocities[i];
    auto x = 0;
    auto y = 0;

    while (y < map.size()) {
      if (map[y][x] == '#')
        tree_encounters[i]++;
      y += velocity_y;
      x += velocity_x;
      x %= map[0].size();
    }
  }

  // note the int64_t is important otherwise we get an int32_t reducer and
  // overflow
  auto product = std::accumulate(tree_encounters.begin(), tree_encounters.end(),
                                 int64_t{1}, std::multiplies<>());

  std::cout << "Trees encountered product = " << product << std::endl;

  return 0;
}
