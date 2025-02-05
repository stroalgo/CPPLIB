/**
 * @file server.cpp
 * @brief HTTPS Server with CRUD operations
 * @details Uses Boost.Beast and OpenSSL for HTTPS functionality
 */

#include "server.h"

// #include <boost/beast/core.hpp>
// #include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <algorithm>


namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

namespace Utilities {


/**
 * @brief Parses task from JSON-like request body
 * @param body The request body containing the task
 * @return Extracted task string
 * @note Simplified parser for demonstration purposes
 */
std::string parseTaskFromBody(const std::string& body) {
    size_t start = body.find(":\"") + 2;
    size_t end = body.find("\"", start);
    return body.substr(start, end - start);
}

/**
 * @brief Adds a new task to the to-do list
 * @param task The task description to add
 * @return Assigned task ID
 */
int addTask(const std::string& task) {
    todoList[nextId] = task;
    return nextId++;
}

/**
 * @brief Retrieves all tasks in JSON format
 * @return JSON string containing all tasks
 */
std::string getAllTasks() {
    std::string json = "[";
    for (const auto& item : todoList) {
        json += "{\"id\":" + std::to_string(item.first) + ",\"task\":\"" + item.second + "\"},";
    }
    if (!todoList.empty()) json.pop_back();
    json += "]";
    return json;
}

/**
 * @brief Updates an existing task
 * @param id Task ID to update
 * @param task New task description
 * @return true if update succeeded, false if ID not found
 */
bool updateTask(int id, const std::string& task) {
    if (todoList.find(id) != todoList.end()) {
        todoList[id] = task;
        return true;
    }
    return false;
}

/**
 * @brief Deletes a task
 * @param id Task ID to delete
 * @return true if deletion succeeded, false if ID not found
 */
bool deleteTask(int id) {
    return todoList.erase(id) > 0;
}

/**
 * @brief Handles incoming HTTP requests
 * @param req The HTTP request to handle
 * @return Appropriate HTTP response
 */
http::response<http::string_body> handleRequest(const http::request<http::string_body>& req) {
    http::response<http::string_body> res{http::status::ok, req.version()};
    res.set(http::field::server, "To-Do List Server");
    res.set(http::field::content_type, "application/json");

    try {
        if (req.method() == http::verb::get) {
            res.body() = getAllTasks();
        } else if (req.method() == http::verb::post) {
            std::string task = parseTaskFromBody(req.body());
            int id = addTask(task);
            res.body() = "{\"id\":" + std::to_string(id) + "}";
        } else if (req.method() == http::verb::put) {
            int id = 0;//std::stoi(req.target().to_string().substr(1));
            std::string task = parseTaskFromBody(req.body());
            if (updateTask(id, task)) {
                res.body() = "{\"status\":\"updated\"}";
            } else {
                res.result(http::status::not_found);
                res.body() = "{\"error\":\"Item not found\"}";
            }
        } else if (req.method() == http::verb::delete_) {
            int id = 0;//std::stoi(req.target().to_string().substr(1));
            if (deleteTask(id)) {
                res.body() = "{\"status\":\"deleted\"}";
            } else {
                res.result(http::status::not_found);
                res.body() = "{\"error\":\"Item not found\"}";
            }
        } else {
            res.result(http::status::bad_request);
            res.body() = "{\"error\":\"Invalid request\"}";
        }
    } catch (const std::exception& e) {
        res.result(http::status::internal_server_error);
        res.body() = "{\"error\":\"Server error\"}";
    }

    res.prepare_payload();
    return res;
}

/**
 * @brief Main server entry point
 * @return Exit status code
 */
// int main() {
//     try {
//         ssl::context ctx{ssl::context::tlsv12};
//         ctx.use_certificate_chain_file("cert.pem");
//         ctx.use_private_key_file("key.pem", ssl::context::pem);

//         net::io_context ioc;
//         tcp::acceptor acceptor(ioc, {tcp::v4(), 8080});

//         std::cout << "To-Do List Server (HTTPS) running on port 8080..." << std::endl;

//         while (true) {
//             tcp::socket socket(ioc);
//             acceptor.accept(socket);

//             beast::ssl_stream<beast::tcp_stream> stream{ioc, ctx};
//             stream.next_layer() = beast::tcp_stream(std::move(socket));
//             stream.handshake(ssl::stream_base::server);

//             beast::flat_buffer buffer;
//             http::request<http::string_body> req;
//             http::read(stream, buffer, req);

//             auto res = handleRequest(req);

//             http::write(stream, res);
//             stream.shutdown();
//         }
//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }
//     return 0;
// }

}