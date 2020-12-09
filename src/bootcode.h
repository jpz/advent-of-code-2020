#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

struct Instruction {
  std::string opcode;
  int parameter;
};

Instruction get_instruction_from_line(const std::string &line) {
  auto ss = std::stringstream{line};
  auto result = Instruction{};
  ss >> result.opcode >> result.parameter;
  return result;
}

std::unordered_map<int, Instruction> read_program(const std::string &filename) {
  auto program = std::unordered_map<int, Instruction>{};
  auto str = std::ifstream{filename};
  auto text = std::string{};

  int line = 0;
  while (std::getline(str, text)) {
    program[line++] = get_instruction_from_line(text);
  }
  return program;
}
