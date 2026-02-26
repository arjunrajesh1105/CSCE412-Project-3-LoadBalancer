#include "load_balancer.h"
#include <string>
using std::string;
using std::to_string;

Load_Balancer::Load_Balancer(int server_total) : clock_cycle(0), requests_finished(0), requests_blocked(0), rest_period(0)
{
    int server_track = 0;
    while (server_track < server_total)
    {
        web_servers.push_back(Web_Server(server_track));
        server_track++;
    }

    int request_track = 0;
    while (request_track < (server_total * 100))
    {
        req_queue.push(createRandomRequest());
        request_track++;
    }
}

void Load_Balancer::totalCycleHandle()
{

}

void Load_Balancer::addServer()
{
    int last_index = web_servers.size() - 1;
    web_servers.push_back(Web_Server(last_index + 1));
}

void Load_Balancer::removeServer()
{
    int last_index = web_servers.size() - 1;
    web_servers.pop_back();
}

Request Load_Balancer::createRandomRequest()
{
    Request curr_request;
    curr_request.incoming_IP = randomIPGenerator();
    curr_request.outgoing_IP = randomIPGenerator();
    curr_request.time_track = (rand() % 76) + 2;
    if((rand() % 2) == 0)
    {
        curr_request.job_type = 'p';
    }
    else
    {
        curr_request.job_type = 's';
    }
    return curr_request;
}

string Load_Balancer::randomIPGenerator()
{
    int octet1 = rand() % 256;
    int octet2 = rand() % 256;
    int octet3 = rand() % 256;
    int octet4 = rand() % 256;
    return to_string(octet1) + "." + to_string(octet2) + "." + to_string(octet3) + "." + to_string(octet4);
}