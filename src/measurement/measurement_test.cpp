#include <gtest/gtest.h>
#include <stdio.h>
#include "measurement.h"

class MeasurementDataTest4x4 : public ::testing::Test
{
protected:
  unsigned char *testData;
  unsigned int *preprocessedTestData;

  virtual void SetUp() override
  {
    std::cout << "Test SetUp" << std::endl;
    this->testData = (unsigned char *)malloc(4 * 4 * sizeof(unsigned char));
    this->preprocessedTestData = (unsigned int *)malloc(4 * 4 * sizeof(unsigned int));

    this->testData[0] = 0x01;
    this->testData[1] = 0x03;
    this->testData[2] = 0x02;
    this->testData[3] = 0x05;
    this->testData[4] = 0x08;
    this->testData[5] = 0x07;
    this->testData[6] = 0x04;
    this->testData[7] = 0x09;
    this->testData[8] = 0x06;
    this->testData[9] = 0x02;
    this->testData[10] = 0x0a;
    this->testData[11] = 0x03;
    this->testData[12] = 0x03;
    this->testData[13] = 0x01;
    this->testData[14] = 0x08;
    this->testData[15] = 0x00;

    this->preprocessedTestData[0] = 1;
    this->preprocessedTestData[1] = 4;
    this->preprocessedTestData[2] = 6;
    this->preprocessedTestData[3] = 11;
    this->preprocessedTestData[4] = 9;
    this->preprocessedTestData[5] = 19;
    this->preprocessedTestData[6] = 25;
    this->preprocessedTestData[7] = 39;
    this->preprocessedTestData[8] = 15;
    this->preprocessedTestData[9] = 27;
    this->preprocessedTestData[10] = 43;
    this->preprocessedTestData[11] = 60;
    this->preprocessedTestData[12] = 18;
    this->preprocessedTestData[13] = 31;
    this->preprocessedTestData[14] = 55;
    this->preprocessedTestData[15] = 72;
  }

  void TearDown() override
  {
    std::cout << "Test TearDown" << std::endl;
    free(this->testData);
    free(this->preprocessedTestData);
  }
};

TEST_F(MeasurementDataTest4x4, TestCreateSucceeds)
{
  MD *md = MD_create(testData, 4, 4);

  ASSERT_NE(md, nullptr);

  EXPECT_EQ(0, memcmp(preprocessedTestData, md->buffer, 4 * 4 * sizeof(unsigned int)));
  EXPECT_EQ(4, md->width);
  EXPECT_EQ(4, md->height);

  MD_delete(md);
}

TEST_F(MeasurementDataTest4x4, TestCreateFailsForInvalidData)
{
  MD *md = MD_create(testData, 4097, 4);

  ASSERT_EQ(md, nullptr);

  md = MD_create(testData, 4, 4097);

  ASSERT_EQ(md, nullptr);
}

TEST_F(MeasurementDataTest4x4, TestCopySucceeds)
{
  MD *md = MD_create(testData, 4, 4);

  MD *md2 = MD_copy(md);
  EXPECT_NE(md2->buffer, md->buffer); // Important check: Deep copy of buffer -> memory location different
  EXPECT_EQ(0, memcmp(md2->buffer, md->buffer, 4 * 4 * sizeof(unsigned int)));
  EXPECT_EQ(md2->width, md->width);
  EXPECT_EQ(md2->height, md->height);

  MD_delete(md);
  MD_delete(md2);
}

TEST_F(MeasurementDataTest4x4, TestGetSumWithUpperLeftZeroWindow)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, 0, 0, 0, 0);
  MD_delete(md);
  EXPECT_EQ(sum, (unsigned int)testData[0]);
}

TEST_F(MeasurementDataTest4x4, TestGetSumWithUpperRightZeroWindow)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, 3, 0, 3, 0);
  MD_delete(md);
  EXPECT_EQ(sum, (unsigned int)testData[3]);
}

TEST_F(MeasurementDataTest4x4, TestGetSumWithLowerLeftZeroWindow)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, 0, 3, 0, 3);
  MD_delete(md);
  EXPECT_EQ(sum, (unsigned int)testData[12]);
}

TEST_F(MeasurementDataTest4x4, TestGetSumWithLowerRightZeroWindow)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, 3, 3, 3, 3);
  MD_delete(md);
  EXPECT_EQ(sum, (unsigned int)testData[15]);
}

TEST_F(MeasurementDataTest4x4, TestGetSumLeftOutOfBoundsWindow)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, -2, 0, -1, 3);
  MD_delete(md);
  EXPECT_EQ(sum, 0);
}

TEST_F(MeasurementDataTest4x4, TestGetSumRightOutOfBoundsWindow)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, 3 + 1, 0, 3 + 2, 3);
  MD_delete(md);
  EXPECT_EQ(sum, 0);
}

TEST_F(MeasurementDataTest4x4, TestGetSumTopOutOfBoundsWindow)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, 0, -2, 3, -1);
  MD_delete(md);
  EXPECT_EQ(sum, 0);
}

TEST_F(MeasurementDataTest4x4, TestGetSumBottomOutOfBoundsWindow)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, 0, 3 + 1, 3, 3 + 2);
  MD_delete(md);
  EXPECT_EQ(sum, 0);
}

TEST_F(MeasurementDataTest4x4, TestGet2x2CenterSum)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, 1, 1, 2, 2);
  MD_delete(md);
  EXPECT_EQ(sum, preprocessedTestData[10] - preprocessedTestData[2] - preprocessedTestData[8] + preprocessedTestData[0]);
}

TEST_F(MeasurementDataTest4x4, TestGet2x2UpperLeftCornerSum)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, -1, -1, 0, 0);
  MD_delete(md);
  EXPECT_EQ(sum, testData[0]);
}

TEST_F(MeasurementDataTest4x4, TestGet2x2UpperRightCornerSum)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int sum = MD_getSum(md, 3, -1, 4, 0);
  MD_delete(md);
  EXPECT_EQ(sum, testData[3]);
}

TEST_F(MeasurementDataTest4x4, TestGetAverageComplete)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int average = MD_getAverage(md, 0, 0, 3, 3);
  MD_delete(md);
  EXPECT_EQ(average, preprocessedTestData[15] / 16);
}

TEST_F(MeasurementDataTest4x4, TestGetAverage16OutOfBoundsFields)
{
  MD *md = MD_create(testData, 4, 4);
  const unsigned int average = MD_getAverage(md, -1, -1, 4, 4);
  MD_delete(md);
  EXPECT_EQ(average, preprocessedTestData[15] / 32);
}

TEST(LargeDataTest, TestCreateSucceeds)
{
  const unsigned int testDataSize = 4096 * 4096 * sizeof(unsigned char);
  unsigned char *testData = (unsigned char *)malloc(testDataSize);
  memset(testData, 0xff, testDataSize); // filling buffer with maximum value for each cell

  MD *md = MD_create(testData, 4096, 4096);

  free(testData);

  ASSERT_NE(md, nullptr);

  const unsigned int maxPreprocessedBufferValue = (unsigned int)4096 * 4096 * 255;
  EXPECT_EQ(md->buffer[4096 * 4096 - 1], maxPreprocessedBufferValue);
  EXPECT_EQ(4096, md->width);
  EXPECT_EQ(4096, md->height);

  MD_delete(md);
}
