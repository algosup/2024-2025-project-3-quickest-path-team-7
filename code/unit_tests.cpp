   #include <gtest/gtest.h>

   // A simple test case
   TEST(SampleTest, BasicAssertions) {
       // Check if 1 + 1 equals 2
       EXPECT_EQ(1 + 1, 2);
       // Check if 1 + 1 does not equal 3
       EXPECT_NE(1 + 1, 3);
   }

   int main(int argc, char **argv) {
       ::testing::InitGoogleTest(&argc, argv);
       return RUN_ALL_TESTS();
   }