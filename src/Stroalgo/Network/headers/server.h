// /**
//  * @file        server.h
//  * @author      ALLOGHO
//  * @brief       HTTPS Server with CRUD operations
//  * @details     Uses Boost.Beast and OpenSSL for HTTPS functionality
//  * @version     1.0
//  * @date        2025-02-13
//  * @copyright   Copyright (c) 2025 stroalgo.corp
//  *
//  */

// #include <boost/beast/core.hpp>
// #include <boost/beast/http.hpp>
// // #include <boost/beast/ssl.hpp>
// // #include <boost/beast/version.hpp>
// // #include <boost/asio.hpp>
// // #include <boost/asio/ssl.hpp>
// #include <map>
// #include <string>

// namespace beast = boost::beast;
// namespace http = beast::http;

// /**
//  * @brief Utilities namespace
//  *
//  */
// namespace Utilities {

// /// Global to-do list storage (in-memory)
// std::map<int, std::string> todoList;

// /// Next available ID counter
// int nextId = 1;

// /**
//  * @brief Parses task from JSON-like request body
//  * @param body The request body containing the task
//  * @return Extracted task string
//  * @note Simplified parser for demonstration purposes
//  */
// std::string parseTaskFromBody(const std::string& body);

// /**
//  * @brief Adds a new task to the to-do list
//  * @param task The task description to add
//  * @return Assigned task ID
//  */
// int addTask(const std::string& task) ;

// /**
//  * @brief Retrieves all tasks in JSON format
//  * @return JSON string containing all tasks
//  */
// std::string getAllTasks() ;

// /**
//  * @brief Updates an existing task
//  * @param id Task ID to update
//  * @param task New task description
//  * @return true if update succeeded, false if ID not found
//  */
// bool updateTask(int id, const std::string& task) ;

// /**
//  * @brief Deletes a task
//  * @param id Task ID to delete
//  * @return true if deletion succeeded, false if ID not found
//  */
// bool deleteTask(int id) ;

// /**
//  * @brief Handles incoming HTTP requests
//  * @param req The HTTP request to handle
//  * @return Appropriate HTTP response
//  */
// http::response<http::string_body> handleRequest(const
// http::request<http::string_body>& req) ;

// } // namespace Utilities
