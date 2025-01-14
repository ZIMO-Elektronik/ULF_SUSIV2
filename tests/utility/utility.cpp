#include <gtest/gtest.h>
#include <ulf/susiv2.hpp>
#include <vector>
#include <zusi/command.hpp>

TEST(utility, command_extract) {
  using namespace ulf::susiv2;

  // Valid cmd byte (CvWrite)
  uint8_t cmd{std::to_underlying(zusi::Command::CvRead)};
  std::vector<uint8_t> frame{cmd, 0xFFu};
  ASSERT_TRUE(get_command(frame)) << "Valid CMD Byte was rejected";
  ASSERT_TRUE(*get_command(frame)) << "Frame didn't have enough data";
  ASSERT_EQ(**get_command(frame), zusi::Command::CvRead)
    << "Unexpected CMD returned";

  // Invalid cmd byte
  frame = {0x00u, 0xFFu};
  ASSERT_FALSE(get_command(frame)) << "CMD Byte should be invalid";

  // Insufficient data
  frame = {};
  ASSERT_TRUE(get_command(frame)) << "Mistakenly tried to validate CMD Byte";
  ASSERT_FALSE(*get_command(frame)) << "Positive return without any CMD Byte";
}

TEST(utility, count_extract) {
  using namespace ulf::susiv2;

  // Valid count in  CvRead CMD
  std::vector<uint8_t> frame{0x01u, 0xFFu};
  ASSERT_TRUE(get_count(frame)) << "Valid Count caused an error";
  ASSERT_TRUE(*get_count(frame)) << "Frame didn't have enough data for search";
  ASSERT_EQ(**get_count(frame), 0xFFu)
    << "Count was modified or search position was wrong";

  // Valid count in CvWrite CMD
  frame = {0x02, 0xFFu};
  ASSERT_TRUE(get_count(frame)) << "Valid Count caused an error";
  ASSERT_TRUE(*get_count(frame)) << "Frame didn't have enough data for search";
  ASSERT_EQ(**get_count(frame), 0xFFu)
    << "Count was modified or search position was wrong";

  // Insufficient data CvRead CMD
  frame = {0x01u};
  ASSERT_TRUE(get_count(frame)) << "CvRead should never cause an error";
  ASSERT_FALSE(*get_count(frame))
    << "Should not be possible to read count with too little data";
}

TEST(utility, address_extract) {
  using namespace ulf::susiv2;

  // Valid address in CvRead CMD
  std::vector<uint8_t> frame{0x01u, 0xFFu, 0x00u, 0x00u, 0x00u, 0xFFu};
  ASSERT_TRUE(get_address(frame))
    << "Valid address in valid command caused an error";
  ASSERT_TRUE(*get_address(frame))
    << "Should be enough data to determine address";
  ASSERT_EQ(**get_address(frame), 0x000000FFu);

  // Insufficient data in CvRead CMD
  frame = {0x01u, 0xFFu, 0x00u, 0x01u, 0x02u};
  ASSERT_TRUE(get_address(frame))
    << "Should return insufficient data but returned an error";
  ASSERT_FALSE(*get_address(frame))
    << "Should not return a valid address since it it incomplete";
}

TEST(utility, data_extract) {
  using namespace ulf::susiv2;

  // Valid values in FlashWrite CMD
  ztl::inplace_vector<uint8_t, 4uz> vals{0x01u, 0x02u, 0x03u, 0x04u};
  std::vector<uint8_t> frame{
    0x05u, size(vals) - 1uz, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
  std::copy(begin(vals), end(vals), std::back_inserter(frame));
  auto tmp{get_data(frame)};
  ASSERT_TRUE(tmp) << "Valid frame part marked as error";
  ASSERT_TRUE(*tmp) << "Valid frame returned insufficient data";
  // Compare each element of return with input
  auto it_v{begin(vals)};
  for (auto it_t : **tmp) { ASSERT_EQ(it_t, *it_v++) << "Data Mismatch"; }

  // Insufficient data
  frame = {0x05u, 0x01u, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
  ASSERT_TRUE(get_data(frame))
    << "Insufficient data should not return an error";
  ASSERT_FALSE(*get_data(frame))
    << "Should not return values since we have incomplete data";

  // Less values than count would tell
  frame = {0x05u, 0x04u, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
  std::copy(begin(vals), end(vals), std::back_inserter(frame));
  ASSERT_TRUE(get_data(frame))
    << "Insufficient data should not return an error";
  ASSERT_FALSE(*get_data(frame))
    << "Should not return values since we didn't insert enough values";
}

TEST(utility, checksum_extract) {
  using namespace ulf::susiv2;

  // Valid CvWrite CMD Frame
  std::vector<uint8_t> frame{0x01u, 0x01u, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
  frame.push_back(zusi::crc8(frame));
  ASSERT_TRUE(get_checksum(frame)) << "Valid frame returned an error";
  ASSERT_TRUE(*get_checksum(frame)) << "Valid frame returned insufficient data";
  ASSERT_EQ(**get_checksum(frame), frame.back()) << "Data mismatch";

  // Insufficient data
  frame.pop_back();
  ASSERT_TRUE(get_checksum(frame))
    << "Insufficient data should not return an error";
  ASSERT_FALSE(*get_checksum(frame))
    << "Should not return a checksum since data is incomplete";
}
