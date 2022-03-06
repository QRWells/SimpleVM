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
#include "Instruction.h"
#ifndef SVM_MACHINE
#define SVM_MACHINE

#include "Frame.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <stack>
#include <vector>

namespace svm {
class Machine {
public:
  // Default heap memory 4KB (8B per unit)
  explicit Machine(uint32_t memorySize = 1 << 9);

  ~Machine();

  // Run the machine
  void run();

  void loadExcutableCode(std::vector<Instruction> const &code);

private:
  // Indicate if the machine has halted
  bool Halted{false};

  // Program counter
  uint32_t Pc{0};

  // Stack frames
  std::stack<Frame> Stack{};

  // Memory size for out of bound check
  uint32_t MemorySize;

  // Memory for program
  std::vector<uint64_t> Memory;

  // Stack for temp variables
  std::stack<uint64_t> TempVars;

  void fetch();
  void decode();
  void execute();

  auto executable(uint32_t const &addr) -> bool;

  void push(uint32_t const &value);
  auto pop() -> uint32_t;
  void call();
  void ret();
};
} // namespace svm

#endif