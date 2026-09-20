/**
 * @file Settings.cpp
 * @brief A Utils class
 * @details Uses Filesystem
 * @copyright   Copyright (c) 2025 stroalgo.corp
 */
#include "Utils.h"

#include <filesystem>
#include <iostream>

#include "Exceptions.h"

namespace Stroalgo::Common {

void Utils::CreateFolder(const std::string& pPath) {
  if (pPath.empty()) {
    throw Stroalgo::Exceptions::LoggerException("CreateFolder: Path is empty");
  }

  // ...existing code...
  std::error_code lError{};
  const bool lExists = std::filesystem::exists(pPath, lError);

  if (lError) {
    throw Exceptions::LoggerException("Unable to check logs path: " +
                                      lError.message());
  }

  if (!lExists) {
    std::filesystem::create_directories(pPath, lError);

    if (lError) {
      throw Exceptions::LoggerException("Unable to create logs folder: " +
                                        lError.message());
    }
  } else if (!std::filesystem::is_directory(pPath, lError)) {
    throw Exceptions::LoggerException("Logs path is not a directory");
  } else {
    // TODO: Use logger lib
    std::cout << "Folder already exists" << std::endl;
    // TODO : update setting_unitest when uncommenting the following line
    //  throw Exceptions::LoggerException("Folder already exists");
  }
}
}  // namespace Stroalgo::Common
