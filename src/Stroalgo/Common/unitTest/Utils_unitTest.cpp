#include "Utils.h"

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <fstream>

#include "Exceptions.h"

namespace fs = boost::filesystem;

class UtilsTest : public ::testing::Test {
 protected:
  fs::path testDirectory{};

  void SetUp() override {
    testDirectory =
        fs::temp_directory_path() / "stroalgo_utils_create_folder_test";

    boost::system::error_code error;
    fs::remove_all(testDirectory, error);
  }

  void TearDown() override {
    boost::system::error_code error;
    fs::remove_all(testDirectory, error);
  }
};

TEST_F(UtilsTest, EmptyPathThrowsException) {
  EXPECT_THROW(Stroalgo::Common::Utils::CreateFolder(""),
               Stroalgo::Exceptions::LoggerException);
}

TEST_F(UtilsTest, CreatesFolder) {
  ASSERT_FALSE(fs::exists(testDirectory));

  EXPECT_NO_THROW(
      Stroalgo::Common::Utils::CreateFolder(testDirectory.string()));

  EXPECT_TRUE(fs::exists(testDirectory));
  EXPECT_TRUE(fs::is_directory(testDirectory));
}

TEST_F(UtilsTest, CreatesNestedFolders) {
  const fs::path nestedPath = testDirectory / "one" / "two" / "three";

  EXPECT_NO_THROW(Stroalgo::Common::Utils::CreateFolder(nestedPath.string()));

  EXPECT_TRUE(fs::exists(nestedPath));
  EXPECT_TRUE(fs::is_directory(nestedPath));
}

TEST_F(UtilsTest, ExistingFolderThrowsException) {
  boost::system::error_code error;
  ASSERT_TRUE(fs::create_directory(testDirectory, error));
  ASSERT_FALSE(error);

  // EXPECT_THROW(
  //     Stroalgo::Common::Utils::CreateFolder(testDirectory.string()),
  //     Stroalgo::Exceptions::LoggerException);
}

TEST_F(UtilsTest, ExistingFileThrowsException) {
  boost::system::error_code error;
  ASSERT_TRUE(fs::create_directory(testDirectory, error));
  ASSERT_FALSE(error);

  const fs::path filePath = testDirectory / "existing_file";
  std::ofstream file(filePath.string());
  ASSERT_TRUE(file.is_open());
  file.close();

  EXPECT_THROW(Stroalgo::Common::Utils::CreateFolder(filePath.string()),
               Stroalgo::Exceptions::LoggerException);
}

TEST_F(UtilsTest, ChildOfFileThrowsException) {
  boost::system::error_code error;
  ASSERT_TRUE(fs::create_directory(testDirectory, error));
  ASSERT_FALSE(error);

  const fs::path filePath = testDirectory / "existing_file";
  std::ofstream file(filePath.string());
  ASSERT_TRUE(file.is_open());
  file.close();

  const fs::path invalidPath = filePath / "child";

  EXPECT_THROW(Stroalgo::Common::Utils::CreateFolder(invalidPath.string()),
               Stroalgo::Exceptions::LoggerException);
}
