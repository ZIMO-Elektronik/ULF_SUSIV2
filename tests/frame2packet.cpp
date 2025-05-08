#include <gtest/gtest.h>
#include "ulf/susiv2.hpp"

TEST(format, test_format_no_checks) {
  using namespace ulf::susiv2;

  // Valid CvRead SUSIV2 Frame
  std::vector<uint8_t> frame{0x01u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu};
  frame.push_back(zusi::crc8(frame));
  std::vector<uint8_t> sv2_frame{0x00u, 0x00u, 0x00u, 0x02u, 0x00u};
  std::copy(begin(frame), end(frame), std::back_inserter(sv2_frame));
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet_no_validate(frame_s)};

  ASSERT_TRUE(std::ranges::equal(ret, frame));
}