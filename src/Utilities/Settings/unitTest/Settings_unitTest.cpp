/**
 * @file Settings_unitTest.cpp
 * @brief Contains all units tests for the SettingManager class
 * @copyright   Copyright (c) 2025 stroalgo.corp
 */

#include "Settings.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>

class SettingsManagerTest : public ::testing::Test {
 protected:
  void TearDown() override {
    if (std::filesystem::exists("settings.ini")) {
      std::filesystem::remove_all("settings.ini");
    }
    if (std::filesystem::exists("LOGS")) {
      std::filesystem::remove_all("LOGS");
    }
  }

 public:
  void CreateEmptySettingsFile() {
    std::ofstream lSettingsFile("settings.ini");
    lSettingsFile.close();
  }

  void CreateMockSettingsFile(const std::string_view pLogsPathFolder = "",
                              const std::string_view pLogsLevel = "") {
    std::ofstream lSettingsFile("settings.ini");
    if (lSettingsFile.is_open()) {
      lSettingsFile << "[Logger]" << std::endl;
      lSettingsFile << "LogPath=" << pLogsPathFolder << std::endl;
      lSettingsFile << "LogLevel=" << pLogsLevel << std::endl;
      lSettingsFile.close();
    } else {
      std::cout << "Unable to create settings.ini file \n";
    }
  }
};

TEST_F(SettingsManagerTest, Instantiate_nothrow) {
  EXPECT_NO_THROW(Utilities::Settings::SettingsManager::GetInstance());
}

TEST_F(SettingsManagerTest, SingletonInstance_sameInstance) {
  Utilities::Settings::Settings &lInstance1 =
      Utilities::Settings::SettingsManager::GetInstance();
  Utilities::Settings::Settings &lInstance2 =
      Utilities::Settings::SettingsManager::GetInstance();

  EXPECT_EQ(&lInstance1, &lInstance2);
}

TEST_F(SettingsManagerTest, GetSettingsFilePath) {
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingsFilePath(),
      "settings.ini");
}

TEST_F(SettingsManagerTest, LoadSettings_noFileExists) {
  EXPECT_FALSE(
      Utilities::Settings::SettingsManager::GetInstance().AreSettingsLoaded());

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  EXPECT_TRUE(
      Utilities::Settings::SettingsManager::GetInstance().AreSettingsLoaded());

  // In case of no file found the LoadSettings function must create
  // settings.ini file with default settings
  std::string_view lSettingsFilePath =
      Utilities::Settings::SettingsManager::GetInstance().GetSettingsFilePath();
  EXPECT_TRUE(std::filesystem::exists(lSettingsFilePath));

  // Default log path is LOGS
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath(),
      "LOGS");
  // Default log level is trace
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);
}
TEST_F(SettingsManagerTest, LoadSettings_FileExists_LogPathEmpty) {
  // Create an settings.ini file
  CreateMockSettingsFile();
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // Default log path is LOGS
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath(),
      "LOGS");

  // Log directory must be created
  EXPECT_TRUE(std::filesystem::exists(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath()));
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_LogPathExists) {
  // Create an settings.ini file
  const std::string_view lLogsPathFolder{
      std::filesystem::path("LOGS/path/logs/PATH/Valid").c_str()};
  CreateMockSettingsFile(lLogsPathFolder);
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath(),
      lLogsPathFolder);

  // Log directory created with the given path
  EXPECT_TRUE(std::filesystem::exists(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath()));
}

// TEST_F(SettingsManagerTest, LoadSettings_FileExists_LogPathInvalid) {
//   EXPECT_FALSE(
//       Utilities::Settings::SettingsManager::GetInstance().AreSettingsLoaded());
// }

TEST_F(SettingsManagerTest, LoadSettings_FileExists_LogLevelEmpty) {
  // Create an settings.ini file
  CreateMockSettingsFile();
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_LogLevelInvalid) {
  // Create an settings.ini file
  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid",
                         "/**inv@lid-log_level++%");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_LogLevelValid) {
  // Create an settings.ini file
  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "trace");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "debug");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::debug);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "info");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::info);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "warning");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::warning);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "error");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::error);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "fatal");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::fatal);
}
