#include "request.h"
#include <iostream>

int main() {
    Request current_request;
    std::cout << "Incoming IP address: " << current_request.incoming_IP << std::endl;
    std::cout << "Outgoing IP address: " << current_request.outgoing_IP << std::endl;
    std::cout << "Time: " << current_request.time_track << std::endl;
    std::cout << "Job Type: " << current_request.job_type << std::endl;
    return 0;
}