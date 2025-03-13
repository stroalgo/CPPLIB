
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

class LoggerTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Register a new module or library
    Utilities::Log::Logger::GetInstance().RegisterModule("Module_Library");
  }

  void TearDown() override {
    if (std::filesystem::exists("Logs")) {
      std::filesystem::remove_all("Logs");
    }
  }

  void CheckLogsStructure(const std::string &pLogPath,
                          const std::string &pPattern, int pCount) {

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

  bool CheckWrittenData(const std::string &pLogPath,
                        const std::string &pLogMsg) {

    bool lRet{false};
    // Open the Log file
    std::ifstream lInputFile(pLogPath);

    if (lInputFile.is_open()) {
      std::string lLine{};
      while (std::getline(lInputFile, lLine)) {
        if (lLine.find(pLogMsg) != std::string::npos) {
          lRet = true;
          break;
        }
      }

      // Close the file
      lInputFile.close();

      return lRet;

    } else {
      std::cout << pLogPath << "file not exists \n";
      return lRet;
    }
  }
};

TEST_F(LoggerTest, RegisterModule) {

  // Logger module is registred on GetInstance first call
  EXPECT_NE(spdlog::get(std::string(Utilities::Constants::c_LoggerModuleName)),
            nullptr);

  // module_library is registred
  EXPECT_NE(spdlog::get(std::string("Module_Library")), nullptr);

  // Exception for Trying  to register the same module_library again is
  // handled
  EXPECT_NO_THROW(
      Utilities::Log::Logger::GetInstance().RegisterModule("Module_Library"));

  // Expect warning message when register moldule with an empty name or contain
  // whitespace tab and newline
  Utilities::Log::Logger::GetInstance().RegisterModule("");
  Utilities::Log::Logger::GetInstance().RegisterModule("         ");
  Utilities::Log::Logger::GetInstance().RegisterModule("\n");
  Utilities::Log::Logger::GetInstance().RegisterModule("\t");
  Utilities::Log::Logger::GetInstance().RegisterModule("      \t    \n");

  std::stringstream lLogFilePath{};
  lLogFilePath.str("");
  lLogFilePath << "Logs/LOGGER/LOGGER_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [error]", 5);
  CheckWrittenData(lLogFilePath.str(), "Module Name can not be an empty string "
                                       "or contain whitespace,tab,newline");
}

TEST_F(LoggerTest, LogsFilesCreated) {

  // Write trace message for module_library component
  Utilities::Log::Logger::GetInstance().Trace(
      "Module_Library", "trace message concerned Module_library {}",
      std::string("value_13"));

  // Expect Logs/Module_Library_{CurrentDate}.txt  for module_library
  // component created
  std::stringstream lLogFilePath{};
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  EXPECT_TRUE(std::filesystem::exists(lLogFilePath.str()));

  // Write trace message for  LOGGER component
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName),
      "trace message concerned {} {}", Utilities::Constants::c_LoggerModuleName,
      std::string("value_73"));
  // Expect Logs/LOGGER_{CurrentDate}.txt  for LOGGER component
  // created
  lLogFilePath.str("");
  lLogFilePath << "Logs/LOGGER/LOGGER_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  EXPECT_TRUE(std::filesystem::exists(lLogFilePath.str()));

  // Write trace message for  unRegistred_Module_Library  component
  // lead to write error in LOGGER Component
  Utilities::Log::Logger::GetInstance().Trace("unRegistred_Module_Library",
                                              "Message not logged");
  // Expect Logs/unRegistred_Module_Library_{CurrentDate}.txt  for
  // unRegistred_Module_Library component not created
  lLogFilePath.str("");
  lLogFilePath << "Logs/unRegistred_Module_Library/unRegistred_Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  EXPECT_FALSE(std::filesystem::exists(lLogFilePath.str()));

  // Expect warning log message created for trying to use
  // unRegistred_Module_Library
  lLogFilePath.str("");
  lLogFilePath << "Logs/LOGGER/LOGGER_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [error]", 1);
  CheckWrittenData(
      lLogFilePath.str(),
      "Unable to write Log : Module unRegistred_Module_Library not registred");
}

TEST_F(LoggerTest, Trace) {

  std::stringstream lLogFilePath{};
  std::string lLogMsg = "trace message concerned Module_library value_13";

  // Write trace message for module_library component
  Utilities::Log::Logger::GetInstance().Trace("Module_Library", lLogMsg);

  // Expect only 1 trace log
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [trace]", 1);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));

  lLogFilePath.str("");
  lLogFilePath << "Logs/LOGGER/LOGGER_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";

  // Write trace message for  LOGGER component
  lLogMsg = "Trace log message number 1-one";
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName), lLogMsg);

  // Expect only 1 trace logs
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [trace]", 1);

  // Write trace message for  LOGGER component
  lLogMsg = "Trace log message number 2-two";
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName), lLogMsg);

  // Expect only 2 trace logs
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [trace]", 2);
}

