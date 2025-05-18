#include <gtest/gtest.h>
#include "ulf/susiv2.hpp"

#include <algorithm>

constexpr std::array<uint8_t, 5uz> susiv2_pre{
  0x00u, 0x00u, 0x00u, 0x00u, 0x01u};
constexpr std::array<uint8_t, 11uz> cvwrite_zusi{
  0x02u, 0x03u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xAFu, 0xBFu, 0xCFu, 0xDFu, 0xD3u};

TEST(format, valid_CvWrite) {
  using namespace ulf::susiv2;

  // Valid CvWrite SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(cvwrite_zusi), end(cvwrite_zusi), std::back_inserter(sv2_frame));
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);

  ASSERT_TRUE(std::ranges::equal(**ret, cvwrite_zusi));
}

TEST(format, short_CvWrite) {
  using namespace ulf::susiv2;

  // Short CvWrite SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(cvwrite_zusi), end(cvwrite_zusi), std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_FALSE(*ret);
}

TEST(format, invalid_CvWrite) {
  using namespace ulf::susiv2;

  // Invalid CvWrite SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(cvwrite_zusi), end(cvwrite_zusi), std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  sv2_frame.push_back(0xFFu); // Faulty checksum
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_FALSE(ret);
}

TEST(format, too_long_CvWrite) {
  using namespace ulf::susiv2;

  // Long CvWrite SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(cvwrite_zusi), end(cvwrite_zusi), std::back_inserter(sv2_frame));

  sv2_frame.push_back(0xFFu); // Too much data
  sv2_frame.push_back(0xFAu); // Even more data
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);
  ASSERT_TRUE(std::ranges::equal(**ret, cvwrite_zusi));
}
