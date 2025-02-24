
#include "Logger.h"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <filesystem>
#include <gtest/gtest.h>
#include <locale>
#include <memory>
#include <sstream>

class LoggerTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Register a new module or library
    Utilities::Log::Logger::GetInstance().RegisterModule("Module_Library");
  }

  void TearDown() override {
    // Shutdown the logger after usage
    Utilities::Log::Logger::GetInstance().ShutDown();

    // Clean-up tests folder
    if (std::filesystem::exists("Logs")) {
      // std::filesystem::remove_all("Logs");
    }
  }

  std::string CurrentDateToString() {
    using namespace boost::gregorian;

    date d = day_clock::universal_day();
    std::unique_ptr<date_facet> df = std::make_unique<date_facet>("%Y-%m-%d");

    std::stringstream ss;
    ss.imbue(std::locale{ss.getloc(), df.get()});
    ss << d;
    return ss.str();
  }
};

TEST_F(LoggerTest, RegisterModule) {

  // Logger module is registred on GetInstance first call
  EXPECT_NE(spdlog::get(std::string(Utilities::Constants::c_LoggerModuleName)),
            nullptr);

  // module_library is registred on Setup call
  EXPECT_NE(spdlog::get(std::string("Module_Library")), nullptr);

  // Exception for Trying  to register the same module_library again is handled
  EXPECT_NO_THROW(
      Utilities::Log::Logger::GetInstance().RegisterModule("Module_Library"));
}

TEST_F(LoggerTest, Trace) {

  // Re-Register
  // Only for test purpose: Logger singleton is not destroy for each test
  // Shutdown sdplog is called for each test
  Utilities::Log::Logger::GetInstance().RegisterModule(
      std::string(Utilities::Constants::c_LoggerModuleName));

  // Write trace message for module_library component
  Utilities::Log::Logger::GetInstance().Trace(
      "Module_Library", "trace message concerned Module_library {}",
      std::string("value_13"));
  // Expect Logs/Module_Library_{CurrentDate}.txt  for module_library component
  // created
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