TEST_F(LoggerTest, Debug) {

  std::string lLogMsg = "Debug log message number 1-one";
  Utilities::Log::Logger::GetInstance().Debug("Module_Library", lLogMsg);

  // Expect only 1 Debug log
  std::stringstream lLogFilePath{};
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [debug]", 1);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));

  // Write another debug message
  lLogMsg = "Debug log message number 2-two";
  Utilities::Log::Logger::GetInstance().Debug("Module_Library", lLogMsg);

  // Expect only 2 debug logs
  lLogFilePath.str("");
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [debug]", 2);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));
}

TEST_F(LoggerTest, Info) {

  std::string lLogMsg = "Info log message number 1-one";
  Utilities::Log::Logger::GetInstance().Info("Module_Library", lLogMsg);

  // Expect only 1 Info log
  std::stringstream lLogFilePath{};
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [info]", 1);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));

  // Write another Info message
  lLogMsg = "Info log message number 2-two";
  Utilities::Log::Logger::GetInstance().Info("Module_Library", lLogMsg);

  // Expect only 2 Info logs
  lLogFilePath.str("");
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [info]", 2);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));
}

TEST_F(LoggerTest, Warning) {

  std::string lLogMsg = "Warning log message number 1-one";
  Utilities::Log::Logger::GetInstance().Warning("Module_Library", lLogMsg);

  // Expect only 1 Warning log
  std::stringstream lLogFilePath{};
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [warning]", 1);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));

  // Write another Warning message
  lLogMsg = "Warning log message number 2-two";
  Utilities::Log::Logger::GetInstance().Warning("Module_Library", lLogMsg);

  // Expect only 2 Warning logs
  lLogFilePath.str("");
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [warning]", 2);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));
}

TEST_F(LoggerTest, Error) {

  std::string lLogMsg = "Error log message number 1-one";
  Utilities::Log::Logger::GetInstance().Error(
      std::string(Utilities::Constants::c_LoggerModuleName), lLogMsg);

  // Expect only 1 Error log
  std::stringstream lLogFilePath{};
  lLogFilePath << "Logs/LOGGER/LOGGER_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [error]", 1);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));

  // Write another Error message
  lLogMsg = "Error log message number 2-two";
  Utilities::Log::Logger::GetInstance().Error(
      std::string(Utilities::Constants::c_LoggerModuleName), lLogMsg);

  // Expect only 2 Error logs
  lLogFilePath.str("");
  lLogFilePath << "Logs/LOGGER/LOGGER_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [error]", 2);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));
}

TEST_F(LoggerTest, critical) {

  std::string lLogMsg = "critical log message number 1-one";
  Utilities::Log::Logger::GetInstance().Critical("Module_Library", lLogMsg);

  // Expect only 1 critical log
  std::stringstream lLogFilePath{};
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [critical]", 1);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));

  // Write another critical message
  lLogMsg = "critical log message number 2-two";
  Utilities::Log::Logger::GetInstance().Critical("Module_Library", lLogMsg);

  // Expect only 2 critical logs
  lLogFilePath.str("");
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [critical]", 2);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));
}

TEST_F(LoggerTest, ChangeLogLevel) {

  // Write another trace message
  std::string lLogMsg = "Trace log message number 1-one";
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName), lLogMsg);

  // Write another trace message
  lLogMsg = "Trace log message number 2-two";
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName), lLogMsg);

  // Write another trace message
  lLogMsg = "Trace log message number 3-three";
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName), lLogMsg);

  // Expect only 3 trace logs
  std::stringstream lLogFilePath{};
  lLogFilePath << "Logs/LOGGER/LOGGER_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [trace]", 3);

  // Change log level to info level
  Utilities::Log::Logger::GetInstance().SetModuleLogLevel(
      std::string(Utilities::Constants::c_LoggerModuleName),
      spdlog::level::info);

  // Write another trace message
  lLogMsg = "Trace log message number 4-four";
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName), lLogMsg);

  // Expect only 3 error logs - last trace log has not be written
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [trace]", 3);
  EXPECT_FALSE(CheckWrittenData(lLogFilePath.str(), lLogMsg));
}

TEST_F(LoggerTest, LogLevelunRegistredModule) {

  // Change log level for unRegistred_Module_Library  component
  //  lead to write error in LOGGER Component
  Utilities::Log::Logger::GetInstance().SetModuleLogLevel(
      "unRegistred_Module_Library", spdlog::level::info);

  // Expect only 1 error log written
  std::stringstream lLogFilePath{};
  std::string lLogMsg = "Unable to set level : Module "
                        "unRegistred_Module_Library is not registred";
  lLogFilePath << "Logs/LOGGER/LOGGER_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [error]", 1);
  EXPECT_TRUE(CheckWrittenData(lLogFilePath.str(), lLogMsg));
}

