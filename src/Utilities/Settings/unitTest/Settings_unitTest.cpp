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
  }

 public:
  void CreateEmptySettingsFile() {
    std::ofstream lSettingsFile("settings.ini");
    lSettingsFile.close();
  }

  void CreateMockSettingsFile(std::string_view pLogsPathFolder = "") {
    std::ofstream lSettingsFile("settings.ini");
    if (lSettingsFile.is_open()) {
      lSettingsFile << "[Logger]" << std::endl;
      lSettingsFile << "LogPath=" << pLogsPathFolder << std::endl;
      lSettingsFile << "LogLevel=info" << std::endl;
      lSettingsFile.close();
    } else {
      std::cout << "Unable to create settings.ini file \n";
    }
  }
};

TEST_F(SettingsManagerTest, Instantiate_nothrow) {
  EXPECT_NO_THROW(Utilities::Settings::SettingsManager::GetInstance());
}

TEST_F(SettingsManagerTest, GetSettingsFilePath) {
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingsFilePath(),
      "settings.ini");
}

TEST_F(SettingsManagerTest, LoadSettings_noFileExists) {
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // In case of no file found the LoadSettings function must create settings.ini
  // file with default settings
  std::string_view lSettingsFilePath =
      Utilities::Settings::SettingsManager::GetInstance().GetSettingsFilePath();
  EXPECT_TRUE(std::filesystem::exists(lSettingsFilePath));

  constexpr std::string_view lLogsPathFolder = "LOGS/";
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath(),
      lLogsPathFolder);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExistsEmpty) {
  // Create an empty settings.ini file
  CreateEmptySettingsFile();
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  std::string_view lSettingsFilePath =
      Utilities::Settings::SettingsManager::GetInstance().GetSettingsFilePath();
  EXPECT_TRUE(std::filesystem::exists(lSettingsFilePath));

  constexpr std::string_view lLogsPathFolder = "LOGS/";
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath(),
      lLogsPathFolder);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_LogPathEmpty) {
  // Create an settings.ini file
  CreateMockSettingsFile();
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  constexpr std::string_view lLogsPathFolder = "LOGS/";
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath(),
      lLogsPathFolder);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_LogPathExists) {
  // Create an settings.ini file
  constexpr std::string_view lLogsPathFolder = "LOGS/path/logs/PATH";
  CreateMockSettingsFile(lLogsPathFolder);
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath(),
      lLogsPathFolder);
}

TEST_F(SettingsManagerTest, LoadSettings_successfully) {
  EXPECT_FALSE(
      Utilities::Settings::SettingsManager::GetInstance().AreSettingsLoaded());

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  EXPECT_TRUE(
      Utilities::Settings::SettingsManager::GetInstance().AreSettingsLoaded());
}
