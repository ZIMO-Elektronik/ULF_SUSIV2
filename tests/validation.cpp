#include <gtest/gtest.h>
#include <vector>
#include <zusi/crc8.hpp>
#include "ulf/susiv2.hpp"

TEST(validation, CvRead_validation) {
  using namespace ulf::susiv2;

  // Valid CvRead Frame
  std::vector<uint8_t> frame{0x01u, 0x01u, 0x00u, 0x00u, 0x00u, 0xFFu};
  frame.push_back(zusi::crc8(frame));
  ASSERT_TRUE(validate(frame));
  ASSERT_TRUE(*validate(frame));
  ASSERT_EQ(**validate(frame), true);

  // Incomplete CvRead Frame
  frame.pop_back();
  ASSERT_TRUE(validate(frame))
    << "Insufficient data should not return an error";
  ASSERT_FALSE(*validate(frame))
    << "Should not return values since the crc8 byte is missing";

  // Wrong CRC8
  frame.push_back(0x00);
  ASSERT_FALSE(validate(frame)) << "Incorrect CRC8 was ignored";
}

TEST(validation, CvWrite_validation) {
  using namespace ulf::susiv2;

  // Valid CvWrite Frame
  ztl::inplace_vector<uint8_t, 4uz> values{0x01u, 0x02u, 0x03u, 0x04u};
  std::vector<uint8_t> frame{
    0x02u, size(values) - 1uz, 0x00u, 0x00u, 0x00u, 0xFFu};
  std::copy(begin(values), end(values), std::back_inserter(frame));
  frame.push_back(zusi::crc8(frame));
  ASSERT_TRUE(validate(frame));
  ASSERT_TRUE(*validate(frame));
  ASSERT_EQ(**validate(frame), true);

  // Incomplete CvWrite Frame
  frame.pop_back();
  ASSERT_TRUE(validate(frame))
    << "Insufficient data should not return an error";
  ASSERT_FALSE(*validate(frame))
    << "Should not return values since the crc8 byte is missing";

  // Wrong CRC8
  frame.push_back(0x00);
  ASSERT_FALSE(validate(frame)) << "Incorrect CRC8 was ignored";
}

TEST(validation, DeleteFlash_validation) {
  using namespace ulf::susiv2;

  // Valid FlashDelete Frame
  std::vector<uint8_t> frame{0x04u, 0x55u, 0xAAu};
  frame.push_back(zusi::crc8(frame));
  ASSERT_TRUE(validate(frame));
  ASSERT_TRUE(*validate(frame));
  ASSERT_EQ(**validate(frame), true);

  // Incomplete FlashDelete Frame
  frame.pop_back();
  ASSERT_TRUE(validate(frame))
    << "Insufficient data should not return an error";
  ASSERT_FALSE(*validate(frame))
    << "Should not return values since the crc8 byte is missing";

  // Wrong CRC8
  frame.push_back(0x00);
  ASSERT_FALSE(validate(frame)) << "Incorrect CRC8 was ignored";
}

TEST(validation, WriteFlash_validation) {
  using namespace ulf::susiv2;

  // Valid FlashWrite Frame
  ztl::inplace_vector<uint8_t, 4uz> values{0x01u, 0x02u, 0x03u, 0x04u};
  std::vector<uint8_t> frame{
    0x05u, size(values) - 1uz, 0x00u, 0x00u, 0x00u, 0xFFu};
  std::copy(begin(values), end(values), std::back_inserter(frame));
  frame.push_back(zusi::crc8(frame));
  ASSERT_TRUE(validate(frame));
  ASSERT_TRUE(*validate(frame));
  ASSERT_EQ(**validate(frame), true);

  // Incomplete FlashWrite Frame
  frame.pop_back();
  ASSERT_TRUE(validate(frame))
    << "Insufficient data should not return an error";
  ASSERT_FALSE(*validate(frame))
    << "Should not return values since the crc8 byte is missing";

  // Wrong CRC8
  frame.push_back(0x00);
  ASSERT_FALSE(validate(frame)) << "Incorrect CRC8 was ignored";
}

TEST(validation, FeatureRequest_validation) {
  using namespace ulf::susiv2;

  // Valid FeatureRequest Frame
  std::vector<uint8_t> frame{0x06u};
  frame.push_back(zusi::crc8(frame));
  ASSERT_TRUE(validate(frame));
  ASSERT_TRUE(*validate(frame));
  ASSERT_EQ(**validate(frame), true);

  // Incomplete FeatureRequest Frame
  frame.pop_back();
  ASSERT_TRUE(validate(frame))
    << "Insufficient data should not return an error";
  ASSERT_FALSE(*validate(frame))
    << "Should not return values since the crc8 byte is missing";

  // Wrong CRC8
  frame.push_back(0x00u);
  ASSERT_FALSE(validate(frame)) << "Incorrect CRC8 was ignored";
}

TEST(validation, Exit_validation) {
  using namespace ulf::susiv2;

  // Valid Exit Frame
  std::vector<uint8_t> frame{0x07u, 0x55u, 0xAAu, 0x00u};
  frame.push_back(zusi::crc8(frame));
  ASSERT_TRUE(validate(frame));
  ASSERT_TRUE(*validate(frame));
  ASSERT_EQ(**validate(frame), true);

  // Incomplete Exit Frame
  frame.pop_back();
  ASSERT_TRUE(validate(frame))
    << "Insufficient data should not return an error";
  ASSERT_FALSE(*validate(frame))
    << "Should not return values since the crc8 byte is missing";

  // Wrong CRC8
  frame.push_back(0x00);
  ASSERT_FALSE(validate(frame)) << "Incorrect CRC8 was ignored";
}
