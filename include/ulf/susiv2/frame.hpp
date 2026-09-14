// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Packet
///
/// \file   susiv2/frame.hpp
/// \author Jonas Gahlert
/// \date   14/09/2026

#pragma once

#include <zusi/packet.hpp>

namespace ulf::susiv2 {

using Frame = ztl::inplace_vector<uint8_t, 5uz + ZUSI_MAX_PACKET_SIZE>;

} // namespace ulf::susiv2
