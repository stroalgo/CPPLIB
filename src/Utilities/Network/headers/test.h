/**
 * @file        web_picture_requests.hpp
 * @author      ALLOGHO (stronguy.allogho@external.idemia.com)
 * @brief       Class for picture request
 * @details     This header provides the declaration of picture requests
 * @version     1.0
 * @date        2023-03-31
 *
 * @copyright   Copyright (c) 2023 stroalgo.corp
 *
 */

#include <stdio>

/**
 * @brief web namespace
 *
 */
namespace Utilities {
/**
 * @brief Test documentation
 *
 */

int test(void);

/**
 * @brief Osiris web Picture requests
 *
 */
class WebPictureRequests {
public:
  /**
   * @brief Construct a new Web Picture Requests object
   *
   */
  WebPictureRequests();

  /**
   * @brief Implementation of the base class Method for converting to json
   * Format
   *
   * @return const boost::property_tree::ptree
   */
  const int ToJson();
};

} // namespace Utilities