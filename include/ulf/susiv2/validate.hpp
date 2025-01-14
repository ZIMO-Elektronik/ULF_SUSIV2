// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

///
///
/// \file   ulf/susiv2/validate.hpp
/// \author Jonas Gahlert
/// \date   03/12/2024

#pragma once

#include <system_error>
#include "utility.hpp"

namespace ulf::susiv2 {

/// Validate ZUSI frame
///
/// \param  frame         Frame to validate
/// \retval true          Frame is valid and ready to use
/// \retval std::nullopt  Frame is incomplete
/// \retval std::errc     Frame data is corrupt and unusable
inline std::expected<std::optional<bool>, std::errc> const
validate(std::span<uint8_t const> const& frame) {
  auto const cmd{get_command(frame)};
  auto const crc{get_checksum(frame)};

  if (cmd && crc) {
    if (*cmd && *crc) {
      if (!zusi::crc8(frame)) return true;
      else return std::unexpected{std::errc::protocol_error};
    }
    // Incomplete
    else
      return std::nullopt;
  }
  // ValidationError
  else
    return std::unexpected{cmd ? cmd.error() : crc.error()};
}
} // namespace ulf::susiv2