// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Convert ZUSI feedback to response
///
/// \file   ulf/susiv2/feedback2response.hpp
/// \author Jonas Gahlert
/// \date   05/12/2024

#pragma once

#include <optional>
#include <ztl/inplace_vector.hpp>
#include <zusi/zusi.hpp>
#include "ack.hpp"
#include "nak.hpp"
#include "response.hpp"

namespace ulf::susiv2 {

/// Convert ZUSI feedback to response
///
/// \param  fb  ZUSI feedback
/// \return Response
constexpr Response feedback2response(zusi::Feedback fb) {
  if (!fb) return {nak};
  Response resp{ack};
  if (size(*fb)) {
    std::ranges::copy(*fb, std::back_inserter(resp));
    resp.push_back(zusi::crc8(*fb));
  }
  return resp;
}

} // namespace ulf::susiv2
