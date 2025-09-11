/**
 * @file        Exceptions.h
 * @author      ALLOGHO
 * @brief       All exceptions class used in the library
 * @version     1.0
 * @date        2025-07-18
 * @copyright   Copyright (c) 2025 stroalgo.corp
 *
 */

#ifndef STROALGO_COMMON_HEADERS_EXCEPTIONS_H_
#define STROALGO_COMMON_HEADERS_EXCEPTIONS_H_

#include <exception>
#include <string>

namespace Stroalgo::Exceptions {
/**
 * @brief Class to handle Logger exception
 @todo: change to  AppException as template maybe
 *
 */
class LoggerException : public std::exception {
 public:
  /**
   * @brief Construct a new Logger Exception object
   *
   * @param pMessage Exception message
   */
  explicit LoggerException(const std::string &pMessage) : message(pMessage) {}

  /**
   * @brief Return the message related to the exception
   *
   * @return Exception message
   */
  const char *what() { return message.c_str(); }

 private:
  /**
   * @brief Exception message
   * @private
   * @memberof LoggerException
   */
  const std::string &message;
};

}  // namespace Stroalgo::Exceptions

#endif  // CPPLIB_COMMON_HEADERS_EXCEPTIONS_H_
