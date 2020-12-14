#include "bootcode.h"
#include <unordered_set>

std::pair<bool, int> does_program_terminate(const Program &program,
                                            int starting_location,
                                            int starting_accum) {
  auto accum = starting_accum;
  auto location = starting_location;
  auto visited_locations = std::unordered_set<int>{};
  while (visited_locations.find(location) == visited_locations.end() &&
         location < program.size()) {
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
  return std::make_pair(location == program.size(), accum);
}

int main() {
  auto program = read_program("data08.txt");

  auto result = std::pair<bool, int>{false, 0};
  // result.first indicates whether the program terminated and we're done
  for (auto i = 0; !result.first && i < program.size(); i++) {
    if (program[i].opcode == "nop") {
      AdjustOpcode adjuster(program, i, "jmp");
      result = does_program_terminate(program, 0, 0);
    } else if (program[i].opcode == "jmp") {
      AdjustOpcode adjuster(program, i, "nop");
      result = does_program_terminate(program, 0, 0);
    }
  }

  std::cout << "accumulator after fix = " << result.second << std::endl;

  return 0;
}
