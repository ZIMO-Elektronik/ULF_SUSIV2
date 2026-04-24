#include <gtest/gtest.h>
#include <ulf/susiv2.hpp>

//TEST(packet2frame, cvwrite) {
//  std::vector<uint8_t> prefix{0x00u, 0x00u, 0x00u, 0x00u, 0x01u};
//  auto const packet = zusi::make_cv_write_packet(0u, 8u, std::array<uint8_t, 1u>{8u});
//
//  std::vector<uint8_t> expected{prefix};
//  std::ranges::copy(packet, std::back_inserter(expected));
//
//  auto result{ulf::susiv2::packet2frame<std::vector<uint8_t>>(packet)};
//
//  ASSERT_EQ(expected, result);
//}