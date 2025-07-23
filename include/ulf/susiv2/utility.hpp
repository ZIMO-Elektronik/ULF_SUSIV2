// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Utility functions
///
/// \file   ulf/susiv2/utility.hpp
/// \author Jonas Gahlert
/// \date   03/12/2024

#pragma once

#include <expected>
#include <optional>
#include <span>
#include <system_error>
#include <ztl/inplace_vector.hpp>
#include <zusi/command.hpp>
#include <zusi/utility.hpp>

namespace ulf::susiv2 {

inline constexpr size_t cvread_size{7uz};
inline constexpr size_t cvwrite_size(uint8_t cnt) { return 7uz + cnt + 1uz; }

inline constexpr size_t zpperase_size{4uz};
inline constexpr size_t zppwrite_size(uint8_t cnt) { return 7uz + cnt + 1uz; }
inline constexpr size_t zpplcdcquery_size{6uz};

inline constexpr size_t features_size{2uz};
inline constexpr size_t exit_size{5uz};

/// Helper to get a command byte from a ZUSI frame
///
/// \param  frame         ZUSI frame to search
/// \retval zusi::Command Command was found
/// \retval std::nullopt  ZUSI frame is too short to contain a command byte
/// \retval std::errc     Unknown command
constexpr std::expected<std::optional<zusi::Command>, std::errc>
get_command(std::span<uint8_t const> frame) {
  if (size(frame) > zusi::cmd_pos) {
    if (zusi::is_valid_command(frame[zusi::cmd_pos]))
      return static_cast<zusi::Command>(frame[zusi::cmd_pos]);
    else return std::unexpected{std::errc::protocol_error};
  }
  return std::nullopt;
}

/// Helper to get a CV or Data count from a ZUSI frame
///
/// \param  frame         ZUSI frame to search
/// \retval uint8_t       Count that was found
/// \retval std::nullopt  Frame is too short to contain a count byte
/// \retval std::errc     Frame cannot contain a count (e.g. FlashDelete)
constexpr std::expected<std::optional<uint8_t>, std::errc>
get_count(std::span<uint8_t const> frame) {
  if (size(frame) > zusi::data_cnt_pos)
    switch (static_cast<zusi::Command>(frame[zusi::cmd_pos])) {
      case zusi::Command::CvRead: [[fallthrough]];
      case zusi::Command::CvWrite: [[fallthrough]];
      case zusi::Command::ZppWrite: return frame[zusi::data_cnt_pos];
      default: return std::unexpected{std::errc::protocol_error};
    }
  return std::nullopt;
}

/// Helper to get an address from a ZUSI frame
///
/// \param  frame         ZUSI frame to search
/// \retval uint32_t      Address from the ZUSI frame
/// \retval std::nullopt  Frame is too short to contain an address
/// \retval std::errc     Frame cannot contain an address or address is out of
///                       valid bounds
constexpr std::expected<std::optional<uint32_t>, std::errc>
get_address(std::span<uint8_t const> frame) {
  if (size(frame) > (zusi::addr_pos + 3uz))
    switch (static_cast<zusi::Command>(frame[zusi::cmd_pos])) {
      case zusi::Command::CvRead: [[fallthrough]];
      case zusi::Command::CvWrite: [[fallthrough]];
      case zusi::Command::ZppWrite:
        return zusi::data2uint32(&frame[zusi::addr_pos]);
      default: return std::unexpected{std::errc::protocol_error};
    }
  return std::nullopt;
}

/// Helper to get flash / CV data from a ZUSI frame
///
/// \todo
/// Optimize return value creation (pass std::span to initializer of vector?)
///
/// \param  frame               ZUSI frame to search
/// \retval ztl::inplace_vector Frame data as an ordered vector
/// \retval std::nullopt        Frame is too short to contain all flash data
/// \retval std::errc           Frame cannot contain flash data
constexpr std::expected<std::optional<ztl::inplace_vector<uint8_t, 256uz>>,
                        std::errc>
get_data(std::span<uint8_t const> frame) {
  if (size(frame) > zusi::data_pos)
    switch (static_cast<zusi::Command>(frame[zusi::cmd_pos])) {
      case zusi::Command::CvWrite: [[fallthrough]];
      case zusi::Command::ZppWrite: {
        size_t const count{frame[zusi::data_cnt_pos]};
        if (size(frame) > (zusi::data_pos + count)) {
          ztl::inplace_vector<uint8_t, 256> tmp{};
          std::copy(&frame[zusi::data_pos],
                    &frame[zusi::data_pos + count],
                    std::back_inserter(tmp));
          return tmp;
        }
        break;
      }
      default: return std::unexpected{std::errc::protocol_error}; break;
    }
  return std::nullopt;
}

/// Helper to get the exit flags of a ZUSI frame
///
/// \param  frame         ZUSI frame to search
/// \retval uint8_t       Exit flags of the ZUSI frame
/// \retval std::nullopt  Frame is too short to contain exit flags
/// \retval std::errc     Frame data is corrupt
constexpr std::expected<std::optional<uint8_t>, std::errc>
get_exit_flags(std::span<uint8_t const> frame) {
  if (size(frame) > zusi::exit_flags_pos)
    switch (static_cast<zusi::Command>(frame[zusi::cmd_pos])) {
      case zusi::Command::Exit: return frame[zusi::exit_flags_pos];
      default: return std::unexpected{std::errc::protocol_error};
    }
  return std::nullopt;
}

/// Helper to get the checksum of a ZUSI frame
///
/// \param  frame         ZUSI frame to search
/// \retval uint8_t       Checksum of the ZUSI frame
/// \retval std::nullopt  Frame is too short to contain a checksum
/// \retval std::errc     Frame data is corrupt
constexpr std::expected<std::optional<uint8_t>, std::errc>
get_checksum(std::span<uint8_t const> frame) {
  // TODO: Replace magic numbers with macros/constexpr/similar

  if (size(frame) > zusi::cmd_pos) {
    switch (static_cast<zusi::Command>(frame[zusi::cmd_pos])) {
      case zusi::Command::CvRead:
        if (size(frame) == zusi::addr_pos + 3uz + 1uz + 1uz)
          return frame[zusi::addr_pos + 3uz + 1uz];
        break;
      case zusi::Command::CvWrite: [[fallthrough]];
      case zusi::Command::ZppWrite:
        if (size(frame) > zusi::data_cnt_pos)
          if (size(frame) ==
              zusi::data_pos + frame[zusi::data_cnt_pos] + 1uz + 1uz)
            return frame[zusi::data_pos + frame[zusi::data_cnt_pos] + 1uz];
        break;
      case zusi::Command::ZppErase:
        if (size(frame) == zusi::sec_bytes_pos + 1uz + 1uz + 1uz)
          return frame[zusi::sec_bytes_pos + 1uz + 1uz];
        break;
      case zusi::Command::Features:
        if (size(frame) == zusi::cmd_pos + 1uz + 1uz)
          return frame[zusi::cmd_pos + 1uz];
        break;
      case zusi::Command::Exit:
        if (size(frame) == zusi::exit_flags_pos + 1uz + 1uz)
          return frame[zusi::exit_flags_pos + 1uz];
        break;
      case zusi::Command::ZppLcDcQuery:
        if (size(frame) == zpplcdcquery_size)
          return frame[zpplcdcquery_size - 1uz];
        break;
      default: return std::unexpected{std::errc::protocol_error};
    }
  }
  return std::nullopt;
}

} // namespace ulf::susiv2
