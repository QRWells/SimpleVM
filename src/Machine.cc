/**
 * @file Machine.c
 * @author QRWells (qirui.wang@moegi.waseda.jp)
 * @brief
 * @version 0.1
 * @date 2022-03-06
 *
 * @copyright Copyright (c) 2022 QRWells. All rights reserved.
 * Licensed under the MIT license.
 *
 */

#include "Machine.h"
#include "Frame.h"
#include "Instruction.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <fmt/core.h>

namespace svm {
Machine::Machine(uint32_t memorySize)
    : MemorySize(memorySize), Memory(memorySize) {}
Machine::~Machine() = default;

void Machine::run() {
  auto Root = Frame(UINT32_MAX);
  Stack.emplace(Root);
  while (!Halted) {
#ifdef DEBUG
    fmt::print("{}: {:#x}\n", Pc, CurrentInst.rawCode());
    if (!TempVars.empty())
      fmt::print("top: {}\n", TempVars.top());
#endif
    fetch();
    execute();
  }
  Stack.pop();
}

auto Machine::loadExecutableCode(std::vector<Instruction> &code) -> bool {
  if (code.size() > MemorySize)
    return false;
  std::copy(code.cbegin(), code.cend(), Memory.begin());
  return true;
}

void Machine::fetch() {
  if (Pc > MemorySize or !executable(Memory[Pc]))
    CurrentInst = Instruction(Operator::HALT, 0);
  CurrentInst = Memory[Pc];
  ++Pc;
}

void Machine::execute() {
  // Decode
  auto const Op = CurrentInst.op();
  auto const Func = CurrentInst.func();
  auto const Operand = CurrentInst.operand();

  // Execute
  switch (Op) {
  case Operator::NOP:
    break;
  case Operator::PUSH:
    push(Operand);
    break;
  case Operator::POP:
    pop();
    break;
  case Operator::DUP:
    dup();
    break;
  case Operator::CALL:
    call(Operand);
    break;
  case Operator::RET:
    ret();
    break;
  case Operator::OPERATION:
    handleOperation(Func, Operand);
    break;
  case Operator::BRANCH:
    handleBranch(Func, Operand);
    break;
  case Operator::JMP: {
    auto To = Operand;
    if (!checkBound(To) or To == Pc - 1)
      Halted = true;
    else
      Pc = To;
    break;
  }
  case Operator::IN: {
    int32_t V = 0;
    *In >> V;
    if (In->fail())
      Halted = true;
    else
      push(V);
    In->clear();
    break;
  }
  case Operator::OUT: {
    auto V = TempVars.top();
    *Out << V << std::endl;
    break;
  }
  // TODO: Store & Load
  case Operator::STORE:
  case Operator::LOAD:
  case Operator::HALT:
  default:
    Halted = true;
  }
}

auto Machine::executable(uint32_t const &addr) -> bool {
  // TODO: Need more detail
  return false;
}

auto Machine::checkBound(int32_t const &addr) const -> bool {
  return addr <= MemorySize && addr > 0;
}

void Machine::push(int32_t const &value) { TempVars.push(value); }

auto Machine::pop() -> int32_t {
  if (TempVars.empty()) {
    Halted = true;
    return -1;
  }
  auto V = TempVars.top();
  TempVars.pop();
  return V;
}

void Machine::dup() {
  if (TempVars.empty()) {
    Halted = true;
    return;
  }
  TempVars.emplace(TempVars.top());
}

void Machine::call(uint32_t const &func) {
  if (func > MemorySize) {
    Halted = true;
    return;
  }
  auto NewFrame = Frame(Pc);
  Stack.push(NewFrame);
  Pc = func;
}

void Machine::ret() {
  Pc = Stack.top().getReturnAddress();
  if (Pc == UINT32_MAX) {
    Halted = true;
    return;
  }
  Stack.pop();
}

void Machine::handleBranch(const Func &func, const int32_t &value) {
  auto Condition = pop();
  bool B = false;
  switch (func) {
  case Func::Z:
    B = Condition == 0;
    break;
  case Func::NZ:
    B = Condition != 0;
    break;
  case Func::GZ:
    B = Condition > 0;
    break;
  case Func::GEZ:
    B = Condition >= 0;
    break;
  case Func::LZ:
    B = Condition < 0;
    break;
  case Func::LEZ:
    B = Condition <= 0;
    break;
  default:
    Halted = true;
  }
  if (B && checkBound(static_cast<int32_t>(Pc) + value))
    Pc = static_cast<uint32_t>(static_cast<int32_t>(Pc) + value - 1);
}

void Machine::handleOperation(const Func &func, const int32_t &value) {
  auto B = pop();
  if (func == Func::NOT) {
    B = B == 0 ? 1 : 0;
    push(B);
    return;
  }
  auto A = pop();
  decltype(B) C = 0;
  switch (func) {
  case Func::ADD:
    C = A + B;
    break;
  case Func::SUB:
    C = A - B;
    break;
  case Func::MUL:
    C = A * B;
    break;
  case Func::DIV:
    C = A / B;
    break;
  case Func::AND:
    C = A & B;
    break;
  case Func::OR:
    C = A | B;
    break;
  case Func::XOR:
    C = A ^ B;
    break;
  default:
    Halted = true;
  }
  push(C);
}

} // namespace svm