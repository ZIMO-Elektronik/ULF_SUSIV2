// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Convert ZUSI packet to SUSIV2 frame
///
/// \file   ulf/susiv2/packet2frame.hpp
/// \author Jonas Gahlert
/// \date   23/04/2026

#pragma once

#include <concepts>
#include <ranges>
#include <zusi/zusi.hpp>
#include "frame.hpp"
#include "utility.hpp"

namespace ulf::susiv2 {

/// Convert packet to frame
///
/// \warning    Naturally, the length of packet can't exceed
///             `ZUSI_MAX_PACKET_LENGTH`
///
/// \param      Packet to format
/// \return     Frame
constexpr Frame packet2frame(std::span<uint8_t const> const packet) {
  // Get and check command
  auto const cmd{get_command(packet)};
  if (!cmd || !*cmd) return {};

  // Map command to response size
  uint8_t const response_size{
    static_cast<uint8_t>(**cmd == zusi::Command::CvRead     ? 2u
                         : **cmd == zusi::Command::Features ? 4u
                                                            : 0u)};

  // Construct frame (AFAIK all commands currently have a busy phase)
  Frame frame{0u, 0u, 0u, response_size, 1u};

  // Copy packet
  std::ranges::copy(packet, end(frame));
  frame.resize(static_cast<Frame::size_type>(size(packet) + 5uz));
  return frame;
}

} // namespace ulf::susiv2
