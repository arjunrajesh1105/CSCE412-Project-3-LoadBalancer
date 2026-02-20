#ifndef Web_Server_H
#define Web_Server_H
#include <string>
#include "request.h"
using std::string;

class Web_Server {
    public:
        string handleServer_Request(); // This helps process the current request time and the status of the server
        string checkServerFree(); // This simply checks if the server is free or busy
        string checkRequestStatus(); // This checks if the request is being worked on or done
        void giveRequest(Request curr_request); // This gives the server a request to work on and modifies the status of the server and request (busy and working)
        int returnServerNum(); // This returns the number of the server

        Web_Server(int server_number); // This is the constructor


    private:
        int server_num; // This helps to keep track of each server and it's respective number assigned to it
        string server_status; // This helps to keep track of the status of the server (free or busy)
        string request_status; // This helps to keep track of the status of the request (done or working)
        Request current_request; // This helps expresses the current request that is being handled
};

#endif