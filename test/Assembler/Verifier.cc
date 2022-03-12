#include "Verifier.h"
#include "Assembler/Util.h"
#include "Instruction.h"
#include <fmt/color.h>
#include <fmt/core.h>

namespace svm {
auto Verifier::verify(const TestCase &test) -> bool {
  using namespace std;
  Stream.str(std::string());
  array<char, sizeof(Instruction)> Buffer{};
  Assembler.loadCode(test.AsmCode);
  Assembler.parse();
  Assembler.writeTo(Stream);
  for (auto I = 0; I < test.MachineCode.size(); ++I) {
    decltype(test.MachineCode.front().rawCode()) Raw = 0;
    Stream.read(reinterpret_cast<char *>(&Raw), sizeof(Raw));
    if (Raw != test.MachineCode[I].rawCode()) {
      fmt::print(fmt::fg(fmt::color::red),
                 "case {} not passed! : {:#016x} != {:#016x}\n", test.Name, Raw,
                 test.MachineCode[I].rawCode());
      return false;
    }
  }
  fmt::print(fmt::fg(fmt::color::green), "case {} passed!\n", test.Name);
  return true;
}
} // namespace svm