TEST_F(LoggerTest, GetModuleLogLevel) {
  EXPECT_EQ(Utilities::Log::Logger::GetInstance().GetModuleLevel(
                std::string(Utilities::Constants::c_LoggerModuleName)),
            "trace");

  // Change log level to info level
  Utilities::Log::Logger::GetInstance().SetModuleLogLevel(
      std::string(Utilities::Constants::c_LoggerModuleName),
      spdlog::level::debug);

  EXPECT_EQ(Utilities::Log::Logger::GetInstance().GetModuleLevel(
                std::string(Utilities::Constants::c_LoggerModuleName)),
            "debug");
}

TEST_F(LoggerTest, GetLogLevels) {
  std::vector<std::string> lExpectedKeys{
      "Module_Library", std::string(Utilities::Constants::c_LoggerModuleName)};

  // Retrieve loggers level
  const auto &lLogLevels{Utilities::Log::Logger::GetInstance().GetLogLevels()};

  // Exepect only 2 logger registred
  EXPECT_EQ(lLogLevels.size(), 2);

  // Expect LOGGER and Module_Library as logger keys and trace as level for both
  for (const auto &[lKey, lValue] : lLogLevels) {
    auto lContain =
        std::find(lExpectedKeys.cbegin(), lExpectedKeys.cend(), lKey);
    EXPECT_NE(lContain, lExpectedKeys.cend());
    EXPECT_EQ(lValue, "trace");
  }
}

TEST_F(LoggerTest, ShutDown) {
  // Shutdown the logger after usage
  Utilities::Log::Logger::GetInstance().ShutDown();

  EXPECT_EQ(nullptr,
            spdlog::get(std::string(Utilities::Constants::c_LoggerModuleName)));
}

TEST_F(LoggerTest, CurrentDateToString) {
  std::regex lRegExpr(R"((\d{4})-(\d{2})-(\d{2}))");
  EXPECT_TRUE(std::regex_search(
      Utilities::Log::Logger::GetInstance().CurrentDateToString(), lRegExpr));
}

TEST_F(LoggerTest, DeleteAllLogs) {

  std::stringstream lLogFilePath{};

  // Write log for LOGGER module
  std::string lLogMsg = "Trace log message about LOGGER_Module";
  Utilities::Log::Logger::GetInstance().Trace(
      std::string(Utilities::Constants::c_LoggerModuleName), lLogMsg);

  lLogFilePath << "Logs/LOGGER/LOGGER_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [trace]", 1);

  // Write log for Module_Library
  lLogMsg = "Critical log message about Module_Library";
  Utilities::Log::Logger::GetInstance().Critical("Module_Library", lLogMsg);
  lLogFilePath.str("");
  lLogFilePath << "Logs/Module_Library/Module_Library_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [critical]", 1);

  // Create fake previous logs files
  std::string lLoggerPreviousLogFilePath{"Logs/LOGGER/LOGGER_1313_01_13.txt"};
  std::string lModuleLibraryPreviousLogFilePath{
      "Logs/Module_Library/Module_Library_1313_01_13.txt"};
  std::ofstream lLOGGERPreviousLogFile(lLoggerPreviousLogFilePath);
  std::ofstream lModuleLibraryPreviousLogFile(
      lModuleLibraryPreviousLogFilePath);

  lLOGGERPreviousLogFile << "Very ancient log" << std::endl;
  lModuleLibraryPreviousLogFile << "Very ancient log" << std::endl;

  lModuleLibraryPreviousLogFile.close();
  lLOGGERPreviousLogFile.close();

  // Delete all logs
  Utilities::Log::Logger::GetInstance().DeleteAllLogs();

  // Expect no logs present / Logs files for Module_Library is empty
  CheckLogsStructure(lLogFilePath.str(), "[Module_Library] [critical]", 0);
  EXPECT_TRUE(
      std::filesystem::is_empty(std::filesystem::path(lLogFilePath.str())));

  // Expect no logs present / Logs files for LOGGER is empty
  lLogFilePath.str("");
  lLogFilePath << "Logs/LOGGER/LOGGER_"
               << Utilities::Log::Logger::GetInstance().CurrentDateToString()
               << ".txt";
  CheckLogsStructure(lLogFilePath.str(), "[LOGGER] [trace]", 0);
  EXPECT_TRUE(
      std::filesystem::is_empty(std::filesystem::path(lLogFilePath.str())));

  // Expect Previous logs files have been deleted
  EXPECT_FALSE(std::filesystem::exists(lLoggerPreviousLogFilePath));
  EXPECT_FALSE(std::filesystem::exists(lModuleLibraryPreviousLogFilePath));
}