/**
 * @file web_server.h
 * @brief Defines the Web_Server class for handling individual server operations.
 * @details This header file declares the Web_Server class which manages individual
 *          web server instances in the load balancer system. Each server processes
 *          requests and tracks its operational status.
 * @author Arjun Rajesh - CSCE 412 Project 3
 * @date 2026
 */

#ifndef Web_Server_H
#define Web_Server_H
#include <string>
#include "request.h"
using std::string;

/**
 * @class Web_Server
 * @brief Manages an individual web server in the load balancer system.
 * @details This class handles request processing for a single server, tracking
 *          both the server's operational status and the current request's status.
 *          Each server can be either free or busy, and processes requests over
 *          multiple clock cycles.
 */
class Web_Server 
{
    public:
        /**
         * @brief Processes the current request for one clock cycle.
         * @details Decrements the request processing time and updates server/request
         *          status accordingly. If processing is complete, marks request as done
         *          and server as free.
         * @return String indicating current server status ("free" or "busy")
         */
        string handleServer_Request();
        
        /**
         * @brief Checks the current operational status of the server.
         * @return String indicating server status ("free" or "busy")
         */
        string checkServerFree();
        
        /**
         * @brief Checks the status of the request being processed.
         * @return String indicating request status ("working" or "done")
         */
        string checkRequestStatus();
        
        /**
         * @brief Assigns a request to the server for processing.
         * @details Sets the provided request to the current_request, marks the
         *          server as busy and the request as working.
         * @param curr_request The Request object to be assigned to this server
         */
        void giveRequest(Request curr_request);
        
        /**
         * @brief Constructor for Web_Server class.
         * @details Initializes a new web server with the given server number,
         *          sets status to "free" and request_status to "working".
         * @param server_number The unique identifier for this server
         */
        Web_Server(int server_number);

    private:
        /** @brief Unique identifier for this server instance */
        int server_num;
        
        /** @brief Current operational status of the server ("free" or "busy") */
        string server_status;
        
        /** @brief Current status of the request being processed ("working" or "done") */
        string request_status;
        
        /** @brief The request currently being processed by this server */
        Request current_request;
};

#endif