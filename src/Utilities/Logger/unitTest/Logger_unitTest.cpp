
#include "Logger.h"

#include "gtest/gtest.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <filesystem>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <locale>
#include <memory>
#include <regex>
#include <sstream>

// Logger singleton is not destroy for each test
class LoggerTest : public ::testing::Test {
protected:
  std::string CurrentDateToString() {
    using namespace boost::gregorian;

    date d = day_clock::local_day();
    std::unique_ptr<date_facet> df = std::make_unique<date_facet>("%Y-%m-%d");

    std::stringstream ss;
    ss.imbue(std::locale{ss.getloc(), df.get()});
    ss << d;
    return ss.str();
  }

  void CheckLogsData(const std::string &pLogPath, const std::string &pPattern,
                     int pCount) {

    int lCount{0};
    // Open the Log file
    std::ifstream lInputFile(pLogPath);

    if (lInputFile.is_open()) {
      std::string lLine;
      std::regex lRegExpr(
          R"((\d{4})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2}).(\d{3})(.*))");

      while (std::getline(lInputFile, lLine)) {
        EXPECT_THAT(lLine, ::testing::StartsWith("["));
        EXPECT_TRUE(std::regex_search(lLine, lRegExpr));
        if (lLine.find(pPattern) != std::string::npos) {
          ++lCount;
        }
      }

      // Close the file
      lInputFile.close();

      // Expect the correct number of written logs
      EXPECT_EQ(lCount, pCount);
    } else {
      std::cout << pLogPath << "file not exists \n";
      FAIL();
    }
  }
};

TEST_F(LoggerTest, RegisterModule) {

  // Register a new module or library
  Utilities::Log::Logger::GetInstance().RegisterModule("Module_Library");

  // Logger module is registred on GetInstance first call
  EXPECT_NE(spdlog::get(std::string(Utilities::Constants::c_LoggerModuleName)),
            nullptr);

  // module_library is registred
  EXPECT_NE(spdlog::get(std::string("Module_Library")), nullptr);

  // Exception for Trying  to register the same module_library again is
  // handled
  EXPECT_NO_THROW(
      Utilities::Log::Logger::GetInstance().RegisterModule("Module_Library"));
}

TEST_F(LoggerTest, LogsFilesCreated) {

  // Write trace message for module_library component
  Utilities::Log::Logger::GetInstance().Trace(
      "Module_Library", "trace message concerned Module_library {}",
      std::string("value_13"));
  // Expect Logs/Module_Library_{CurrentDate}.txt  for module_library
  // component created
  std::stringstream lLogFilePath{};
  lLogFilePath << "Logs/Module_Library_" << CurrentDateToString() << ".txt";
  EXPECT_TRUE(std::filesystem::exists(lLogFilePath.str()));

  // Write trace message for  LOGGER component
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName),
      "trace message concerned {} {}", Utilities::Constants::c_LoggerModuleName,
      std::string("value_73"));
  // Expect Logs/LOGGER_{CurrentDate}.txt  for LOGGER component
  // created
  lLogFilePath.str("");
  lLogFilePath << "Logs/LOGGER_" << CurrentDateToString() << ".txt";
  EXPECT_TRUE(std::filesystem::exists(lLogFilePath.str()));

  // Write trace message for  unRegistred_Module_Library  component
  // lead to write error in LOGGER Component
  Utilities::Log::Logger::GetInstance().Trace("unRegistred_Module_Library",
                                              "Message not logged");
  // Expect Logs/unRegistred_Module_Library_{CurrentDate}.txt  for
  // unRegistred_Module_Library component not created
  lLogFilePath.str("");
  lLogFilePath << "Logs/unRegistred_Module_Library_" << CurrentDateToString()
               << ".txt";
  EXPECT_FALSE(std::filesystem::exists(lLogFilePath.str()));
}

TEST_F(LoggerTest, Trace) {

  // Write trace message for  LOGGER component
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName),
      "Trace log message number 1-one");

  // Expect only 1 trace log
  std::stringstream lLogFilePath{};
  lLogFilePath << "Logs/Module_Library_" << CurrentDateToString() << ".txt";
  CheckLogsData(lLogFilePath.str(), "[Module_Library] [trace]", 1);

  // Write trace message for  LOGGER component
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName),
      "Trace log message number 2-two");

  // Expect only 3 trace logs
  lLogFilePath.str("");
  lLogFilePath << "Logs/LOGGER_" << CurrentDateToString() << ".txt";
  CheckLogsData(lLogFilePath.str(), "[LOGGER] [trace]", 3);
}

TEST_F(LoggerTest, ShutDown) {
  // Shutdown the logger after usage
  Utilities::Log::Logger::GetInstance().ShutDown();
}