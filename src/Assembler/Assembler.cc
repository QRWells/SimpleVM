/**
 * @file Assembler.cc
 * @author QRWells (qirui.wang@moegi.waseda.jp)
 * @brief
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) 2022 QRWells. All rights reserved.
 * Licensed under the MIT license.
 *
 */

#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <ios>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>

#include "Assembler/Assembler.h"
#include "Assembler/Exceptions/MultipleDefineException.h"
#include "Assembler/Util.h"
#include "Instruction.h"
#include "magic_enum.hpp"

namespace svm {
Assembler::Assembler(std::string_view code)
    : Code(std::string(code), std::ios_base::in) {}
Assembler::Assembler(std::ifstream const &code) { Code << code.rdbuf(); }

auto Assembler::isParsed() -> bool { return !Error.has_value(); }

void Assembler::parse() {
  using namespace std;
  string Line{};
  while (getline(Code, Line)) {
    ++CurrentLine;
    auto Content = preProcess(Line);

    if (!Content.has_value() || Content.value().empty())
      continue;

    auto [whole, label, inst] = svm::FILTER(Content.value());
    if (!whole)
      continue;

    auto Label = label.to_string();
    auto Inst = inst.to_string();

    if (!handleLabel(Label))
      break;

    if (Inst.empty())
      continue;

    if (!handleInstruction(Inst))
      break;

    ++InstPos;
  }
}

void Assembler::writeTo(std::ostream &stream) {
  auto const Size = sizeof(Instruction);
  stream.seekp(0);
  std::array<char, 8> Buffer{};
  for (auto const &I : InstructionList) {
    auto Pos = I.first;
    stream.seekp(static_cast<std::streamoff>(Pos * Size));
    Instruction Next{};
    auto Op = I.second.Op;
    auto Func = I.second.Func;
    Next.setOp(Op);
    Next.setFunc(Func);
    auto const *Value = std::get_if<std::string>(&I.second.Value);
    if (Value == nullptr) // Value is number
      Next.setValue(std::get<int>(I.second.Value));
    else { // Value is string(label)
      auto LabelPos = tryGetPosition(std::get<std::string>(I.second.Value));
      switch (Op) {
      case Operator::BRANCH:
        Next.setValue(LabelPos.value() - Pos);
      default:
        Next.setValue(LabelPos.value());
      }
    }
    std::to_chars(Buffer.begin(), Buffer.end(), Next.rawCode());
    stream.write(Buffer.data(), Size);
  }
}

auto Assembler::handleLabel(std::string label) -> bool {
  // set label if there is a label
  if (!label.empty()) {
    label.pop_back();
    trim(label);
    if (!trySetLabel(label, InstPos)) {
      Error = ParseError::MultipleDefinedLabel;
      return false;
    }
  }
  return true;
}

auto Assembler::handleInstruction(std::string inst) -> bool {
  using namespace std;
  auto [all_op, op, operand] = INST(inst);

  bool IsLabel{false};
  std::variant<int, std::string> Value;
  PreInst Res{};

  try {
    Value = stoi(operand.to_string());
  } catch (invalid_argument &Ex) {
    auto Pos = tryGetPosition(operand.to_string());
    if (!Pos.has_value()) {
      trySetLabel(operand.to_string(), InstPos);
      Value = operand.to_string();
      IsLabel = true;
    }
  } catch (out_of_range &Ex) {
    Error = ParseError::ImmediateoutOfRange;
    return false;
  }

  Res.Value = Value;

  auto Op = op.to_string();
  std::transform(Op.begin(), Op.end(), Op.begin(), ::toupper);
  if (magic_enum::enum_contains<Func>(Op)) {
    Res.Func = magic_enum::enum_cast<Func>(Op).value();
    if (Op[0] == 'B')
      Res.Op = Operator::BRANCH;
    else
      Res.Op = Operator::OPERATION;
  } else if (magic_enum::enum_contains<Operator>(Op)) {
    Res.Op = magic_enum::enum_cast<Operator>(Op).value();
  } else {
    Error = ParseError::UnknownInstruction;
    return false;
  }
  InstructionList.emplace(InstPos, Res);
  return true;
}

auto Assembler::tryGetPosition(std::string_view label) -> std::optional<int> {
  if (LabelMap.contains(label))
    return LabelMap.at(label);
  return std::nullopt;
}

auto Assembler::trySetLabel(std::string_view label, int const &value) -> bool {
  if (LabelMap.contains(label) && LabelMap.at(label) != LABEL_PLACEHOLDER)
    return false;
  LabelMap.emplace(label, value);
  return true;
}
} // namespace svm