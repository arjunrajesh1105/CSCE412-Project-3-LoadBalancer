#ifndef REQUEST_H
#define REQUEST_H
#include <string>

struct Request {
    std::string incoming_IP;
    std::string outgoing_IP;
    int time_track;
    char job_type;

    Request() : incoming_IP("0.0.0.0"), outgoing_IP("0.0.0.0"), time_track(0), job_type('p') {}
};
#endif