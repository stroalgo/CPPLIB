#include "GenericSingleton.h"

#include <gtest/gtest.h>

#include <iostream>

class MyClass {
 public:
  MyClass() = default;

  void DoSomeThing() {
    // Example method
    std::cout << "DoSomeThing!" << std::endl;
  }
};

class MyClassSingletonWrap
    : public Stroalgo::Common::GenericSingleton<MyClass> {
  friend class Stroalgo::Common::GenericSingleton<
      MyClass>;  // Allow Singleton access to MyClass constructor
 private:
  MyClassSingletonWrap() = default;
};

TEST(GenericSingletonTest, NoThrow) {
  EXPECT_NO_THROW(MyClassSingletonWrap::GetInstance());
  EXPECT_NO_THROW(MyClassSingletonWrap::GetInstance().DoSomeThing());
}

TEST(GenericSingletonTest, SameInstance) {
  MyClass& lFirstInstance = MyClassSingletonWrap::GetInstance();
  MyClass& lSecondInstance = MyClassSingletonWrap::GetInstance();
  EXPECT_EQ(&lFirstInstance, &lSecondInstance);
}
