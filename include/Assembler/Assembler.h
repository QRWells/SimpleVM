/**
 * @file Assembler.h
 * @author QRWells (qirui.wang@moegi.waseda.jp)
 * @brief
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) 2022 QRWells. All rights reserved.
 * Licensed under the MIT license.
 *
 */

#pragma once
#include "Assembler/Exceptions/MultipleDefineException.h"
#include <ostream>
#ifndef SVM_AS_ASSEMBLER
#define SVM_AS_ASSEMBLER

#include <cstdio>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <variant>

#include "Instruction.h"

namespace svm {
enum class ParseError {
  None,
  MultipleDefinedLabel,
  ImmediateoutOfRange,
  UnknownInstruction
};

struct PreInst {
  Operator Op;
  Func Func{Func::NONE};
  std::variant<int, std::string> Value;
};

class Assembler {
public:
  Assembler() = default;
  Assembler(std::string const &code);
  Assembler(std::ifstream const &code);

  void loadCode(std::string const &code);
  void loadCode(std::ifstream const &code);
  void parse();
  void writeTo(std::ostream &stream);
  auto isParsable() -> bool;
  auto isParsed() -> bool;
  auto getError() const -> std::string;

private:
  int static constexpr LABEL_PLACEHOLDER = -1;

  int CurrentLine{0};
  int InstPos{0};
  std::optional<ParseError> Error{std::nullopt};
  std::stringstream Code;
  std::unordered_map<std::string, int> LabelMap;
  std::map<int, PreInst> InstructionList;

  auto tryGetPosition(std::string const &label) -> std::optional<int>;
  auto trySetLabel(std::string const &label, int const &value) -> bool;

  void fetchChar();

  auto handleLabel(std::string label) -> bool;
  auto handleInstruction(std::string inst) -> bool;
};
} // namespace svm

#endif