#include <gtest/gtest.h>
#include "ulf/susiv2.hpp"

constexpr std::array<uint8_t, 5uz> susiv2_pre{
  0x00u, 0x00u, 0x00u, 0x00u, 0x01u};
constexpr std::array<uint8_t, 11uz> flashwrite_zusi{
  0x05u, 0x03u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xAFu, 0xBFu, 0xCFu, 0xDFu, 0x8Bu};

TEST(format, valid_FlashWrite) {
  using namespace ulf::susiv2;

  // Valid FlashWrite SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(flashwrite_zusi),
            end(flashwrite_zusi),
            std::back_inserter(sv2_frame));
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);

  ASSERT_TRUE(std::ranges::equal(**ret, flashwrite_zusi));
}

TEST(format, short_FlashWrite) {
  using namespace ulf::susiv2;

  // Short FlashWrite SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(flashwrite_zusi),
            end(flashwrite_zusi),
            std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_FALSE(*ret);
}

TEST(format, invalid_FlashWrite) {
  using namespace ulf::susiv2;

  // Invalid FlashWrite SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(flashwrite_zusi),
            end(flashwrite_zusi),
            std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  sv2_frame.push_back(0xFFu); // Faulty checksum
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_FALSE(ret);
}

TEST(format, too_long_FlashWrite) {
  using namespace ulf::susiv2;

  // Long FlashWrite SUSIV2 Frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(flashwrite_zusi),
            end(flashwrite_zusi),
            std::back_inserter(sv2_frame));

  sv2_frame.push_back(0xFFu); // Too much data
  sv2_frame.push_back(0xFAu); // Even more data
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);

  ASSERT_TRUE(std::ranges::equal(**ret, flashwrite_zusi));
}