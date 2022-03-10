/**
 * @file machine.h
 * @author QRWells (qirui.wang@moegi.waseda.jp)
 * @brief
 * @version 0.1
 * @date 2022-03-06
 *
 * @copyright Copyright (c) 2022 QRWells. All rights reserved.
 * Licensed under the MIT license.
 *
 */

#pragma once
#ifndef SVM_MACHINE
#define SVM_MACHINE

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stack>
#include <vector>

#include "Frame.h"
#include "Instruction.h"

namespace svm {
class Machine {
public:
  // Default heap memory 4KB (8B per unit)
  explicit Machine(uint32_t memorySize = 1 << 9);

  ~Machine();

  // Run the machine
  void run();

  auto loadExecutableCode(std::vector<Instruction> const &code) -> bool;

private:
  // Indicate if the machine has halted
  bool Halted{false};

  // Program counter
  uint32_t Pc{0};

  // Instruction
  Instruction CurrentInst;

  // Stack frames
  std::stack<Frame> Stack{};

  // Memory size for out of bound check
  uint32_t MemorySize;

  // Memory for program
  std::vector<uint64_t> Memory;

  // Stack for temp variables
  std::stack<int32_t> TempVars;

  // In/Out file for data input and output
  std::istream *In{&std::cin};
  std::ostream *Out{&std::cout};

  void fetch();
  void execute();

  [[nodiscard]] auto checkBound(int32_t const &addr) const -> bool;
  void handleOperation(Func const &func, int32_t const &value);
  void handleBranch(Func const &func, int32_t const &value);

  auto static executable(uint32_t const &addr) -> bool;

  void push(int32_t const &value);
  auto pop() -> int32_t;
  void dup();
  void call(uint32_t const &func);
  void ret();
};
} // namespace svm

#endif