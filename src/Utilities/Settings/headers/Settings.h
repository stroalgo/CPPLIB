/**
 * @file        Settings.h
 * @author      ALLOGHO
 * @brief       A Settings class to manage every entries that will allow app to
 * run
 * @details     Uses boost library to speed up logging
 * @version     1.0
 * @date        2025-07-18
 * @copyright   Copyright (c) 2025 stroalgo.corp
 *
 */

#ifndef UTILITIES_SETTINGS_HEADERS_SETTINGS_H_
#define UTILITIES_SETTINGS_HEADERS_SETTINGS_H_

#include <boost/log/trivial.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cstdint>
#include <map>
#include <string>
#include <string_view>

#include "GenericSingleton.h"

namespace Utilities::Settings {

/**
 * @brief Class to represent every settings that will be used by the application
 * @class Settings
 */
class Settings {
 public:
  /**
   * @brief Destroy the Settings Manager object
   * @memberof Settings
   * @public
   */
  virtual ~Settings() = default;

  /**
   * @brief Construct a new Settings Manager object
   * @memberof Settings
   * @public
   */
  Settings() = default;

  /**
   * @brief Get the Settings File Path object
   * @memberof Settings
   * @return const std::string&
   */
  inline const std::string_view GetSettingsFilePath() const {
    return m_SettingsFilePath;
  };

  /**
   * @brief Get the Log Path object
   * @memberof Settings
   * @return const std::string_view&
   * @public
   */
  inline const std::string_view GetSettingLogPath() const {
    return m_LoggerSettings.m_SettingLogPath;
  };

  /**
   * @brief Get the Log Level
   * @memberof Settings
   * @return const std::string_view&
   * @public
   */
  inline const boost::log::trivial::severity_level& GetSettingLogLevel() const {
    return m_LoggerSettings.m_SettingLogLevel;
  };

  /**
   * @brief Get the Module Log Level
   * @memberof Settings
   * @param pModuleName Name of the module
   * @return The level of module logger
   */
  const boost::log::trivial::severity_level& GetSettingModuleLogLevel(
      const std::string& pModuleName);

  /**
   * @brief Get the Settings Server Port object
   * @memberof Settings
   * @return int
   */
  inline std::uint16_t GetSettingsServerPort() const {
    return m_ServerSettings.m_ServerPort;
  };

  /**
   * @brief Check if module settings exist
   * @param pModuleName Name of the module
   * @return true if module settings exist, false otherwise
   */
  bool IsModuleSettingsLoaded(const std::string& pModuleName);

  /**
   * @brief Load settings from file
   * @memberof Settings
   * @public
   */
  void LoadSettings();

  /**
   * @brief Check if settings have been loaded successfully
   * @memberof Settings
   * @return true if settings have been loaded successfully, false otherwise
   * @public
   */
  inline bool AreSettingsLoaded() const { return m_SettingsLoaded; }

  // TODO(stroalgo) :Future implementations
  //   void SaveSettings();
  //   void ResetSettings();
  //   void PrintSettings();
  //   void SetSetting(const std::string &id, const std::string &value);
  //   bool HasSetting(const std::string &id);
  //   void RemoveSetting(const std::string &id);
  //   void ClearSettings();
 private:
  /**
   * @brief Struct to hold every settings related to logger
   * @memberof Settings
   * @struct LoggerSettings
   * @private
   */
  struct LoggerSettings {
    std::string m_SettingLogPath{"LOGS"};
    boost::log::trivial::severity_level m_SettingLogLevel{
        boost::log::trivial::trace};
  } m_LoggerSettings{};

  /**
   * @brief Struct to hold every settings related to a module
   * @memberof Settings
   * @struct ModuleSettings
   * @private
   */
  struct ModuleSettings {
    std::string m_ModuleName{""};
    boost::log::trivial::severity_level m_ModuleLogLevel{
        boost::log::trivial::trace};
  };

  /**
   * @brief Map to hold every module settings
   * @memberof Settings
   * @private
   */
  std::map<const std::string, ModuleSettings> m_ModulesSettings{};

  /**
   * @brief Struct to hold every settings related to server
   * @memberof Settings
   * @struct ServerSettings
   * @private
   */
  struct ServerSettings {
    std::uint16_t m_ServerPort{};
  } m_ServerSettings{};

  /**
   * @brief Create a default settings file
   * @memberof Settings
   * @private
   */
  void CreateDefaultSettingsFile();

  /**
   * @brief Create logs folder if it does not exist
   * @memberof Settings
   * @param pLogsPath The path to the logs folder
   * @private
   */
  void CreateLogsFolder(const std::string& pLogsPath);

  /**
   * @brief Settings file path
   * @memberof SettingsManager
   * @private
   */
  std::string_view m_SettingsFilePath{"settings.ini"};

  /**
   * @brief Flag to indicate if settings have been loaded successfully
   * @memberof SettingsManager
   * @private
   */
  bool m_SettingsLoaded{false};
};

/**
 * @brief Class to manage every settings for the application
 *
 */
class SettingsManager : public Utilities::Common::GenericSingleton<Settings> {
 public:
  /**
   * @brief Allow only the GenericSingleton to get access to constructor
   * @public
   */
  friend class Utilities::Common::GenericSingleton<Settings>;

  /**
   * @brief Destroy the Settings Manager object
   * @memberof SettingsManager
   * @public
   */
  virtual ~SettingsManager() = default;

 private:
  /**
   * @brief Construct a new Settings Manager object
   * @memberof SettingsManager
   * @private
   */
  SettingsManager() = default;
};

}  // namespace Utilities::Settings

#endif  // UTILITIES_SETTINGS_HEADERS_SETTINGS_H_
