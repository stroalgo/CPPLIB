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

#include "Constants.h"

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

  void CreateMockSettingsFile(
      const std::string_view pLogsPathFolder = "",
      const std::string_view pLogsLevel = "",
      const std::map<std::string, std::string> &pModules = {},
      const std::map<std::string, std::string> &pServer = {}) {
    std::ofstream lSettingsFile("settings.ini");
    if (lSettingsFile.is_open()) {
      lSettingsFile << "[Logger]" << std::endl;
      lSettingsFile << "LogPath=" << pLogsPathFolder << std::endl;
      lSettingsFile << "LogLevel=" << pLogsLevel << std::endl;

      if (!pModules.empty()) {
        lSettingsFile << "[Modules]" << std::endl;
        for (const auto &lModule : pModules) {
          lSettingsFile << lModule.first << "=" << lModule.second << std::endl;
        }
      }

      if (!pServer.empty()) {
        lSettingsFile << "[Server]" << std::endl;
        for (const auto &lSetting : pServer) {
          lSettingsFile << lSetting.first << "=" << lSetting.second
                        << std::endl;
        }
      }

      lSettingsFile.close();
    } else {
      std::cout << "Unable to create settings.ini file \n";
    }
  };
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
  // Default modules must be loaded
  for (const auto &lModule : Utilities::Constants::c_ModuleNames) {
    EXPECT_TRUE(Utilities::Settings::SettingsManager::GetInstance()
                    .IsModuleSettingsLoaded(std::string(lModule)));
    EXPECT_EQ(Utilities::Settings::SettingsManager::GetInstance()
                  .GetSettingModuleLogLevel(std::string(lModule)),
              boost::log::trivial::trace);
  }
  // Default server port must be loaded
  EXPECT_EQ(Utilities::Settings::SettingsManager::GetInstance()
                .GetSettingsServerPort(),
            Utilities::Constants::c_DefaultServerPort);
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

TEST_F(SettingsManagerTest, LoadSettings_FileExists_LogPathInvalid) {
  // Create an settings.ini file
  CreateMockSettingsFile("/**inv@lid-log_path++%");
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
  const std::string &lLogsPathFolder{
      std::filesystem::path("LOGS/path/logs/PATH/Valid")
          .make_preferred()
          .string()};
  std::map<std::string, std::string> pModules{
      {"Module_Library", "warning"},
  };
  std::map<std::string, std::string> pServer{
      {"Port", "9313"},
  };
  CreateMockSettingsFile(lLogsPathFolder, "info", pModules, pServer);

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath(),
      lLogsPathFolder);

  // Log directory created with the given path
  EXPECT_TRUE(std::filesystem::exists(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogPath()));
}

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
  std::map<std::string, std::string> lModules{
      {"Module_Library", "warning"},
  };
  std::map<std::string, std::string> pServer{
      {"Port", "9313"},
  };

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "trace", lModules,
                         pServer);
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "debug", lModules,
                         pServer);
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::debug);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "info", lModules,
                         pServer);
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::info);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "warning", lModules,
                         pServer);
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::warning);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "error", lModules,
                         pServer);
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::error);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "fatal", lModules,
                         pServer);
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::fatal);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_LogLevelCaseSensitive) {
  // Create an settings.ini file
  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "TrAcE");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "DeBuG");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "InFo");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "WaRnInG");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "ErRoR");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "FaTaL");
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();
  EXPECT_EQ(
      Utilities::Settings::SettingsManager::GetInstance().GetSettingLogLevel(),
      boost::log::trivial::trace);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_ModulesEmpty) {
  // Create an settings.ini file
  CreateMockSettingsFile();
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // Default modules must be loaded
  for (const auto &lModule : Utilities::Constants::c_ModuleNames) {
    EXPECT_TRUE(Utilities::Settings::SettingsManager::GetInstance()
                    .IsModuleSettingsLoaded(std::string(lModule)));
    EXPECT_EQ(Utilities::Settings::SettingsManager::GetInstance()
                  .GetSettingModuleLogLevel(std::string(lModule)),
              boost::log::trivial::trace);
  }
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_NewModule_NameEmpty) {
  // Create an settings.ini file
  std::map<std::string, std::string> pModules{{"", "info"}};

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "info", pModules);
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // A module with empty name will not be loaded
  EXPECT_FALSE(Utilities::Settings::SettingsManager::GetInstance()
                   .IsModuleSettingsLoaded(""));

  EXPECT_THROW(Utilities::Settings::SettingsManager::GetInstance()
                   .GetSettingModuleLogLevel(""),
               std::exception);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_ModuleNew_NameInvalid) {
  // Create an settings.ini file
  std::map<std::string, std::string> pModules{{"@ill_$formatted", "info"}};

  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "info", pModules);
  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // A module with empty name will not be loaded with invalid log level
  EXPECT_FALSE(Utilities::Settings::SettingsManager::GetInstance()
                   .IsModuleSettingsLoaded("@ill_$formatted"));

  EXPECT_THROW(Utilities::Settings::SettingsManager::GetInstance()
                   .GetSettingModuleLogLevel("@ill_$formatted"),
               std::exception);
}

