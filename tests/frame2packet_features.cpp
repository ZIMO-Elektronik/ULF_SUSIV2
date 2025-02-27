#include <gtest/gtest.h>
#include "ulf/susiv2.hpp"

constexpr std::array<uint8_t, 5uz> susiv2_pre{
  0x00u, 0x00u, 0x00u, 0x02u, 0x01u};
constexpr std::array<uint8_t, 2uz> features_zusi{0x06u, 0xDDu};

TEST(format, valid_Features) {
  using namespace ulf::susiv2;

  // Valid Features SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(features_zusi), end(features_zusi), std::back_inserter(sv2_frame));
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);

  auto it_v{begin(features_zusi)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}

TEST(format, short_Features) {
  using namespace ulf::susiv2;

  // Short Features SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(features_zusi), end(features_zusi), std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_FALSE(*ret);

  auto it_v{begin(sv2_frame)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}

TEST(format, invalid_Features) {
  using namespace ulf::susiv2;

  // Invalid Features SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(features_zusi), end(features_zusi), std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  sv2_frame.push_back(0xFFu); // Faulty checksum
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_FALSE(ret);

  auto it_v{begin(sv2_frame)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}

TEST(format, too_long_Features) {
  using namespace ulf::susiv2;

  // Long Features SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(
    begin(features_zusi), end(features_zusi), std::back_inserter(sv2_frame));

  sv2_frame.push_back(0xFFu); // Too much data
  sv2_frame.push_back(0xFAu); // Even more data
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);
  ASSERT_EQ(size(frame_s), size(features_zusi)) << "Too much data was returned";

  auto it_v{begin(features_zusi)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}