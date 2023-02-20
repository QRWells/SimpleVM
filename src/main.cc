/**
 * @file main.cc
 * @author QRWells (qirui.wang@moegi.waseda.jp)
 * @brief Program for executing assembled code("machine code").
 * @version 0.1
 * @date 2022-03-06
 *
 * @copyright Copyright (c) 2022 QRWells. All rights reserved.
 * Licensed under the MIT license.
 *
 */

#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <span>
#include <string>
#include <vector>

#include <fmt/color.h>

#include "Instruction.h"
#include "Machine.h"

using namespace std;
using namespace svm;

auto main(int argc, char *argv[]) -> int {

  auto Args = span{argv, static_cast<size_t>(argc)};
  if (argc == 1)
    fmt::print(stderr, fmt::fg(fmt::color::red), "Please specify a program to execute!", Args[1]);

  ifstream Prog{Args[1], ios_base::in | ios_base::binary};

  if (!Prog.is_open()) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "Unable to open file : {}", Args[1]);
    return -1;
  }

  size_t MemSize = 1 << 7;

  if (argc > 2) {
    try {
      MemSize = stoul(Args[2]);
    } catch (exception &E) {
      fmt::print(stderr, fmt::fg(fmt::color::red), "Please input a valid number");
      Prog.close();
      return -1;
    }
  }

  Prog.ignore(std::numeric_limits<std::streamsize>::max());
  auto Length = Prog.gcount();
  Prog.clear();
  Prog.seekg(0, std::ios_base::beg);

  if (Length % 8 != 0) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "Unable to resolve the program : {}", Args[1]);
    return -1;
  }

  code_t         Buffer{};
  constexpr auto CodeSize = sizeof(Buffer);

  auto     M              = Machine(MemSize);
  CodeList List{};

  for (auto I = 0; I < Length / CodeSize; ++I) {
    Prog.read(reinterpret_cast<char *>(&Buffer), CodeSize);
    List.emplace_back(Buffer);
  }

  if (!M.loadExecutableCode(List)) {
    fmt::print(stderr, fmt::fg(fmt::color::red), "There is no space to load the program");
    return -1;
  }

  M.run();

  return 0;
}