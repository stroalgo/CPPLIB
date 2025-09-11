/**
 * @file        Constants.h
 * @author      ALLOGHO
 * @brief       A header file containing constant values
 * @version     1.0
 * @date        2025-07-18
 * @copyright   Copyright (c) 2025 stroalgo.corp
 *
 */

#ifndef STROALGO_COMMON_HEADERS_CONSTANTS_H_
#define STROALGO_COMMON_HEADERS_CONSTANTS_H_

#include <string_view>
#include <vector>
namespace Stroalgo::Constants {
constexpr std::string_view c_LoggerModuleName{"LOGGER"};
constexpr int c_DefaultServerPort{8080};

const std::vector<std::string_view> c_ModuleNames{
    c_LoggerModuleName,
};

}  // namespace Stroalgo::Constants

#endif  // STROALGO_COMMON_HEADERS_CONSTANTS_H_
