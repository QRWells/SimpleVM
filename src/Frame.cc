#include "Frame.h"
#include <cstdint>
#include <string>

namespace svm {
auto Frame::getReturnAddress() const -> uint32_t { return ReturnAddress; }
void Frame::setVariable(int32_t const &name, int32_t const &value) {
  Variables.emplace(name, value);
}
auto Frame::getVariable(int32_t const &name) -> int32_t {
  if (Variables.contains(name)) [[likely]]
    return Variables.at(name);
  return 0;
}
} // namespace svm