/**
 * @file        Utils.h
 * @author      ALLOGHO
 * @brief       A header file containing utilities function
 * @version     1.0
 * @date        2026-09-19
 * @copyright   Copyright (c) 2025 stroalgo.corp
 *
 */

#ifndef STROALGO_COMMON_HEADERS_UTILS_H_
#define STROALGO_COMMON_HEADERS_UTILS_H_

#include <string>

namespace Stroalgo::Common {

class Utils {
 public:
  /**
   * @brief Create  folder if it does not exist
   * @memberof Settings
   * @param pPath The path to the folder
   * @private
   */
  static void CreateFolder(const std::string& pPath);
};

}  // namespace Stroalgo::Common

#endif  // STROALGO_COMMON_HEADERS_UTILS_H_
