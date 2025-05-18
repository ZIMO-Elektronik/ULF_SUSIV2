#include <gtest/gtest.h>
#include "ulf/susiv2.hpp"

constexpr std::array<uint8_t, 5uz> susiv2_pre{
  0x00u, 0x00u, 0x00u, 0x02u, 0x01u};
constexpr std::array<uint8_t, 4uz> flasherase_zusi{0x04u, 0x55u, 0xAAu, 0xC7};

TEST(format, valid_FlashErase) {
  using namespace ulf::susiv2;

  // Valid FlashErase SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(flasherase_zusi),
            end(flasherase_zusi),
            std::back_inserter(sv2_frame));
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);

  ASSERT_TRUE(std::ranges::equal(**ret, flasherase_zusi));
}

TEST(format, short_FlashErase) {
  using namespace ulf::susiv2;

  // Short FlashErase SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(flasherase_zusi),
            end(flasherase_zusi),
            std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_FALSE(*ret);
}

TEST(format, invalid_FlashErase) {
  using namespace ulf::susiv2;

  // Invalid FlashErase SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(flasherase_zusi),
            end(flasherase_zusi),
            std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  sv2_frame.push_back(0xFFu); // Faulty checksum
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_FALSE(ret);
}

TEST(format, too_long_FlashErase) {
  using namespace ulf::susiv2;

  // Long FlashErase SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(flasherase_zusi),
            end(flasherase_zusi),
            std::back_inserter(sv2_frame));

  sv2_frame.push_back(0xFFu); // Too much data
  sv2_frame.push_back(0xFAu); // Even more data
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);

  ASSERT_TRUE(std::ranges::equal(**ret, flasherase_zusi));
}