#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
//#include <algorithm>
//#include <tuple>

struct Point {
  int x;
  int y;
  int z;
  int w;

  bool operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
  }
};

namespace std {
template <> struct hash<Point> {
  size_t operator()(const Point &p) const {
    return p.x + p.y * 1000 + p.z * 1000 * 1000 + p.w * 1000 * 1000 * 1000;
  }
};
} // namespace std

class SpaceMap {
  std::unordered_set<Point> active_locations_;

public:
  void activate(const Point &p) { active_locations_.emplace(p); }
  void deactivate(const Point &p) { active_locations_.erase(p); }
  bool active(const Point &p) const {
    return active_locations_.find(p) != active_locations_.end();
  }
  int active_count() const { return active_locations_.size(); }
  int neighbour_count(const Point &p) const {
    int count = -(active_locations_.find(p) != active_locations_.end());
    for (int ix = p.x - 1; ix <= p.x + 1; ix++) {
      for (int iy = p.y - 1; iy <= p.y + 1; iy++) {
        for (int iz = p.z - 1; iz <= p.z + 1; iz++) {
          for (int iw = p.w - 1; iw <= p.w + 1; iw++) {
            count += (active_locations_.find(Point{ix, iy, iz, iw}) !=
                      active_locations_.end());
          }
        }
      }
    }
    return count;
  }

  std::pair<int, int> get_x_bounds() const {
    int min = 0;
    int max = 0;
    for (auto &p : active_locations_) {
      min = std::min(min, p.x);
      max = std::max(max, p.x);
    }
    return std::make_pair(min, max);
  }
  std::pair<int, int> get_y_bounds() const {
    int min = 0;
    int max = 0;
    for (auto &p : active_locations_) {
      min = std::min(min, p.y);
      max = std::max(max, p.y);
    }
    return std::make_pair(min, max);
  }
  std::pair<int, int> get_z_bounds() const {
    int min = 0;
    int max = 0;
    for (auto &p : active_locations_) {
      min = std::min(min, p.z);
      max = std::max(max, p.z);
    }
    return std::make_pair(min, max);
  }
  std::pair<int, int> get_w_bounds() const {
    int min = 0;
    int max = 0;
    for (auto &p : active_locations_) {
      min = std::min(min, p.w);
      max = std::max(max, p.w);
    }
    return std::make_pair(min, max);
  }
};

int main() {
  auto str = std::ifstream{"data17.txt"};
  auto line = std::string{};
  auto space_map = SpaceMap{};

  int y = 0;
  while (str >> line) {
    for (auto x = 0; x < line.length(); x++) {
      if (line[x] == '#') {
        space_map.activate(Point{x, y, 0});
      }
    }
    y++;
  }

  for (auto cycle = 0; cycle < 6; cycle++) {
    auto space_map_new = SpaceMap{};
    auto x_bounds = space_map.get_x_bounds();
    auto y_bounds = space_map.get_y_bounds();
    auto z_bounds = space_map.get_z_bounds();
    auto w_bounds = space_map.get_w_bounds();

    for (int x = x_bounds.first - 1; x <= x_bounds.second + 1; x++) {
      for (int y = y_bounds.first - 1; y <= y_bounds.second + 1; y++) {
        for (int z = z_bounds.first - 1; z <= z_bounds.second + 1; z++) {
          for (int w = w_bounds.first - 1; w <= w_bounds.second + 1; w++) {
            auto p = Point{x, y, z, w};
            auto active = space_map.active(p);
            auto c = space_map.neighbour_count(p);
            if (active && c == 2 || c == 3) {
              space_map_new.activate(p);
            }
            if (!active and c == 3) {
              space_map_new.activate(p);
            }
          }
        }
      }
    }
    space_map = space_map_new;
  }

  std::cout << "Active after 6 cycles = " << space_map.active_count()
            << std::endl;

  return 0;
}
