#include <gtest/gtest.h>
#include "ulf/susiv2.hpp"

constexpr std::array<uint8_t, 5uz> susiv2_pre{
  0x00u, 0x00u, 0x00u, 0x00u, 0x01u};
constexpr std::array<uint8_t, 6uz> lc_dc_query_zusi{
  0x0Du, 0x00u, 0x01u, 0x02u, 0x03u, 0x34u};

TEST(format, valid_LcDc_Query) {
  using namespace ulf::susiv2;

  // Valid LCDC_Qucrc SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(lc_dc_query_zusi),
            end(lc_dc_query_zusi),
            std::back_inserter(sv2_frame));
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);

  ASSERT_TRUE(std::ranges::equal(**ret, lc_dc_query_zusi));
}

TEST(format, short_LcDc_Query) {
  using namespace ulf::susiv2;

  // Short LCDC_Qucrc SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(lc_dc_query_zusi),
            end(lc_dc_query_zusi),
            std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_FALSE(*ret);
}

TEST(format, invalidcLcDC_Query) {
  using namespace ulf::susiv2;

  // Invalid LCDC_Qucrc SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(lc_dc_query_zusi),
            end(lc_dc_query_zusi),
            std::back_inserter(sv2_frame));

  sv2_frame.pop_back();
  sv2_frame.push_back(0xFFu); // Faulty checksum
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_FALSE(ret);
}

TEST(format, too_lonc_LcDc_Query) {
  using namespace ulf::susiv2;

  // Long LCDC_Qucrc SUSIV2 frame
  std::vector<uint8_t> sv2_frame{begin(susiv2_pre), end(susiv2_pre)};
  std::copy(begin(lc_dc_query_zusi),
            end(lc_dc_query_zusi),
            std::back_inserter(sv2_frame));

  sv2_frame.push_back(0xFFu); // Too much data
  sv2_frame.push_back(0xFAu); // Even more data
  std::span<uint8_t const> frame_s{sv2_frame};

  auto ret{frame2packet(frame_s)};
  ASSERT_TRUE(ret);
  ASSERT_TRUE(*ret);

  ASSERT_TRUE(std::ranges::equal(**ret, lc_dc_query_zusi));
}
