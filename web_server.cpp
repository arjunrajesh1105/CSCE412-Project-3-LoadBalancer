#include "web_server.h"
using std::string;

Web_Server::Web_Server(int server_number) : server_num(server_number), server_status("free"), request_status("working") {}

// This functions helps process a server for one clock cycle
// Then checks the status of the server and request to see if the request is done or still being worked on, and if the server is free or busy
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


// This function simply returns the status of the server (free or busy)
string Web_Server::checkServerFree() 
{
    return server_status;
}

// This function simply returns the status of the request (working or done)
string Web_Server::checkRequestStatus() 
{
    return request_status;
}

// This functions gives the server a request to work on and modifies the status of the server and request (busy and working)
void Web_Server::giveRequest(Request curr_request) 
{
    current_request = curr_request;
    request_status = "working";
    server_status = "busy";
}

// This returns the number of the server (identification purposes)
int Web_Server::returnServerNum() 
{
    return server_num;
}
    