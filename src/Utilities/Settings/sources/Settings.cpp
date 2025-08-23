/**
 * @file Logger.cpp
 * @brief A SettingManager class
 * @details Uses Boost
 * @copyright   Copyright (c) 2025 stroalgo.corp
 */

#include "Settings.h"

#include <boost/property_tree/ini_parser.hpp>
#include <filesystem>

namespace Utilities::Settings {

const std::string_view Settings::GetSettingLogPath() {
  return m_LoggerSettings.m_SettingLogPath;
}

void Settings::CreateDefaultSettingsFile() {
  // Create default settings
  boost::property_tree::ptree lSettingsTree{};
  lSettingsTree.put("Logger.LogPath", m_LoggerSettings.m_SettingLogPath);

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

    // Populate Logger settings struct
    m_LoggerSettings.m_SettingLogPath =
        std::move(lSettingsTree.get<std::string>(
            "Logger.LogPath", m_LoggerSettings.m_SettingLogPath));

    // Flag to indicate settings loaded successfully
    m_SettingsLoaded = true;
  } catch (...) {
    CreateDefaultSettingsFile();
  }
}
}  // namespace Utilities::Settings
