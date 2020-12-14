#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int neighbours_occupied_count(const std::vector<std::string> &map, int r,
                              int c) {
  auto const rows = map.size();
  auto const cols = map[0].size();
  auto no_left = (c == 0);
  auto no_right = (c == cols - 1);
  auto no_up = (r == 0);
  auto no_down = (r == rows - 1);

  auto neighbour_count = 0;

  if (!no_up) {
    if (!no_left) {
      neighbour_count += (map[r - 1][c - 1] == '#');
    }
    neighbour_count += (map[r - 1][c] == '#');
    if (!no_right) {
      neighbour_count += (map[r - 1][c + 1] == '#');
    }
  }
  if (!no_left) {
    neighbour_count += (map[r][c - 1] == '#');
  }
  if (!no_right) {
    neighbour_count += (map[r][c + 1] == '#');
  }
  if (!no_down) {
    if (!no_left) {
      neighbour_count += (map[r + 1][c - 1] == '#');
    }
    neighbour_count += (map[r + 1][c] == '#');
    if (!no_right) {
      neighbour_count += (map[r + 1][c + 1] == '#');
    }
  }
  return neighbour_count;
}

int main() {
  auto map = std::vector<std::string>{};
  auto str = std::ifstream{"data11.txt"};
  auto row = std::string{};

  while (str >> row) {
    if (row.size()) {
      map.push_back(row);
    }
  }

  auto changed = true;

  auto const rows = map.size();
  auto const cols = map[0].size();

  while (changed) {
    changed = false;
    auto new_map = map;
    for (auto r = 0; r < rows; r++) {
      for (auto c = 0; c < cols; c++) {
        if (map[r][c] == 'L' && neighbours_occupied_count(map, r, c) == 0) {
          changed = true;
          new_map[r][c] = '#';
        }
        if (map[r][c] == '#' && neighbours_occupied_count(map, r, c) >= 4) {
          changed = true;
          new_map[r][c] = 'L';
        }
      }
    }
    map = new_map;
  }

  auto occupied_seats = 0;
  for (auto const &row : map) {
    for (auto seat : row) {
      if (seat == '#') {
        occupied_seats++;
      }
    }
  }

  std::cout << "Occupied seats = " << occupied_seats << std::endl;

  return 0;
}
