/**
 * @file Verifier.h
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
#ifndef TEST_AS_VERIFIER
#define TEST_AS_VERIFIER

#include <sstream>

#include "Assembler/Assembler.h"
#include "TestCase.h"

namespace svm {
class Verifier {
public:
  auto verify(TestCase const &test) -> bool;

private:
  std::stringstream Stream;
  Assembler Assembler;
};
} // namespace svm
#endif