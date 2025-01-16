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
/// \param  re_frame  Optional decoder response data
/// \return Formatted answer ready to send
constexpr ztl::inplace_vector<uint8_t, 6uz>
answer_zsp(std::optional<ztl::inplace_vector<uint8_t, 4uz>> re_frame) {
  if (!re_frame) return {nak};
  ztl::inplace_vector<uint8_t, 6uz> retval{ack};
  std::ranges::copy(*re_frame, begin(retval) + 1);
  retval.push_back(zusi::crc8(*re_frame));
  return retval;
}

} // namespace ulf::susiv2
