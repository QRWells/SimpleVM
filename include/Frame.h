/**
 * @file Frame.h
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
#ifndef SVM_FRAME
#define SVM_FRAME

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace svm {
class Frame {
public:
  Frame(uint32_t returnAddress) : ReturnAddress(returnAddress) {}

  void setVariable(int32_t const &name, int32_t const &value);
  auto getVariable(int32_t const &name) -> int32_t;
  auto getReturnAddress() const -> uint32_t;

private:
  uint32_t ReturnAddress;
  std::unordered_map<int32_t, int32_t> Variables;
};
} // namespace svm

#endif