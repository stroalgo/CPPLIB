#include "server.h"
#include <gtest/gtest.h>


class TodoListTest : public ::testing::Test {
protected:
    void SetUp() override {
        Utilities::todoList.clear();
        Utilities::nextId = 1;
    }
};




// Test parseTaskFromBody
TEST_F(TodoListTest, ParseTaskFromBody) {
    std::string body = "{\"task\":\"Buy groceries\"}";
    std::string task = Utilities::parseTaskFromBody(body);
    EXPECT_EQ(task, "Buy groceries");
}

// Test addTask
TEST_F(TodoListTest, AddTask) {
    int id = Utilities::addTask("Buy groceries");
    EXPECT_EQ(id, 1);
    EXPECT_EQ(Utilities::todoList[1], "Buy groceries");
}

// Test getAllTasks
TEST_F(TodoListTest, GetAllTasks) {
    Utilities::addTask("Buy groceries");
    Utilities::addTask("Walk the dog");
    std::string expected = "[{\"id\":1,\"task\":\"Buy groceries\"},{\"id\":2,\"task\":\"Walk the dog\"}]";
    EXPECT_EQ(Utilities::getAllTasks(), expected);
}

// Test updateTask
TEST_F(TodoListTest, UpdateTask) {
    Utilities::addTask("Buy groceries");
    bool result = Utilities::updateTask(1, "Buy milk");
    EXPECT_TRUE(result);
    EXPECT_EQ(Utilities::todoList[1], "Buy milk");

    result = Utilities::updateTask(2, "Invalid task"); // Non-existent ID
    EXPECT_FALSE(result);
}

// Test deleteTask
TEST_F(TodoListTest, DeleteTask) {
    Utilities::addTask("Buy groceries");
    bool result = Utilities::deleteTask(1);
    EXPECT_TRUE(result);
    EXPECT_EQ(Utilities::todoList.size(), 0);

    result = Utilities::deleteTask(2); // Non-existent ID
    EXPECT_FALSE(result);
}