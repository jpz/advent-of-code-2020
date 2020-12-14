#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int neighbours_occupied_count(const std::vector<std::string> &map, int r,
                              int c) {
  auto const rows = map.size();
  auto const cols = map[0].size();

  auto neighbour_count = 0;
  // check left
  for (auto i = c - 1; i >= 0; i--) {
    if (map[r][i] == 'L') {
      break;
    }
    if (map[r][i] == '#') {
      neighbour_count++;
      break;
    }
  }
  // check right
  for (auto i = c + 1; i < cols; i++) {
    if (map[r][i] == 'L') {
      break;
    }
    if (map[r][i] == '#') {
      neighbour_count++;
      break;
    }
  }
  // check up
  for (auto i = r - 1; i >= 0; i--) {
    if (map[i][c] == 'L') {
      break;
    }
    if (map[i][c] == '#') {
      neighbour_count++;
      break;
    }
  }
  // check down
  for (auto i = r + 1; i < rows; i++) {
    if (map[i][c] == 'L') {
      break;
    }
    if (map[i][c] == '#') {
      neighbour_count++;
      break;
    }
  }
  // check up/left
  for (auto i = 1; i <= std::max(rows, cols); i++) {
    if (r - i < 0 || c - i < 0) {
      break;
    }
    if (map[r - i][c - i] == 'L') {
      break;
    }
    if (map[r - i][c - i] == '#') {
      neighbour_count++;
      break;
    }
  }
  // check up/right
  for (auto i = 1; i <= std::max(rows, cols); i++) {
    if (r - i < 0 || c + i >= cols) {
      break;
    }
    if (map[r - i][c + i] == 'L') {
      break;
    }
    if (map[r - i][c + i] == '#') {
      neighbour_count++;
      break;
    }
  }
  // check down/left
  for (auto i = 1; i < std::max(rows, cols); i++) {
    if (r + i >= rows || c - i < 0) {
      break;
    }
    if (map[r + i][c - i] == 'L') {
      break;
    }
    if (map[r + i][c - i] == '#') {
      neighbour_count++;
      break;
    }
  }
  // check down/right
  for (auto i = 1; i < std::max(rows, cols); i++) {
    if (r + i >= rows || c + i >= cols) {
      break;
    }
    if (map[r + i][c + i] == 'L') {
      break;
    }
    if (map[r + i][c + i] == '#') {
      neighbour_count++;
      break;
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
        if (map[r][c] == '#' && neighbours_occupied_count(map, r, c) >= 5) {
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
