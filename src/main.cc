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

#include <iostream>
#include <vector>

#include "Instruction.h"
#include "Machine.h"

using namespace std;
using namespace svm;

auto main(int argc, char *argv[]) -> int {

  /*
  fac:
      dup
      push 1
      sub
      dup
      bnz  L1
      pop
      ret
  L1:
      call fac
      mut
      ret
  main:
      push 5
      call fac
      out
  */
  vector<svm::Instruction> Code = {
      {Operator::JMP, 11},
      // Fac
      {Operator::DUP},
      {Operator::PUSH, 1},
      {Operator::OPERATION, Func::SUB},
      {Operator::DUP},
      {Operator::BRANCH, Func::NZ, 3},
      {Operator::POP},
      {Operator::RET},
      {Operator::CALL, 1},
      {Operator::OPERATION, Func::MUL},
      {Operator::RET},
      // Main
      {Operator::PUSH, 5},
      {Operator::CALL, 1},
      {Operator::OUT},
      {Operator::HALT},
  };

  auto M = Machine(1 << 7);

  M.loadExecutableCode(Code);
  M.run();

  return 0;
}