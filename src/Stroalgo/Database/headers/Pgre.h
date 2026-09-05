/**
 * @file        Pgre.h
 * @author      ALLOGHO
 * @brief       A Pgre class to manage PostgreSQL database operations
 * @details     Uses libpq library to interact with PostgreSQL
 * @version     1.0
 * @date        2026-08-22
 * @copyright   Copyright (c) 2026 stroalgo.corp
 *
 */
#ifndef STROALGO_DATABASE_HEADERS_PGRE_H_
#define STROALGO_DATABASE_HEADERS_PGRE_H_

#include <map>
#include <string>

#include "GenericSingleton.h"

namespace Stroalgo::Database {

/** @brief A class to manage PostgreSQL database operations
 * @class Pgre
 */
class Pgre {
 public:
  /**
   * @brief Destroy the Pgre object
   * @memberof Pgre
   * @public
   */
  virtual ~Pgre() = default;

  /**
   * @brief Construct a new Pgre object
   * @memberof Pgre
   * @public
   */
  Pgre() = default;
};

class PgreManager : public Stroalgo::Common::GenericSingleton<Pgre> {
 public:
  /**
   * @brief Allow only the GenericSingleton to get access to constructor
   * @public
   */
  friend class Stroalgo::Common::GenericSingleton<Pgre>;

  /**
   * @brief Destroy the Pgre Manager object
   * @memberof PgreManager
   * @public
   */
  virtual ~PgreManager() = default;

 private:
  /**
   * @brief Construct a new Pgre Manager object
   * @memberof PgreManager
   * @private
   */
  PgreManager() = default;
};
}  // namespace Stroalgo::Database
#endif  // STROALGO_DATABASE_HEADERS_PGRE_H_
