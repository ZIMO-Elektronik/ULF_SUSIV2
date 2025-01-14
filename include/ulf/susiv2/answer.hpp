// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Contains formatters for answering the ZSP (Zimo Sound Programmer)
///
/// \file   ulf/susiv2/answer.hpp
/// \author Jonas Gahlert
/// \date   05/12/2024

#pragma once

#include <optional>
#include <ztl/inplace_vector.hpp>
#include <zusi/crc8.hpp>
#include "ack.hpp"
#include "nak.hpp"

namespace ulf::susiv2 {

/// Formatter for an answer destined to ZSP
///
/// \param  response  ack or nak
/// \param  re_frame  optional decoder response data
/// \return Formatted answer ready to send
inline auto answer_zsp(
  bool response,
  std::optional<ztl::inplace_vector<uint8_t, 4uz>> re_frame = std::nullopt) {
  ztl::inplace_vector<uint8_t, 6uz> ret;
  ret.push_back(response ? ack : nak);
  if (re_frame) {
    std::ranges::copy(*re_frame, std::back_inserter(ret));
    ret.push_back(zusi::crc8(*re_frame));
  }
  return ret;
}

} // namespace ulf::susiv2
