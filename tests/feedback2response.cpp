#include <gtest/gtest.h>
#include "ulf/susiv2.hpp"

using namespace ulf::susiv2;

TEST(feedback2response, empty_feedback) {
  EXPECT_EQ((feedback2response(zusi::Feedback{})), Response{nak});
}

TEST(feedback2response, ack_feedback) {
  EXPECT_EQ(
    (feedback2response(zusi::Feedback{ztl::inplace_vector<uint8_t, 4uz>{}})),
    Response{ack});
}

TEST(feedback2response, cv_feedback) {
  EXPECT_EQ((feedback2response(zusi::Feedback{{42}})),
            (Response{ack, 42, zusi::crc8(42)}));
}
