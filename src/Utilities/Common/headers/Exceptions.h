#include <exception>
#include <string>

namespace Utilities::Exceptions {
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
  LoggerException(const std::string &pMessage) : message(pMessage) {}

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

}  // namespace Utilities::Exceptions
