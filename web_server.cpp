/**
 * @file web_server.cpp
 * @brief Implementation of the Web_Server class.
 * @details Contains the implementation of all Web_Server methods for managing
 *          individual server operations and request processing.
 * @author Arjun Rajesh - CSCE 412 Project 3
 * @date 2026
 */

#include "web_server.h"
using std::string;

/**
 * @brief Constructor implementation for Web_Server.
 * @details Initializes a new Web_Server instance with the provided server number,
 *          sets the initial server status to "free" and request status to "working".
 * @param server_number Unique identifier for this server instance
 */
Web_Server::Web_Server(int server_number) : server_num(server_number), server_status("free"), request_status("working") {}

/**
 * @brief Processes the current request for one clock cycle.
 * @details Decrements the time_track of the current request by one and updates
 *          the server and request statuses accordingly. When time_track reaches 0,
 *          marks the request as done and the server as free.
 * @return String representing the server's status: "free" or "busy"
 */
string Web_Server::handleServer_Request() 
{
    if(server_status == "busy")
    {
        if(current_request.time_track == 0)
        {
            server_status = "free";
            request_status = "done";
        }

        else
        {
            current_request.time_track--;
            if(current_request.time_track == 0)
            {
                server_status = "free";
                request_status = "done";
            }
            else
            {
                server_status = "busy";
                request_status = "working";
            }
        }
    }

    else
    {
        server_status = "free";
    }
    
    return server_status;
}

/**
 * @brief Returns the current operational status of the server.
 * @details Simple accessor method that returns whether the server is currently
 *          free and available for new requests or busy processing an existing request.
 * @return String value: "free" if available, "busy" if processing a request
 */
string Web_Server::checkServerFree() 
{
    return server_status;
}

/**
 * @brief Returns the current status of the request being processed.
 * @details Accessor method to check if the current request is still being
 *          worked on or has completed processing.
 * @return String value: "working" if still processing, "done" if complete
 */
string Web_Server::checkRequestStatus() 
{
    return request_status;
}

/**
 * @brief Assigns a new request to this server for processing.
 * @details Sets the provided request as the current request being processed,
 *          marks the server as "busy" and the request as "working".
 * @param curr_request The Request object to assign to this server
 */
void Web_Server::giveRequest(Request curr_request) 
{
    current_request = curr_request;
    request_status = "working";
    server_status = "busy";
}
    