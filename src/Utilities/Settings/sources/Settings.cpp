/**
 * @file Logger.cpp
 * @brief A SettingManager class
 * @details Uses Boost
 * @copyright   Copyright (c) 2025 stroalgo.corp
 */

#include "Settings.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/regex.hpp>
#include <filesystem>

#include "Constants.h"
#include "Exceptions.h"

namespace Utilities::Settings {

bool Settings::IsModuleSettingsLoaded(const std::string& pModuleName) {
  bool lRet{};
  auto lIt = m_ModulesSettings.find(pModuleName);
  if (lIt != m_ModulesSettings.end()) {
    lRet = true;
  } else {
    lRet = false;
  }
  return lRet;
}

const boost::log::trivial::severity_level& Settings::GetSettingModuleLogLevel(
    const std::string& pModuleName) {
  auto lIt = m_ModulesSettings.find(pModuleName);
  if (lIt != m_ModulesSettings.end()) {
    return lIt->second.m_ModuleLogLevel;
  } else {
    // throw Utilities::Exceptions::ModuleSettingsNotFound(
    throw Utilities::Exceptions::LoggerException(
        "Module settings not found for module: " + pModuleName);
  }
}
void Settings::CreateDefaultSettingsFile() {
  // Create default settings
  // Default log path is LOGS/
  boost::property_tree::ptree lSettingsTree{};
  m_LoggerSettings.m_SettingLogPath = "LOGS";
  lSettingsTree.put<std::string>("Logger.LogPath",
                                 m_LoggerSettings.m_SettingLogPath);
  CreateLogsFolder(m_LoggerSettings.m_SettingLogPath);

  // Default log level is trace
  m_LoggerSettings.m_SettingLogLevel = boost::log::trivial::trace;
  lSettingsTree.put<boost::log::trivial::severity_level>(
      "Logger.LogLevel", boost::log::trivial::trace);

  // Default modules settings
  m_ModulesSettings.clear();
  for (const auto& lModule : Constants::c_ModuleNames) {
    ModuleSettings lModuleSettings{};
    lModuleSettings.m_ModuleName = lModule;
    lModuleSettings.m_ModuleLogLevel = boost::log::trivial::trace;
    m_ModulesSettings[std::string(lModule)] = lModuleSettings;
  }

  // Save default settings to file
  boost::property_tree::ini_parser::write_ini(std::string(m_SettingsFilePath),
                                              lSettingsTree);

  // Flag to indicate settings loaded successfully
  m_SettingsLoaded = true;
}

void Settings::CreateLogsFolder(const std::string& pLogsPath) {
  if (!std::filesystem::exists(pLogsPath)) {
    std::filesystem::create_directories(pLogsPath);
  }
}

void Settings::LoadSettings() {
  try {
    // Load settings from file
    boost::property_tree::ptree lSettingsTree{};
    boost::property_tree::ini_parser::read_ini(std::string(m_SettingsFilePath),
                                               lSettingsTree);

    // Check and Populate LogPath of LoggerSettings struct
    const std::string& lSettingPath{
        lSettingsTree.get<std::string>("Logger.LogPath")};

    CreateLogsFolder(lSettingPath);
    m_LoggerSettings.m_SettingLogPath =
        std::filesystem::path(lSettingPath).make_preferred().string();

    const auto& lSettingLevel{
        lSettingsTree.get<boost::log::trivial::severity_level>(
            "Logger.LogLevel")};

    // Populate LogLevel of LoggerSettings struct
    m_LoggerSettings.m_SettingLogLevel = lSettingLevel;

    // Check and Populate every module settings
    auto modulesSection = lSettingsTree.get_child("Modules");
    m_ModulesSettings.clear();
    const boost::regex special_char_regex("[^a-zA-Z0-9_]");
    for (const auto& module : modulesSection) {
      const std::string& lModuleName{module.first};

      // Validate ModuleName
      if (boost::regex_search(lModuleName, special_char_regex)) {
        throw Exceptions::LoggerException(
            lModuleName +
            "Module name ill formatted");  // Skip empty module names
      } else {
        const boost::log::trivial::severity_level lModuleLogLevel{
            module.second.get_value<boost::log::trivial::severity_level>()};

        ModuleSettings lModuleSettings{};
        lModuleSettings.m_ModuleName = lModuleName;
        lModuleSettings.m_ModuleLogLevel = lModuleLogLevel;
        m_ModulesSettings.emplace(lModuleSettings.m_ModuleName,
                                  lModuleSettings);
      }
    }

    // Flag to indicate settings loaded successfully
    m_SettingsLoaded = true;
  } catch (...) {
    // In case of any error (file not found, parse error, etc...) create a
    // default settings file
    CreateDefaultSettingsFile();
  }
}
}  // namespace Utilities::Settings
