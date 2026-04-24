#include <gtest/gtest.h>
#include <ulf/susiv2.hpp>

TEST(packet2frame, cvread) {
  std::vector<uint8_t> prefix{0x00u, 0x00u, 0x00u, 0x02u, 0x01u};
  auto const packet = zusi::make_cv_read_packet(0u, 7u);

  std::vector<uint8_t> expected{prefix};
  std::ranges::copy(packet, std::back_inserter(expected));

  auto result{ulf::susiv2::packet2frame<std::vector<uint8_t>>(packet)};

  ASSERT_EQ(expected, result);
}