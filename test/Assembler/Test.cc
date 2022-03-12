#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include <fmt/core.h>
#include <magic_enum.hpp>

#include "./Programs.h"
#include "Assembler/Assembler.h"
#include "Verifier.h"

using namespace std;

auto main(int argc, char *const argv[]) -> int {
  svm::Verifier Verifier;
  Verifier.verify(svm::test::FAC);
  return 0;
}