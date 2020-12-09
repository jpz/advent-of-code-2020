#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Instruction {
  std::string opcode;
  int parameter;
};

using Program = std::vector<Instruction>;

Instruction get_instruction_from_line(const std::string &line) {
  auto ss = std::stringstream{line};
  auto result = Instruction{};
  ss >> result.opcode >> result.parameter;
  return result;
}

Program read_program(const std::string &filename) {
  auto program = Program{};
  auto str = std::ifstream{filename};
  auto text = std::string{};

  while (std::getline(str, text)) {
    program.push_back(get_instruction_from_line(text));
  }
  return program;
}

// adjust the opcode of a program within a local context.
class AdjustOpcode {
  Program &program_;
  const int position_;
  std::string prior_opcode_;

public:
  AdjustOpcode(Program &program, int position, const std::string &opcode)
      : program_(program), position_(position),
        prior_opcode_(program[position].opcode) {
    program[position].opcode = opcode;
  }

  ~AdjustOpcode() { program_[position_].opcode = std::move(prior_opcode_); }
};