// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Not acknowledge
///
/// \file   ulf/susiv2/nak.hpp
/// \author Vincent Hamp
/// \date   18/08/2024

#pragma once

#include <cstdint>

namespace ulf::susiv2 {

inline constexpr uint8_t nak{0x15u};

} // namespace ulf::susiv2