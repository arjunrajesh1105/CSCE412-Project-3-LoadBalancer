#include "load_balancer.h"
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;
using std::to_string;

Load_Balancer::Load_Balancer(int server_total) : clock_cycle(0), requests_finished(0), requests_allowed(0), requests_blocked(0), rest_period(0)
{
    int server_track = 0;
    while (server_track < server_total)
    {
        web_servers.push_back(Web_Server(server_track));
        server_track++;
    }

    beginning_reqqueue_size = server_total * 100;
    int request_track = 0;
    while (request_track < (server_total * 100))
    {
        req_queue.push(createRandomRequest());
        request_track++;
    }

    outputBeginningData();
}

void Load_Balancer::totalCycleHandle()
{
    clock_cycle++;
    int server_track = 0;
    while (server_track < web_servers.size())
    {
        web_servers[server_track].handleServer_Request();
        if (web_servers[server_track].checkRequestStatus() == "done")
        {
            requests_finished++;
        }

        if (web_servers[server_track].checkServerFree() == "free")
        {
            if (req_queue.size() > 0)
            {
                Request curr_request = req_queue.front();
                req_queue.pop();
                
                if ((firewall_check.firewallAnalysis(curr_request.incoming_IP)) == "safe")
                {
                    web_servers[server_track].giveRequest(curr_request);
                    requests_allowed++;
                }
                else
                {
                    requests_blocked++;
                }
            }
        }
        server_track++;
    }

    handleRestPeriod();
    addRandomRequest();
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

void Load_Balancer::addRandomRequest()
{
    if (rand() % 10 == 0)
    {
        req_queue.push(createRandomRequest());
    }
}

void Load_Balancer::handleRestPeriod()
{
    if (rest_period == 0)
    {
        if (req_queue.size() > (80 * web_servers.size()))
        {
            addServer();
            rest_period = 50;
        }
        else if (req_queue.size() < (50 * web_servers.size()))
        {
            if (web_servers.size() > 1)
            {
                removeServer();
                rest_period = 50;
            }
        }
    }

    else
    {
        rest_period--;
    }
}

void Load_Balancer::outputBeginningData()
{
    cout << "Starting amount of requests in queue: " << beginning_reqqueue_size << endl;
    cout << "Starting amount of servers: " << web_servers.size() << endl;
    cout << "Request processing time range: 2-77 cycles" << endl;
}

void Load_Balancer::outputProgressiveData()
{

}

void Load_Balancer::outputEndingData()
{
    
}