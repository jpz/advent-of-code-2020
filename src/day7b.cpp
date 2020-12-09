#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

struct Key {
  std::string shade;
  std::string colour;
  bool operator<(const Key &k) const {
    return shade == k.shade ? colour < k.colour : shade < k.shade;
  }
};

struct Node {
  Key head;
  std::vector<std::pair<Key, int>> children;
};

Node parse_line(const std::string &line) {
  auto ss = std::stringstream(line.c_str());
  auto result = Node{};
  auto shade = std::string{};
  auto colour = std::string{};
  auto discard = std::string{};

  ss >> shade >> colour >> discard >> discard; // discard "bags" "contain"
  result.head = {shade, colour};

  auto count = 0;

  while (ss >> count >> shade >> colour >> discard) {
    // discard here dumps "bags[.,]" or "bag[.,]"
    result.children.push_back(std::make_pair(Key{shade, colour}, count));
  }

  return result;
}

int main() {
  auto str = std::ifstream{"data7.txt"};
  auto s = std::string{};

  auto map = std::map<Key, Node>{};

  while (std::getline(str, s)) {
    auto node = parse_line(s);
    map[node.head] = node;
  }

  auto root = Key{"shiny", "gold"};

  auto bag_count_memo = std::map<Key, int>{};

  // depth-first memoized search - returns count of bags including self.
  std::function<int(const Key &)> bag_count =
      [&map, &bag_count_memo, &bag_count](const Key &key) -> int {
    auto it = bag_count_memo.find(key);
    if (it != bag_count_memo.end()) { // if memoized value exists, return this
      return it->second;
    }
    int sum = 1;                      // must count self, start at 1 not 0!
    if (map.find(key) != map.end()) { // if key has entry in map add children
      for (auto const &kv : map[key].children) {
        auto bc = bag_count(kv.first);
        sum += bc * kv.second;
      }
    }
    // update memoized value and return
    bag_count_memo[key] = sum;
    return bag_count_memo[key];
  };

  std::cout << "total bags contained within shiny gold = "
            << bag_count(root) - 1 << std::endl;

  return 0;
}
