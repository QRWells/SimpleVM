#include <string>

#include <fmt/core.h>
#include <magic_enum.hpp>

#include "Instruction.h"

namespace svm {
auto Instruction::toString() const -> std::string {
  auto Op   = magic_enum::enum_name(op());
  auto Func = magic_enum::enum_name(func());
  if (op() == Operator::OPERATION or op() == Operator::BRANCH)
    return fmt::format("{: <5} {}", Func, operand());
  return fmt::format("{: <5} {}", Op, operand());
}
} // namespace svm