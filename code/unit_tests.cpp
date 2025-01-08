#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include "hello_world.cpp"  // Include hello_world.cpp to use printHelloWorld

// Test fixture to capture output
class HelloWorldTest : public ::testing::Test {
protected:
    std::ostringstream captured_output;

    // Override the stream buffer of std::cout to capture printed output
    void SetUp() override {
        std::streambuf* original_buffer = std::cout.rdbuf();
        std::cout.rdbuf(captured_output.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(nullptr);  // Reset std::cout to original buffer
    }
};

// Test case: check if printHelloWorld() outputs the correct string
TEST_F(HelloWorldTest, OutputsHelloWorld) {
    printHelloWorld();
    EXPECT_EQ(captured_output.str(), "Hello, World!\n");
}

// Test case: check if printHelloWorld() does NOT output an incorrect string
TEST_F(HelloWorldTest, DoesNotOutputIncorrectString) {
    printHelloWorld();
    EXPECT_NE(captured_output.str(), "Hello, Universe!\n");  // Deliberate incorrect string
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}