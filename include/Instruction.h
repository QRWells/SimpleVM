/**
 * @file instruction.h
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
#ifndef SVM_INST
#define SVM_INST

#include <cstdint>

namespace svm {
enum class Operator : uint8_t {
  NOP = 0x00,
  PUSH,
  POP,
  OPERATION,
  BRANCH,
  JMP,
  STORE,
  LOAD,
  CALL,
  RET,
  HALT = 0xff
};

enum class Func : uint8_t {
  ADD = 0,
  SUB,
  MUL,
  DIV,

  AND,
  OR,
  XOR,
  NOT,

  EQ = 0,
  NEQ,
  GT,
  LT,
};

/**
 *
 *
 *
 *
 *
 */
struct Instruction {
public:
  Instruction(uint64_t machineCode) : MachineCode(machineCode) {}
  Instruction(Operator op, int32_t operand)
      : MachineCode((static_cast<uint64_t>(op) << 56) | operand) {}

  [[nodiscard]] auto op() const -> Operator {
    return static_cast<Operator>((MachineCode >> 32) & 0xff);
  }

  [[nodiscard]] auto func() const -> Func {
    return static_cast<Func>((MachineCode >> 40) & 0xff);
  }

  [[nodiscard]] auto operand() const -> int32_t {
    return static_cast<int32_t>(MachineCode & 0xffffffff);
  }

private:
  uint64_t MachineCode;
};
} // namespace svm

#endif