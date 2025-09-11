/**
 * @file        GenericSingleton.h
 * @author      ALLOGHO
 * @brief       A generic singleton class for creating unique object
 * @details     Uses Boost.Beast and OpenSSL for HTTPS functionality
 * @version     1.0
 * @date        2025-02-15
 * @copyright   Copyright (c) 2025 stroalgo.corp
 *
 */

#ifndef STROALGO_COMMON_HEADERS_GENERICSINGLETON_H
#define STROALGO_COMMON_HEADERS_GENERICSINGLETON_H

#include <iostream>
#include <memory>
#include <mutex>

namespace Stroalgo::Common {
/**
 * @class GenericSingleton
 * @brief Defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance
 *
 * @tparam T Type to provide for creating an singleton class
 */
template <typename T>
class GenericSingleton {
 public:
  /**
   * @brief Copy Construct
   * @public
   */
  GenericSingleton(const GenericSingleton&) = delete;

  /**
   * @brief Assignation Operator
   * @public
   * @return Singleton&
   */
  GenericSingleton& operator=(const GenericSingleton&) = delete;

  /**
   * @brief Get the Instance object
   * @public
   * @return T&
   */
  static T& GetInstance() {
    std::call_once(initFlag, []() { instance.reset(new T()); });
    return *instance;
  }

 protected:
  /**
   * @brief Construct a new Singleton object
   * @protected
   */
  GenericSingleton() = default;

  /**
   * @brief Destroy the Singleton object
   * @protected
   */
  virtual ~GenericSingleton() = default;

 private:
  /**
   * @brief Unique instance of type T
   * @private
   */
  inline static std::unique_ptr<T> instance{nullptr};

  /**
   * @brief Flag used to construct instance once
   * @private
   */
  inline static std::once_flag initFlag{};
};
}  // namespace Stroalgo::Common

#endif  // STROALGO_COMMON_HEADERS_GENERICSINGLETON_H
