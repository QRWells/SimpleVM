/**
 * @file Util.h
 * @author QRWells (qirui.wang@moegi.waseda.jp)
 * @brief
 * @version 0.1
 * @date 2022-03-08
 *
 * @copyright Copyright (c) 2022 QRWells. All rights reserved.
 * Licensed under the MIT license.
 *
 */

#pragma once
#ifndef SVM_AS_UTIL
#define SVM_AS_UTIL

#include <array>
#include <bit>
#include <cctype>
#include <cstddef>
#include <locale>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include <ctre.hpp>
#include <fmt/core.h>

#include "Instruction.h"

namespace svm {

auto static constexpr FILTER = ctre::match<R"(^([^\d]\w+\s*:)?\s*([^/;#\n\r]*)?$)">;
auto static constexpr INST   = ctre::match<R"(^([a-zA-Z]{2,5})(\s+\S+)?\s*$)">;

inline auto extractFileName(std::string_view filename) -> std::string {
  auto Dot = filename.find_last_of('.');
  if (Dot == std::string_view::npos)
    return {filename.begin(), filename.end()};
  auto Name = filename.substr(0, Dot);
  return {Name.begin(), Name.end()};
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return std::isspace(ch) == 0; }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return std::isspace(ch) == 0; }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

inline auto preProcess(std::string_view line) -> std::optional<std::string> {
  std::string Line{line.begin(), line.end()};
  trim(Line);
  if (Line.empty())
    return std::nullopt;
  if (auto Pos = Line.find_first_of(";#/"); Pos != std::string::npos)
    return Line.substr(0, Pos);
  return Line;
}

template <typename T, size_t size>
  requires std::is_arithmetic_v<T> && (size >= sizeof(T))
void splitValueToBytes(std::array<char, size> &des, T const &value) {
  if constexpr (std::endian::native == std::endian::big)
    for (auto I = 0; I < sizeof(value); ++I)
      des.data()[I] = value >> (I * 8);

  else if constexpr (std::endian::native == std::endian::little)
    for (size_t I = 0; I < sizeof(value); ++I)
      des.data()[I] = value >> ((sizeof(T) - 1) * 8 - I * 8);
}

template <typename T, size_t size>
  requires std::is_arithmetic_v<T> && (sizeof(T) >= size)
                                    auto mergeBytesToValue(std::array<char, size> const &src) -> T {
  T Value{0};
  if constexpr (std::endian::native == std::endian::big)
    for (auto I = 0; I < size; ++I)
      Value |= (src.data()[I] << (I * 8));

  else if constexpr (std::endian::native == std::endian::little)
    for (size_t I = 0; I < size; ++I)
      Value |= (src.data()[I] << ((sizeof(T) - 1) * 8 - I * 8));

  return Value;
}
} // namespace svm

#endif