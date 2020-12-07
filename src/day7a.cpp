#include <deque>
#include <fstream>
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
  auto reverse_map = std::map<Key, std::set<Key>>{};

  while (std::getline(str, s)) {
    auto node = parse_line(s);
    map[node.head] = node;
    for (auto key : node.children) {
      reverse_map[key.first].insert(node.head);
    }
  }

  auto root = Key{"shiny", "gold"};

  auto nodes_seen = std::set<Key>{root};
  auto nodes_to_visit =
      std::deque<Key>{reverse_map[root].begin(), reverse_map[root].end()};
  for (auto kk : reverse_map[root]) {
    nodes_seen.insert(kk);
  }
  while (nodes_to_visit.size()) {
    auto child = nodes_to_visit.front();
    nodes_to_visit.pop_front();
    for (auto parent : reverse_map[child]) {
      if (nodes_seen.find(parent) == nodes_seen.end()) {
        nodes_seen.insert(parent);
        nodes_to_visit.push_back(parent);
      }
    }
  }

  std::cout << "total parents of shiny gold = " << nodes_seen.size() - 1
            << std::endl;

  return 0;
}
