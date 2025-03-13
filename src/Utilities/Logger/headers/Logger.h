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
  // TODO : Use setting module to load logs folder path and logger default level
  void RegisterModule(const std::string &pModuleName);

  /**
   * @brief Set the Module Log Level
   *
   * @param pModuleName Name of the module or library
   * @param pLogLevel Log level for the module or library
   */
  void SetModuleLogLevel(const std::string &pModuleName,
                         const spdlog::level::level_enum pLogLevel);

  /**
   * @brief Get the Module Log Level
   *
   * @param pModuleName Name of the module
   * @return The level of module logger
   */
  const std::string GetModuleLevel(const std::string &pModuleName);

  /**
   * @brief Retrieve levels for all registered modules
   *
   * @return A map  containing module as key and level as value
   */
  const std::map<std::string, std::string> GetLogLevels();

  /**
   * @brief Delete all logs for all registered module
   *
   */
  void DeleteAllLogs();

  /**
   * @brief Get Current date as string in a "yyyy-mm-dd" format
   *
   * @return std::string Date as string
   */
  // TODO :  will moved in common modules if needed in many place
  std::string CurrentDateToString();

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
  }

  /**
   * @brief Write a Debug message
   *
   * @tparam Args Type
   * @param pModuleName Module name concerned
   * @param pFormat Message format
   * @param pArgs Messages
   */
  template <typename... Args>
  inline void Debug(const std::string &pModuleName,
                    const spdlog::format_string_t<Args...> pFormat,
                    Args &&...pArgs) {

    WriteLog(spdlog::level::debug, pModuleName, pFormat,
             std::forward<Args>(pArgs)...);
  }

  /**
   * @brief Write a Info message
   *
   * @tparam Args Type
   * @param pModuleName Module name concerned
   * @param pFormat Message format
   * @param pArgs Messages
   */
  template <typename... Args>
  inline void Info(const std::string &pModuleName,
                   const spdlog::format_string_t<Args...> pFormat,
                   Args &&...pArgs) {

    WriteLog(spdlog::level::info, pModuleName, pFormat,
             std::forward<Args>(pArgs)...);
  }

  /**
   * @brief Write a Warning message
   *
   * @tparam Args Type
   * @param pModuleName Module name concerned
   * @param pFormat Message format
   * @param pArgs Messages
   */
  template <typename... Args>
  inline void Warning(const std::string &pModuleName,
                      const spdlog::format_string_t<Args...> pFormat,
                      Args &&...pArgs) {

    WriteLog(spdlog::level::warn, pModuleName, pFormat,
             std::forward<Args>(pArgs)...);
  }

  /**
   * @brief Write a Error message
   *
   * @tparam Args Type
   * @param pModuleName Module name concerned
   * @param pFormat Message format
   * @param pArgs Messages
   */
  template <typename... Args>
  inline void Error(const std::string &pModuleName,
                    const spdlog::format_string_t<Args...> pFormat,
                    Args &&...pArgs) {

    WriteLog(spdlog::level::err, pModuleName, pFormat,
             std::forward<Args>(pArgs)...);
  }

  /**
   * @brief Write a critical message
   *
   * @tparam Args Type
   * @param pModuleName Module name concerned
   * @param pFormat Message format
   * @param pArgs Messages
   */
  template <typename... Args>
  inline void Critical(const std::string &pModuleName,
                       const spdlog::format_string_t<Args...> pFormat,
                       Args &&...pArgs) {

    WriteLog(spdlog::level::critical, pModuleName, pFormat,
             std::forward<Args>(pArgs)...);
  }

  friend class Utilities::Common::GenericSingleton<Logger>;

private:
  /**
   * @brief Construct a new Logger object
   *
   */
  Logger();

  /**
   * @brief Write log in console, in file .txt and .json
   *
   * @tparam Args Type
   * @param pLogLevel the log level
   * @param pModuleName The module name concerned by the log
   * @param pFormat Message format to use
   * @param pArgs Extra args to incorporate
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
        HandleWriteFailure("Unable to write Log : Logger for Module {} is null",
                           pModuleName);
      }
    } else {

      HandleWriteFailure("Unable to write Log : Module {} not registred",
                         pModuleName);
    }
  }

  /**
   * @brief Write any failure using the module LOGGER registred at construction
   *
   * @tparam Args Type
   * @param pModuleName Module Concerned by the failure
   * @param pFormat Message format to use when writting
   */
  template <typename... Args>
  void HandleWriteFailure(const spdlog::format_string_t<Args...> &pFormat,
                          const std::string &pModuleName) {
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
   * @brief Convert spdlog level into string
   *
   * @param pLogLevel The log level
   * @return Log level as string
   */
  const std::string LogLevelTostring(const spdlog::level::level_enum pLogLevel);

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
