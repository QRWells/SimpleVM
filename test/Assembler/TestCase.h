/**
 * @file TestCase.h
 * @author QRWells (qirui.wang@moegi.waseda.jp)
 * @brief
 * @version 0.1
 * @date 2022-03-11
 *
 * @copyright Copyright (c) 2022 QRWells. All rights reserved.
 * Licensed under the MIT license.
 *
 */

#pragma once
#ifndef TEST_AS_TESTCASE
#define TEST_AS_TESTCASE

#include <string>
#include <vector>

#include "Instruction.h"

namespace svm {
struct TestCase {
public:
  std::string Name;
  std::string AsmCode;
  std::vector<Instruction> MachineCode;
};
} // namespace svm
#endif