/**
 * @file Logger.cpp
 * @brief A logger class
 * @details Uses spdlog
 */

#include "Logger.h"

#include <format>
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

  if (spdlog::get(pModuleName) == nullptr) {

    // Console LOG
    auto lConsole_sink =
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    lConsole_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] ---> %v%$");

    // File LOG.txt
    std::string lFilename_txt_path{std::string("Logs/") + pModuleName +
                                   std::string(".txt")};
    // Create a new Log file at 00:00 and delete it after 31 days
    auto lFile_txt_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
        lFilename_txt_path, 00, 00, false, 31);
    lFile_txt_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] ---> %v%$");

    // File LOG.json
    std::string lFilename_json_path{std::string("Logs/") + pModuleName +
                                    std::string(".json")};
    // Create a new Log file at 00:00 and delete it after 31 days
    auto lFile_json_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
        lFilename_json_path, 00, 00, false, 31);
    lFile_json_sink->set_pattern(
        "{\"time\": \"%Y-%m-%dT%H:%M:%S.%f%z\", \"name\": \"%n\", \"level\": "
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

void Logger::ShutDown() {
  spdlog::drop_all();
  spdlog::shutdown();
}

} // namespace Utilities::Log