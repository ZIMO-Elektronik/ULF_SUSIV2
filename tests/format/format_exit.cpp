#include <gtest/gtest.h>
#include "ulf/susiv2.hpp"

constexpr std::array<uint8_t, 5uz> susiv2_pre{
  0x00u, 0x00u, 0x00u, 0x02u, 0x01u};
constexpr std::array<uint8_t, 5uz> exit_zusi{0x07u, 0x55u, 0xAAu, 0x02u, 0x7Du};

TEST(format, valid_Exit) {
  using namespace ulf::susiv2;

  // Valid Exit SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(exit_zusi), end(exit_zusi), std::back_inserter(sv2_frame));
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{format(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);

  auto it_v{begin(exit_zusi)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}

TEST(format, short_Exit) {
  using namespace ulf::susiv2;

  // Short Exit SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(exit_zusi), end(exit_zusi), std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{format(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_FALSE(*ret);

  auto it_v{begin(sv2_frame)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}

TEST(format, invalid_Exit) {
  using namespace ulf::susiv2;

  // Invalid Exit SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(exit_zusi), end(exit_zusi), std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  sv2_frame.push_back(0xFFu); // Faulty checksum
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{format(frame_s)};
  ASSERT_FALSE(ret);

  auto it_v{begin(sv2_frame)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}

TEST(format, too_long_Exit) {
  using namespace ulf::susiv2;

  // Long Exit SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(exit_zusi), end(exit_zusi), std::back_inserter(sv2_frame));

  sv2_frame.push_back(0xFFu); // Too much data
  sv2_frame.push_back(0xFAu); // Even more data
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{format(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);
  ASSERT_EQ(size(frame_s), size(exit_zusi)) << "Too much data was returned";

  auto it_v{begin(exit_zusi)};
  for (auto it_t : frame_s) { ASSERT_EQ(it_t, *it_v++); }
}