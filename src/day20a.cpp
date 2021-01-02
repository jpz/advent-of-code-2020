#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

using namespace std::string_literals;

// a tile is described four corners, 0, 1, 2, 3
// 0 ---- 1
// |      |
// |      |
// |      |
// 3 ---- 2
// we store the tile with compact representation as 4 lines of 10 character
// length

struct Tile {
  int tile_no;
  std::string l01;
  std::string l12;
  std::string l32;
  std::string l03;

  // return true if an edge of the tile matches the top edge
  bool match_t(const Tile &t) const {
    return match_t(t.l01) || match_t(t.l12) || match_t(t.l32) || match_t(t.l03);
  }

  // return true if an edge of the tile matches the left edge
  bool match_l(const Tile &t) const {
    return match_l(t.l01) || match_l(t.l12) || match_l(t.l32) || match_l(t.l03);
  }

  // return true if an edge of the tile matches the right edge
  bool match_r(const Tile &t) const {
    return match_r(t.l01) || match_r(t.l12) || match_r(t.l32) || match_r(t.l03);
  }
  // return true if an edge of the tile matches the bottom edge
  bool match_b(const Tile &t) const {
    return match_b(t.l01) || match_b(t.l12) || match_b(t.l32) || match_b(t.l03);
  }

  bool match_t(const std::string &s) const {
    return l01 == s || std::equal(l01.begin(), l01.end(), s.rbegin());
  }
  bool match_r(const std::string &s) const {
    return l12 == s || std::equal(l12.begin(), l12.end(), s.rbegin());
  }
  bool match_b(const std::string &s) const {
    return l32 == s || std::equal(l32.begin(), l32.end(), s.rbegin());
  }
  bool match_l(const std::string &s) const {
    return l03 == s || std::equal(l03.begin(), l03.end(), s.rbegin());
  }
};

std::unordered_map<int, Tile> read_tiles(std::istream &is) {
  auto line = ""s;
  auto prefix = "Tile "s;
  auto result = std::unordered_map<int, Tile>{};
  while (std::getline(is, line)) {
    Tile t;
    if (line.rfind(prefix, 0) == 0) {
      t.tile_no = std::atoi(line.data() + prefix.size());
      auto left_col = ""s;
      auto right_col = ""s;
      for (auto i = 0; i < 10; i++) {
        is >> line;
        if (i == 0) {
          t.l01 = line;
        }
        if (i == 9) {
          t.l32 = line;
        }
        left_col += line[0];
        right_col += line[9];
      }
      t.l03 = left_col;
      t.l12 = right_col;
      result[t.tile_no] = t;
    }
  }
  return result;
}

// Rather and build an expression tree to solve this problem, we just apply
// reductions to the token list using some basic rules
int main() {
  auto str = std::ifstream{"data20.txt"};

  auto tiles = read_tiles(str);

  auto corner_product = 1l;

  for (auto kv1 : tiles) {
    auto tile1_no = kv1.first;
    auto &tile1 = kv1.second;
    auto top_matches = 0;
    auto bottom_matches = 0;
    auto left_matches = 0;
    auto right_matches = 0;
    for (auto kv2 : tiles) {
      auto tile2_no = kv2.first;
      auto &tile2 = kv2.second;
      if (tile1_no != tile2_no) {
        top_matches += tile1.match_t(tile2);
        bottom_matches += tile1.match_b(tile2);
        left_matches += tile1.match_l(tile2);
        right_matches += tile1.match_r(tile2);
      }
    }

    // have verified that there are only 4 tiles with two non-matching edges (i.e. the corners)
    if (top_matches + bottom_matches + left_matches + right_matches == 2) {
      corner_product *= tile1_no;
    }
  }

  std::cout << "product of corner tile ids = " << corner_product << std::endl;

  return 0;
}
