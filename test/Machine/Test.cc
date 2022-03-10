#include <iostream>

#include "Machine.h"
#include "Program.h"

using namespace std;

auto main(int argc, char *const argv[]) -> int {
  auto M = Machine(1 << 7);

  M.loadExecutableCode(FACTORIAL);
  M.run();

  return 0;
}