// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Packet 2 Frame
///
/// \file   ulf/susiv2/packet2frame.hpp
/// \author Jonas Gahlert
/// \date   23/04/2026

#pragma once

#include <concepts>
#include <ranges>
#include <zusi/zusi.hpp>
#include "utility.hpp"

namespace ulf::susiv2 {

/// Packet 2 Frame 
///
/// \tparam _O  Output range type (e.g. std::vector<uint8_t>)
/// \tparam _I  Input range type (zusi::Packet)
/// \param in   Packet to format
/// \return     Frame
template<std::ranges::output_range<uint8_t> _O, std::ranges::input_range _I>
requires std::convertible_to<typename _I::value_type, uint8_t>
constexpr _O packet2frame(_I const& in) {

  // Get and check command
  auto const cmd{get_command(in)};
  if (!cmd || !*cmd) return {};

  // For now, no response > 4 byte is sent
  _O result{0u, 0u, 0u};

  // Assing Expected response size depending on command
  switch (**cmd) {
    case zusi::Command::CvRead: result.push_back(2u); break;
    case zusi::Command::Features: result.push_back(4u); break; 
    case zusi::Command::CvWrite: [[fallthrough]];
    case zusi::Command::ZppErase: [[fallthrough]];
    case zusi::Command::ZppWrite: [[fallthrough]];
    case zusi::Command::ZppLcDcQuery: [[fallthrough]];
    case zusi::Command::Exit: result.push_back(0u); break; 
      default: result.push_back(0u); break; 
  }

  // AFAIK all commands include a (short) busy phase
  result.push_back(1u); 

  // Copy packet
  std::ranges::copy(in, std::back_inserter(result));
  return result;
}

} // namespace ulf::susiv2
