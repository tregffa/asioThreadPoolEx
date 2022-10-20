#include "gtest/gtest.h"
#include "threadPool.h"
#include <iostream>

using namespace std::chrono;
using namespace std::this_thread;

std::atomic_int counter = 0;

class ThreadPoolTest : public ::testing::Test {
protected:
    void SetUp() override {
        pool = std::make_shared<ThreadPool>();
        counter = 0;
    }

    void TearDown() override {
        pool.reset();
    }

    std::shared_ptr<ThreadPool> pool;
};

void longTask() {
    counter++;
    sleep_for(milliseconds(10));
}

int longTaskChangeValue(int value) {
    sleep_for(milliseconds(10));
    return ++value;
}

TEST_F(ThreadPoolTest, ShouldInitializeWithGivenThreadsCount) {
    pool = std::make_shared<ThreadPool>(2);
    pool->pushTask(longTask);
    pool->pushTask(longTask);
    pool->pushTask(longTask);
    sleep_for(milliseconds(2));
    EXPECT_EQ(counter, 2);
}
TEST_F(ThreadPoolTest, ShouldExecuteTasks) {
    int result = 0;
    pool->pushTask([&result]() {result++; });
    pool->join();
    EXPECT_EQ(result, 1);
}
TEST_F(ThreadPoolTest, ShouldReturnValue) {
    int value = 10;
    auto [id, result] = pool->pushTask(longTaskChangeValue, value);
    EXPECT_EQ(result.get(), 11);
}
TEST_F(ThreadPoolTest, ShouldSaveTasksInQueue) {
}
TEST_F(ThreadPoolTest, ShouldLimitQueue) {
}
TEST_F(ThreadPoolTest, ShouldExecuteAllTasksInQueue) {
}
TEST_F(ThreadPoolTest, ShouldShowTasksStatus) {
}

TEST_F(ThreadPoolTest, ShouldNotChangeArguments) {
}