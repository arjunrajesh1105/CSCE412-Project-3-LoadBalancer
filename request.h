#ifndef Request_H
#define Request_H
#include <string>

struct Request 
{
    std::string incoming_IP; // This expresses the incoming IP address of the request
    std::string outgoing_IP; // This expresses the outgoing IP address of the request
    int time_track; // This is to track time for a request to be fully processed
    char job_type; // This will be to express if the job type is processing (p) or streaming (s)

    Request() : incoming_IP("0.0.0.0"), outgoing_IP("0.0.0.0"), time_track(0), job_type('p') {} // This is the default constructor for the struct Request
};
#endif