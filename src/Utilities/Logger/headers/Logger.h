/**
 * @file        Logger.h
 * @author      ALLOGHO
 * @brief       A logger class to write every event action
 * @details     Uses spdlog library to speed up logging
 * @version     1.0
 * @date        2025-02-19
 * @copyright   Copyright (c) 2025 stroalgo.corp
 *
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "Constants.h"
#include "GenericSingleton.h"

#include <format>
#include <map>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <string>
namespace Utilities::Log {

/**
 * @brief
 *
 */
class Logger : public Utilities::Common::GenericSingleton<Logger> {
public:
  /**
   * @brief
   *
   */
  // TODO : if not nessecary remove it
  enum class LogLevel { Trace = 0, Debug, Info, Warn, Error, Fatal };

  /**
   * @brief Destroy the Logger object
   *
   */
  virtual ~Logger() = default;

  /**
   * @brief Shutdown the logger
   *
   */
  void ShutDown();

  /**
   * @brief Register a logger for a moduleor library
   *
   * @param pModuleName Name of the module or library to register
   */
  void RegisterModule(const std::string &pModuleName);

  /**
   * @brief Set the Module Log Level
   *
   * @param pModuleName Name of the module or library
   * @param pLogLevel Log level for the module or library
   */
  void SetModuleLogLevel(const std::string &pModuleName, LogLevel pLogLevel);

  /**
   * @brief Write a trace message
   *
   * @tparam Args Type
   * @param pModuleName Module name concerned
   * @param pFormat Message format
   * @param pArgs Messages
   */
  template <typename... Args>
  inline void Trace(const std::string &pModuleName,
                    const spdlog::format_string_t<Args...> pFormat,
                    Args &&...pArgs) {

    WriteLog(spdlog::level::trace, pModuleName, pFormat,
             std::forward<Args>(pArgs)...);
  };

  friend class Utilities::Common::GenericSingleton<Logger>;

private:
  /**
   * @brief Construct a new Logger object
   *
   */
  Logger();

  /**
   * @brief
   *
   * @tparam Args
   * @param pLogLevel
   * @param pModuleName
   * @param pFormat
   * @param pArgs
   */
  template <typename... Args>
  inline void WriteLog(const spdlog::level::level_enum &pLogLevel,
                       const std::string &pModuleName,
                       const spdlog::format_string_t<Args...> &pFormat,
                       Args &&...pArgs) {

    // Find the logger related to module
    auto lLogger = m_Loggers->find(pModuleName);

    // Write log if Module is registered
    if (lLogger != m_Loggers->end()) {
      if (lLogger->second != nullptr) {
        lLogger->second->log(pLogLevel, pFormat, std::forward<Args>(pArgs)...);
      } else {
        HandleWriteFailure(
            pModuleName, "Unable to write Log : Logger for Module {} is null");
      }
    } else {

      HandleWriteFailure(pModuleName,
                         "Unable to write Log : Module {} does not exist");
    }
  };

  /**
   * @brief
   *
   * @tparam Args
   * @param pModuleName
   * @param pFormat
   */
  template <typename... Args>
  void HandleWriteFailure(const std::string &pModuleName,
                          const spdlog::format_string_t<Args...> &pFormat) {
    auto lLogger =
        m_Loggers->find(std::string(Utilities::Constants::c_LoggerModuleName));

    if (lLogger != m_Loggers->end()) {
      if (lLogger->second != nullptr) {
        lLogger->second->error(pFormat, pModuleName);
      } else {
        spdlog::critical(
            "The logger for {} module is null : Logs are not saved into files",
            Utilities::Constants::c_LoggerModuleName);
      }
    } else {
      spdlog::critical(
          "The {} module is not registered : Logs are not saved into files",
          Utilities::Constants::c_LoggerModuleName);
    }
  }

  /**
   * @brief Registered loggers container
   * @private
   * @memberof Logger
   */
  std::unique_ptr<std::map<std::string, std::shared_ptr<spdlog::logger>>>
      m_Loggers = std::make_unique<
          std::map<std::string, std::shared_ptr<spdlog::logger>>>();
};

} // namespace Utilities::Log
#endif