TEST_F(SettingsManagerTest,
       LoadSettings_FileExists_ModuleNew_NameValid_LogLevelInvalid) {
  // Create an settings.ini file
  std::map<std::string, std::string> pModules{
      {"Module_Library", "/!@#invalid-log_level$$%"},
  };
  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "info", pModules);

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // A non default module will not be loaded with invalid log level
  EXPECT_FALSE(Utilities::Settings::SettingsManager::GetInstance()
                   .IsModuleSettingsLoaded("Module_Library"));

  EXPECT_THROW(Utilities::Settings::SettingsManager::GetInstance()
                   .GetSettingModuleLogLevel("Module_Library"),
               std::exception);
}

TEST_F(SettingsManagerTest,
       LoadSettings_FileExists_ModuleDefault_NameValid_LogLevelInvalid) {
  // Create an settings.ini file
  std::map<std::string, std::string> pModules{
      {std::string(Utilities::Constants::c_LoggerModuleName),
       "/!@#invalid-log_level$$%"},
  };
  CreateMockSettingsFile("LOGS/path/logs/PATH/Valid", "info", pModules);

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // A  default module will not be loaded with invalid log level
  EXPECT_TRUE(Utilities::Settings::SettingsManager::GetInstance()
                  .IsModuleSettingsLoaded(
                      std::string(Utilities::Constants::c_LoggerModuleName)));

  EXPECT_EQ(Utilities::Settings::SettingsManager::GetInstance()
                .GetSettingModuleLogLevel(
                    std::string(Utilities::Constants::c_LoggerModuleName)),
            boost::log::trivial::trace);
}

TEST_F(SettingsManagerTest,
       LoadSettings_FileExists_ModuleNew_NameValid_LogLevelvalid) {
  // Create an settings.ini file
  std::map<std::string, std::string> pModules{
      {"Module_Library", "warning"},
  };

  std::map<std::string, std::string> pServer{
      {"Port", "9313"},
  };
  const std::string &lLogsPathFolder{
      std::filesystem::path("LOGS/path/logs/PATH/Valid")
          .make_preferred()
          .string()};
  CreateMockSettingsFile(lLogsPathFolder, "info", pModules, pServer);

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // A non default module will  be loaded with  log level set to default log
  // level
  EXPECT_TRUE(Utilities::Settings::SettingsManager::GetInstance()
                  .IsModuleSettingsLoaded("Module_Library"));
  EXPECT_EQ(Utilities::Settings::SettingsManager::GetInstance()
                .GetSettingModuleLogLevel("Module_Library"),
            boost::log::trivial::warning);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_ServerPortKeyEmpty) {
  // Create an settings.ini file
  std::map<std::string, std::string> pModules{
      {"Module_Library", "warning"},
  };
  std::map<std::string, std::string> pServer{
      {"", "8080"},
  };
  const std::string &lLogsPathFolder{
      std::filesystem::path("LOGS/path/logs/PATH/Valid")
          .make_preferred()
          .string()};
  CreateMockSettingsFile(lLogsPathFolder, "info", pModules, pServer);

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // A non default module will  be loaded with  log level set to default log
  // level
  EXPECT_EQ(Utilities::Settings::SettingsManager::GetInstance()
                .GetSettingsServerPort(),
            Utilities::Constants::c_DefaultServerPort);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_ServerPortInValid) {
  // Create an settings.ini file
  std::map<std::string, std::string> pModules{
      {"Module_Library", "warning"},
  };
  std::map<std::string, std::string> pServer{
      {"Port", "@invalid_9313_port"},
  };
  const std::string &lLogsPathFolder{
      std::filesystem::path("LOGS/path/logs/PATH/Valid")
          .make_preferred()
          .string()};
  CreateMockSettingsFile(lLogsPathFolder, "info", pModules, pServer);

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // A non default module will  be loaded with  log level set to default log
  // level
  EXPECT_EQ(Utilities::Settings::SettingsManager::GetInstance()
                .GetSettingsServerPort(),
            Utilities::Constants::c_DefaultServerPort);

  std::map<std::string, std::string> pServer_1{
      {"Port", "1023"},
  };

  CreateMockSettingsFile(lLogsPathFolder, "info", pModules, pServer_1);

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // A non default module will  be loaded with  log level set to default log
  // level
  EXPECT_EQ(Utilities::Settings::SettingsManager::GetInstance()
                .GetSettingsServerPort(),
            Utilities::Constants::c_DefaultServerPort);

  std::map<std::string, std::string> pServer_2{
      {"Port", "65536"},
  };

  CreateMockSettingsFile(lLogsPathFolder, "info", pModules, pServer_2);

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // A non default module will  be loaded with  log level set to default log
  // level
  EXPECT_EQ(Utilities::Settings::SettingsManager::GetInstance()
                .GetSettingsServerPort(),
            Utilities::Constants::c_DefaultServerPort);
}

TEST_F(SettingsManagerTest, LoadSettings_FileExists_ServerPortValid) {
  // Create an settings.ini file
  std::map<std::string, std::string> pModules{
      {"Module_Library", "warning"},
  };
  std::map<std::string, std::string> pServer{
      {"Port", "9313"},
  };
  const std::string &lLogsPathFolder{
      std::filesystem::path("LOGS/path/logs/PATH/Valid")
          .make_preferred()
          .string()};
  CreateMockSettingsFile(lLogsPathFolder, "info", pModules, pServer);

  Utilities::Settings::SettingsManager::GetInstance().LoadSettings();

  // A non default module will  be loaded with  log level set to default log
  // level
  EXPECT_EQ(Utilities::Settings::SettingsManager::GetInstance()
                .GetSettingsServerPort(),
            stoi(pServer.at("Port")));
}
