#include "bootcode.h"
#include <unordered_set>

int run_program_until_loop_and_return_accum(const Program &program,
                                            int starting_location,
                                            int starting_accum) {
  auto accum = starting_accum;
  auto location = starting_location;
  auto visited_locations = std::unordered_set<int>{};
  while (visited_locations.find(location) == visited_locations.end()) {
    visited_locations.insert(location);
    auto instruction = program.at(location);
    if (instruction.opcode == "acc") {
      accum += instruction.parameter;
      location++;
    } else if (instruction.opcode == "jmp") {
      location += instruction.parameter;
    } else if (instruction.opcode == "nop") {
      location++;
    }
  }
  return accum;
}

int main() {
  auto program = read_program("data8.txt");

  std::cout << "Accum value at position of loop = "
            << run_program_until_loop_and_return_accum(program, 0, 0)
            << std::endl;

  return 0;
}
