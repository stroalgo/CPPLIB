/**
 * @file Logger.cpp
 * @brief A logger class
 * @details Uses spdlog
 */

#include "Logger.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <list>
#include <memory>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>

namespace Utilities::Log {

Logger::Logger() {
  // Register the Logger class itself
  RegisterModule(std::string(Utilities::Constants::c_LoggerModuleName));
}

void Logger::RegisterModule(const std::string &pModuleName) {

  std::string lModuleName = pModuleName;
  boost::algorithm::trim(lModuleName);
  if (lModuleName.empty()) {
    HandleWriteFailure("Module Name can not be an empty string or contain "
                       "whitespace,tab,newline",
                       std::string(Utilities::Constants::c_LoggerModuleName));
  } else if (spdlog::get(pModuleName) == nullptr) {

    // Console LOG
    auto lConsole_sink =
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    lConsole_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] ---> %v%$");

    // File LOG.txt
    std::string lFilename_txt_path{std::string("Logs/") + pModuleName +
                                   std::string("/") + pModuleName +
                                   std::string(".txt")};
    // Create a new Log file at 00:00 and delete it after 31 days
    auto lFile_txt_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
        lFilename_txt_path, 00, 00, false, 31);
    lFile_txt_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] ---> %v%$");

    // File LOG.json
    std::string lFilename_json_path{std::string("Logs/") + pModuleName +
                                    std::string("/") + pModuleName +
                                    std::string(".json")};
    // Create a new Log file at 00:00 and delete it after 31 days
    auto lFile_json_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
        lFilename_json_path, 00, 00, false, 31);
    lFile_json_sink->set_pattern(
        "{\"time\": \"%Y-%m-%d %H:%M:%S.%f%z\", \"name\": \"%n\", \"level\": "
        "\"%^%l%$\", \"process\": %P, \"thread\": %t, \"message\": \"%v\"},");

    spdlog::sinks_init_list lSink_list{lConsole_sink, lFile_txt_sink,
                                       lFile_json_sink};

    // Create Logger
    auto lLog = std::make_shared<spdlog::logger>(
        pModuleName, lSink_list.begin(), lSink_list.end());

    // Save logger to avoid multiple call of sdplog::get
    m_Loggers->try_emplace(pModuleName, lLog);

    // Default Log level
    lLog->set_level(spdlog::level::trace);

    // Min Level log to flush
    lLog->flush_on(spdlog::level::trace);

    // Register Logger to enable retrieve using spdlog::get
    spdlog::register_logger(lLog);
  } else {
    spdlog::get(std::string(Utilities::Constants::c_LoggerModuleName))
        ->warn("Module {} already registered", pModuleName);
  }
}

void Logger::SetModuleLogLevel(const std::string &pModuleName,
                               const spdlog::level::level_enum pLogLevel) {

  // Find the logger related to module
  auto lLogger = m_Loggers->find(pModuleName);

  // Set level if Module is registered
  if (lLogger != m_Loggers->end()) {
    if (lLogger->second != nullptr) {
      lLogger->second->set_level(pLogLevel);
    } else {
      HandleWriteFailure("Unable to set level : Logger for Module {} is null",
                         pModuleName);
    }
  } else {

    HandleWriteFailure("Unable to set level : Module {} is not registred",
                       pModuleName);
  }
}

const std::string Logger::GetModuleLevel(const std::string &pModuleName) {

  std::string lRet{};
  // Find the logger related to module
  auto lLogger = m_Loggers->find(pModuleName);

  // Set level if Module is registered
  if (lLogger != m_Loggers->end()) {
    if (lLogger->second != nullptr) {
      lRet = LogLevelTostring(lLogger->second->level());
    } else {
      HandleWriteFailure("Unable to set level : Logger for Module {} is null",
                         pModuleName);
    }
  } else {

    HandleWriteFailure("Unable to get level : Module {} is not registred",
                       pModuleName);
  }
  return lRet;
}

const std::string
Logger::LogLevelTostring(const spdlog::level::level_enum pLogLevel) {
  std::string lRet{};
  switch (pLogLevel) {
  case spdlog::level::trace:
    lRet = "trace";
    break;
  case spdlog::level::debug:
    lRet = "debug";
    break;
  case spdlog::level::info:
    lRet = "info";
    break;
  case spdlog::level::warn:
    lRet = "warning";
    break;
  case spdlog::level::err:
    lRet = "error";
    break;
  case spdlog::level::critical:
    lRet = "critical";
    break;
  default:
    lRet = "";
    break;
  }
  return lRet;
}

const std::map<std::string, std::string> Logger::GetLogLevels() {
  std::map<std::string, std::string> lRet{};
  std::for_each(m_Loggers->cbegin(), m_Loggers->cend(),
                [&lRet, this](const auto &pLogger) {
                  lRet.try_emplace(pLogger.first,
                                   LogLevelTostring(pLogger.second->level()));
                });
  return lRet;
}

void Logger::ShutDown() {
  spdlog::drop_all();
  spdlog::shutdown();
}

void Logger::DeleteAllLogs() {
  std::for_each(m_Loggers->cbegin(), m_Loggers->cend(),
                [this](const auto &pLogger) { DeleteLogs(pLogger.first); });
}

void Logger::DeleteAllModuleLogs(const std::string &pModuleName) {

  // Find the logger related to module
  auto lLogger = m_Loggers->find(pModuleName);

  // Set level if Module is registered
  if (lLogger != m_Loggers->end()) {
    DeleteLogs(lLogger->first);
  } else {
    HandleWriteFailure("Unable to delete logs : Module {} is not registred",
                       pModuleName);
    throw Utilities::Exceptions::LoggerException(fmt::format(
        "Unable to delete logs : Module {} is not registred", pModuleName));
  }
} // namespace Utilities::Log

std::string Logger::CurrentDateToString() {
  using namespace boost::gregorian;

  // Get the current date
  date d = day_clock::local_day();
  std::unique_ptr<date_facet> df = std::make_unique<date_facet>("%Y-%m-%d");

  // Apply format
  std::stringstream ss;
  ss.imbue(std::locale{ss.getloc(), df.get()});
  ss << d;
  return ss.str();
}

void Logger::DeleteLogs(const std::string &pModuleName) {

  std::list<std::string> lListOfPath{};
  for (const auto &lFilePath :
       std::filesystem::directory_iterator{"Logs/" + pModuleName}) {
    lListOfPath.push_back(lFilePath.path().string());
  }

  // Clear the content of current used logfile
  std::stringstream lLogFilePath{};
  lLogFilePath << "Logs/" << pModuleName << "/" << pModuleName << "_"
               << CurrentDateToString() << ".txt";
  lListOfPath.remove(lLogFilePath.str());
  std::fstream lFileStreamTxt{};
  lFileStreamTxt.open(lLogFilePath.str(),
                      std::ofstream::out | std::ofstream::trunc);
  lFileStreamTxt.close();
  lLogFilePath.str("");
  lLogFilePath << "Logs/" << pModuleName << "/" << pModuleName << "_"
               << CurrentDateToString() << ".json";
  lListOfPath.remove(lLogFilePath.str());
  std::fstream lFileStreamJson{};
  lFileStreamJson.open(lLogFilePath.str(),
                       std::ofstream::out | std::ofstream::trunc);
  lFileStreamJson.close();

  // Delete all logs files exept the current log file
  for (const auto &lFilePath : lListOfPath) {
    std::filesystem::remove(lFilePath);
  }
}

} // namespace Utilities::Log