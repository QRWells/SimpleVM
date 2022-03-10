#include <fstream>
#include <iostream>
#include <sstream>

#include "magic_enum.hpp"
#include <fmt/core.h>

#include "./Programs.h"
#include "Assembler/Assembler.h"

using namespace std;

auto main(int argc, char *const argv[]) -> int {
  svm::Assembler Assembler{FACTORIAL};
  Assembler.parse();
  if (!Assembler.isParsed())
    fmt::print("{}\n", Assembler.getError());
  stringstream Result;
  Assembler.writeTo(Result);
  cout << Result.str() << endl;
  return 0;
}