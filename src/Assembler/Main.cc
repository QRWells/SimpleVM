/**
 * @file Main.cc
 * @author QRWells (qirui.wang@moegi.waseda.jp)
 * @brief
 * @version 0.1
 * @date 2022-03-08
 *
 * @copyright Copyright (c) 2022 QRWells. All rights reserved.
 * Licensed under the MIT license.
 *
 */

#include <fstream>
#include <ios>
#include <iostream>
#include <optional>
#include <span>

#include <fmt/color.h>
#include <fmt/core.h>
#include <string>

#include "Assembler/Assembler.h"
#include "Assembler/Util.h"

using namespace std;
using namespace fmt;

auto main(int argc, char *const argv[]) -> int {
  auto Args = span{argv, static_cast<size_t>(argc)};

  if (argc < 2)
    print(stderr, "Please specify the file to aseemble");

  ifstream As{Args[1], ios_base::in};

  if (!As.is_open())
    print(stderr, "Unable to open file: {}", Args[1]);

  string Line;

  svm::Assembler Assembler{As};
  Assembler.parse();
  if (!Assembler.isParsed())
    fmt::print(fmt::fg(fmt::color::red), "Error :{}\n", Assembler.getError());

  ofstream Code{svm::extractFileName(Args[1]) + ".bin", ios_base::out | ios_base::binary};

  Assembler.writeTo(Code);
  return 0;
}