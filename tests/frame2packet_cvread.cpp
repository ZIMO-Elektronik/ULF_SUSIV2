#include <gtest/gtest.h>
#include "ulf/susiv2.hpp"

constexpr std::array<uint8_t, 5uz> susiv2_pre{
  0x00u, 0x00u, 0x00u, 0x02u, 0x01u};
constexpr std::array<uint8_t, 7uz> cvread_zusi{
  0x01u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0x02u};

TEST(format, valid_CvRead) {
  using namespace ulf::susiv2;

  // Valid CvRead SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(cvread_zusi), end(cvread_zusi), std::back_inserter(sv2_frame));
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);

  auto it_v{begin(cvread_zusi)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}

TEST(format, short_CvRead) {
  using namespace ulf::susiv2;

  // Short CvRead SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(cvread_zusi), end(cvread_zusi), std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_FALSE(*ret);

  auto it_v{begin(sv2_frame)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}

TEST(format, invalid_CvRead) {
  using namespace ulf::susiv2;

  // Invalid CvRead SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(cvread_zusi), end(cvread_zusi), std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  sv2_frame.push_back(0xFFu); // Faulty checksum
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_FALSE(ret);

  auto it_v{begin(sv2_frame)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}

TEST(format, too_long_CvRead) {
  using namespace ulf::susiv2;

  // Long CvRead SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(cvread_zusi), end(cvread_zusi), std::back_inserter(sv2_frame));

  sv2_frame.push_back(0xFFu); // Too much data
  sv2_frame.push_back(0xFAu); // Even more data
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);
  ASSERT_EQ(size(frame_s), size(cvread_zusi)) << "Too much data was returned";

  auto it_v{begin(cvread_zusi)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}