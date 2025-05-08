// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Convert frame to ZUSI packet
///
/// \file   ulf/susiv2/frame2packet.hpp
/// \author Jonas Gahlert
/// \date   03/12/2024

#pragma once

#include <expected>
#include <optional>
#include <system_error>
#include "validate.hpp"

namespace ulf::susiv2 {

/// Convert frame to ZUSI packet without validation
///
/// \param[in,out]  frame SUSIV2 frame to be converted, will contain the ZUSI
///                       frame on return
constexpr std::span<uint8_t const>
frame2packet_no_validate(std::span<uint8_t const>& frame) {
  return frame.subspan(5uz);
}

/// Convert frame to ZUSI packet
///
/// \param[in,out]  frame         SUSIV2 frame to be converted, will contain the
///                               ZUSI frame on successful verification
/// \retval         std::span     View on Packet
/// \retval         std::nullopt  Frame incomplete
/// \retval         std::errc     Frame corrupt
constexpr std::expected<std::optional<std::span<uint8_t const>>, std::errc>
frame2packet(std::span<uint8_t const> frame) {
  if (size(frame) < 7uz) return std::nullopt;

  auto result{frame.subspan(5uz)};
  auto const cmd{get_command(result)};
  if (!cmd) return std::unexpected{cmd.error()};
  if (!*cmd) return std::nullopt;

  switch (std::bit_cast<zusi::Command>(**cmd)) {
    case zusi::Command::CvRead:
      if (size(result) >= cvread_size)
        result = result.subspan(0uz, cvread_size);
      else return std::nullopt;
      break;
    case zusi::Command::CvWrite:
      if (size(result) >= cvwrite_size(result[zusi::data_cnt_pos]))
        result = result.subspan(0uz, cvwrite_size(result[zusi::data_cnt_pos]));
      else return std::nullopt;
      break;
    case zusi::Command::ZppWrite:
      if (size(result) >= zppwrite_size(result[zusi::data_cnt_pos]))
        result = result.subspan(0uz, zppwrite_size(result[zusi::data_cnt_pos]));
      else return std::nullopt;
      break;
    case zusi::Command::ZppErase:
      if (size(result) >= zpperase_size)
        result = result.subspan(0uz, zpperase_size);
      else return std::nullopt;
      break;
    case zusi::Command::Features:
      if (size(result) >= features_size)
        result = result.subspan(0uz, features_size);
      else return std::nullopt;
      break;
    case zusi::Command::Exit:
      if (size(result) >= exit_size) result = result.subspan(0uz, exit_size);
      else return std::nullopt;
      break;
    default: break;
  }

  // Could be command?
  auto ret = validate(result);

  if (!ret) return std::unexpected(ret.error());
  if (!*ret) return std::nullopt;
  return result;
}

} // namespace ulf::susiv2