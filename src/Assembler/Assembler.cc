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

#include <cstdint>
#include <cstdlib>
#include <ios>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include <fmt/core.h>
#include <magic_enum.hpp>

#include "Assembler/Assembler.h"
#include "Assembler/Exceptions/MultipleDefineException.h"
#include "Assembler/Util.h"
#include "Instruction.h"

namespace svm {
Assembler::Assembler(std::string const &code) : Code(code, std::ios_base::in) {}
Assembler::Assembler(std::ifstream const &code) { Code << code.rdbuf(); }
void Assembler::loadCode(std::string const &code) {
  Code = std::stringstream{code, std::ios_base::in};
}
void Assembler::loadCode(std::ifstream const &code) { Code << code.rdbuf(); }

auto Assembler::isParsable() -> bool { return Code.rdbuf()->in_avail() != 0; }
auto Assembler::isParsed() -> bool {
  return !Error.has_value() || Error.value() == ParseError::None;
}

auto Assembler::getError() const -> std::string {
  return fmt::format(
      "{} at line {}",
      magic_enum::enum_name<svm::ParseError>(Error.value_or(ParseError::None)),
      CurrentLine);
}

void Assembler::parse() {
  using namespace std;
  if (!isParsable())
    return;
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
      auto LabelPos =
          tryGetPosition(std::get<std::string>(I.second.Value)).value();
      switch (Op) {
      case Operator::BRANCH:
        Next.setValue(LabelPos - Pos);
        break;
      default:
        Next.setValue(LabelPos);
      }
    }
    auto Raw = Next.rawCode();
    stream.write(reinterpret_cast<char *>(&Raw), sizeof(Raw));
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

  std::variant<int, std::string> Value;
  PreInst Res{};

  auto Temp = operand.to_string();
  trim(Temp);
  if (!Temp.empty())
    try {
      Value = stoi(Temp);
    } catch (std::invalid_argument &Ex) {
      auto Pos = tryGetPosition(Temp);
      if (!Pos.has_value())
        trySetLabel(Temp, LABEL_PLACEHOLDER);
      Value = Temp;
    } catch (std::out_of_range &Ex) {
      Error = ParseError::ImmediateoutOfRange;
      return false;
    }
  else
    Value = 0;

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

auto Assembler::tryGetPosition(std::string const &label) -> std::optional<int> {
  if (LabelMap.contains(label))
    return LabelMap.at(label);
  return std::nullopt;
}

auto Assembler::trySetLabel(std::string const &label, int const &value)
    -> bool {
  if (LabelMap.contains(label)) {
    if (LabelMap.at(label) == LABEL_PLACEHOLDER)
      LabelMap.at(label) = value;
    else
      return false;
  }
  LabelMap.insert({label, value});
  return true;
}
} // namespace svm