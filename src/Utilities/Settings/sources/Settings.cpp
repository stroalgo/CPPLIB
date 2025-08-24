/**
 * @file Logger.cpp
 * @brief A SettingManager class
 * @details Uses Boost
 * @copyright   Copyright (c) 2025 stroalgo.corp
 */

#include "Settings.h"

#include <boost/property_tree/ini_parser.hpp>
#include <filesystem>

#include "Exceptions.h"

namespace Utilities::Settings {

void Settings::CreateDefaultSettingsFile() {
  // Create default settings
  // Default log path is LOGS/
  boost::property_tree::ptree lSettingsTree{};
  lSettingsTree.put<std::string>("Logger.LogPath",
                                 m_LoggerSettings.m_SettingLogPath);

  // Default log level is trace
  lSettingsTree.put<boost::log::trivial::severity_level>(
      "Logger.LogLevel", m_LoggerSettings.m_SettingLogLevel);

  // Save default settings to file
  boost::property_tree::ini_parser::write_ini(std::string(m_SettingsFilePath),
                                              lSettingsTree);

  // Flag to indicate settings loaded successfully
  m_SettingsLoaded = true;
}

void Settings::LoadSettings() {
  try {
    // Load settings from file
    boost::property_tree::ptree lSettingsTree{};
    boost::property_tree::ini_parser::read_ini(std::string(m_SettingsFilePath),
                                               lSettingsTree);

    // Check and Populate LogPath of LoggerSettings struct
    const std::string& lSettingPath{lSettingsTree.get<std::string>(
        "Logger.LogPath", m_LoggerSettings.m_SettingLogPath)};
    if (lSettingPath.empty())  //|| lSettingPath == " " || lSettingPath == "\n"
                               //||    lSettingPath == "\t")
    {
      throw Utilities::Exceptions::LoggerException(
          "Logger.LogPath setting is empty in settings.ini file");
    } else {
      m_LoggerSettings.m_SettingLogPath = lSettingPath;
    }

    const auto& lSettingLevel{
        lSettingsTree.get<boost::log::trivial::severity_level>(
            "Logger.LogLevel", m_LoggerSettings.m_SettingLogLevel)};

    // Check and Populate LogLevel of LoggerSettings struct
    if (lSettingLevel < boost::log::trivial::trace ||
        lSettingLevel > boost::log::trivial::fatal) {
      throw Utilities::Exceptions::LoggerException(
          "Logger.LogLevel setting is invalid in settings.ini file");
    } else {
      m_LoggerSettings.m_SettingLogLevel = lSettingLevel;
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
