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
#include <string>
#include <vector>

namespace svm {
using code_t = uint64_t;

enum class Operator : uint8_t {
  NOP = 0x01,
  PUSH,
  POP,
  DUP,
  OPERATION,
  BRANCH,
  JMP,
  STORE,
  LOAD,
  CALL,
  RET,
  IN,
  OUT,
  HALT = 0x7f
};

enum class Func : uint8_t {
  NONE,

  ADD,
  SUB,
  MUL,
  DIV,

  AND,
  OR,
  XOR,
  NOT,

  BZ,
  BNZ,
  BGZ,
  BLZ,
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
  Instruction() = default;
  Instruction(code_t const &machineCode) : MachineCode(machineCode) {}
  Instruction(Operator const &op, int32_t const &operand = 0)
      : MachineCode((static_cast<code_t>(op) << 32) | operand) {}
  Instruction(Operator const &op, Func const &func, int32_t const &operand = 0)
      : MachineCode((static_cast<code_t>(func) << 40) |
                    (static_cast<code_t>(op) << 32) | operand) {}

  [[nodiscard]] auto op() const -> Operator {
    return static_cast<Operator>((MachineCode >> 32) & 0xff);
  }

  [[nodiscard]] auto func() const -> Func {
    return static_cast<Func>((MachineCode >> 40) & 0xff);
  }

  [[nodiscard]] auto operand() const -> int32_t {
    return static_cast<int32_t>(
        ((static_cast<int64_t>(MachineCode & 0xffffffff)) << 32) >> 32);
  }

  [[nodiscard]] auto rawCode() const -> code_t { return MachineCode; }

  operator code_t() const { return MachineCode; }

  [[nodiscard]] auto toString() const -> std::string;

  void setOp(Operator const &op) {
    MachineCode |= (static_cast<code_t>(op) << 32);
  }
  void setFunc(Func const &func) {
    MachineCode |= (static_cast<code_t>(func) << 40);
  }
  void setValue(int32_t const &value) {
    MachineCode |= static_cast<int64_t>(value) & 0xffffffff;
  }

private:
  code_t MachineCode{0};
};

using CodeList = std::vector<Instruction>;
} // namespace svm

#endif