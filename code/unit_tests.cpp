#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include "hello_world.cpp"  // Include hello_world.cpp to use printHelloWorld


// Declare the printHelloWorld function (so we can call it in the tests)
void printHelloWorld();

// Test fixture to capture output
class HelloWorldTest : public ::testing::Test {
protected:
    std::ostringstream captured_output;

    void SetUp() override {
        std::streambuf* original_buffer = std::cout.rdbuf();
        std::cout.rdbuf(captured_output.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(nullptr);  // Reset std::cout to the original buffer
    }
};

TEST_F(HelloWorldTest, OutputsHelloWorld) {
    printHelloWorld();
    EXPECT_EQ(captured_output.str(), "Hello, World!\n");
}

TEST_F(HelloWorldTest, DoesNotOutputIncorrectString) {
    printHelloWorld();
    EXPECT_NE(captured_output.str(), "Hello, Universe!\n");
}
