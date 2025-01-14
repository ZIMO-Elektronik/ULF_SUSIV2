// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Convert SUSIV2 into ZUSI frames
///
/// \file   ulf/susiv2/format.hpp
/// \author Jonas Gahlert
/// \date   03/12/2024

#pragma once

#include <expected>
#include <optional>
#include <system_error>
#include "validate.hpp"

namespace ulf::susiv2 {

/// Formats a SUSIV2 frame into a ZUSI frame, but performs no integrity checks
///
/// \param[in,out]  frame  SUSIV2 frame to be converted, will contain the ZUSI
///                       frame on return
inline void format_no_validate(std::span<uint8_t const>& frame) {
  frame = frame.subspan(5uz);
}

/// Formats a SUSIV2 frame into a ZUSI frame, performs checks to verify
/// integrity of the frame
///
/// \param[in,out]  frame         SUSIV2 frame to be converted, will contain the
///                               ZUSI frame on successful verification
/// \retval         true          Operation successful
/// \retval         std::nullopt  Frame incomplete
/// \retval         std::errc     Frame corrupt
inline std::expected<std::optional<bool>, std::errc>
format(std::span<uint8_t const>& frame) {
  if (size(frame) < 7uz) return std::nullopt;

  auto tmp{frame.subspan(5uz)};
  auto const cmd{get_command(tmp)};
  if (!cmd) return std::unexpected{cmd.error()};
  if (!*cmd) return std::nullopt;

  switch (std::bit_cast<zusi::Command>(**cmd)) {
    case zusi::Command::CvRead:
      if (size(tmp) >= cvread_size) tmp = tmp.subspan(0uz, cvread_size);
      else return std::nullopt;
      break;
    case zusi::Command::CvWrite:
      if (size(tmp) >= cvwrite_size(tmp[zusi::data_cnt_pos]))
        tmp = tmp.subspan(0uz, cvwrite_size(tmp[zusi::data_cnt_pos]));
      else return std::nullopt;
      break;
    case zusi::Command::ZppWrite:
      if (size(tmp) >= zppwrite_size(tmp[zusi::data_cnt_pos]))
        tmp = tmp.subspan(0uz, zppwrite_size(tmp[zusi::data_cnt_pos]));
      else return std::nullopt;
      break;
    case zusi::Command::ZppErase:
      if (size(tmp) >= zpperase_size) tmp = tmp.subspan(0uz, zpperase_size);
      else return std::nullopt;
      break;
    case zusi::Command::Features:
      if (size(tmp) >= features_size) tmp = tmp.subspan(0uz, features_size);
      else return std::nullopt;
      break;
    case zusi::Command::Exit:
      if (size(tmp) >= exit_size) tmp = tmp.subspan(0uz, exit_size);
      else return std::nullopt;
      break;
    default: break;
  }

  // Could be command?
  auto ret = validate(tmp);
  if (ret && *ret) frame = tmp;

  return ret;
}

} // namespace ulf::susiv2