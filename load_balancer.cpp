#include "load_balancer.h"
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;
using std::to_string;
#define red "\033[31m"
#define green "\033[32m"
#define blue "\033[34m"
#define yellow "\033[33m"
#define orange "\033[38;5;208m"
#define bold "\033[1m"
#define reset "\033[0m"

Load_Balancer::Load_Balancer(int server_total) : clock_cycle(0), requests_finished(0), requests_allowed(0), requests_blocked(0), rest_period(0), beginning_reqqueue_size(server_total * 100), beginning_server_size(server_total),max_servers(server_total), p_jobtype_total(0), s_jobtype_total(0), servers_added(0), servers_removed(0), randomRequestSuccesfulAdded(0), randomRequestFailedAdded(0)
{
    log.open("load_balancer_log.txt");
    int server_track = 0;
    while (server_track < server_total)
    {
        web_servers.push_back(Web_Server(server_track));
        server_track++;
    }

    max_servers = server_total;

    beginning_reqqueue_size = server_total * 100;
    beginning_server_size = server_total;
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
                    log << "CYCLE " << clock_cycle << ": REQUEST BLOCKED - Total Requests Blocked: " << requests_blocked << endl;
                    log << endl;
                }
            }
        }
        server_track++;
    }

    handleRestPeriod();
    addRandomRequest();
    
    if (clock_cycle % 5 == 0)
    {
        outputProgressiveData();
    }
}

void Load_Balancer::addServer()
{
    int last_index = web_servers.size() - 1;
    web_servers.push_back(Web_Server(last_index + 1));
    servers_added++;
    if (web_servers.size() > max_servers)
    {
        max_servers = web_servers.size();
    }

    log << "CYCLE " << clock_cycle << ": SERVER ADDED - Total Servers: " << web_servers.size() << endl;
    log << endl;
}

void Load_Balancer::removeServer()
{
    int last_index = web_servers.size() - 1;
    web_servers.pop_back();
    servers_removed++;

    log << "CYCLE " << clock_cycle << ": SERVER REMOVED - Total Servers: " << web_servers.size() << endl;
    log << endl;
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
        p_jobtype_total++;
    }
    else
    {
        curr_request.job_type = 's';
        s_jobtype_total++;
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
        randomRequestSuccesfulAdded++;
        log << "CYCLE " << clock_cycle << ": RANDOM REQUEST ADDED - Total Requests Added: " << randomRequestSuccesfulAdded << endl;
        log << endl;
    }
    else
    {
        randomRequestFailedAdded++;
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
    cout << blue << bold << "LOAD BALANCER SIMULATION START" << reset << endl;
    cout << blue << "------------------------------" << reset << endl;
    cout << endl;
    cout << "Starting amount of requests in queue: " << beginning_reqqueue_size << endl;
    cout << "Starting amount of servers: " << web_servers.size() << endl;
    cout << "Request processing time range: 2-77 cycles" << endl;
    cout << endl;

    log << "LOAD BALANCER LOG: SIMULATION START" << endl;
    log << "------------------------------" << endl;
    log << endl;
    log << "Starting amount of requests in queue: " << beginning_reqqueue_size << endl;
    log << "Starting amount of servers: " << web_servers.size() << endl;
    log << "Request processing time range: 2-77 cycles" << endl;
    log << endl;
    
}

void Load_Balancer::outputProgressiveData()
{
    float requestSuccessfulAdded = randomRequestSuccesfulAdded;
    if (clock_cycle % 500 == 0)
    {
        cout << blue << bold << "PROGRESSIVE OUTPUT UPDATE" << reset << endl;
        cout << blue << "------------------------------" << reset << endl;
        cout << endl;
        cout << orange << "Clock cycle: " << clock_cycle << reset << endl;
        cout << "Queue size: " << req_queue.size() << endl;
        cout << "Current servers: " << web_servers.size() << endl;
        cout << yellow << "Servers added: " << servers_added << reset << endl;
        cout << yellow << "Servers removed: " << servers_removed << reset << endl;
        cout << green << "Requests finished: " << requests_finished << reset << endl;
        cout << red << "Requests blocked: " << requests_blocked << reset << endl;
        cout << yellow << "Random requests successfully added to queue: " << randomRequestSuccesfulAdded << reset << endl;
        cout << yellow << "Random requests added percentage: " << ((requestSuccessfulAdded * 100) / clock_cycle) << "%" << reset << endl;
        cout << endl;
    }

    if (clock_cycle % 5 == 0)
    {
        log << "PROGRESSIVE OUTPUT UPDATE" << endl;
        log << "------------------------------" << endl;
        log << endl;
        log << "Clock cycle: " << clock_cycle << endl;
        log << "Queue size: " << req_queue.size() << endl;
        log << "Current servers: " << web_servers.size() << endl;
        log << "Servers added: " << servers_added << endl;
        log << "Servers removed: " << servers_removed << endl;
        log << "Requests finished: " << requests_finished << endl;
        log << "Requests blocked: " << requests_blocked << endl;
        log << "Random requests successfully added to queue: " << randomRequestSuccesfulAdded << endl;
        log << "Random requests added percentage: " << ((requestSuccessfulAdded * 100) / clock_cycle) << "%" << endl;
        log << endl;
    }
}

void Load_Balancer::outputEndingData()
{
    float requestSuccessfulAdded = randomRequestSuccesfulAdded;

    cout << blue << bold << "FINAL OUTPUT SUMMARY" << reset << endl;
    cout << blue << "------------------------------" << reset << endl;
    cout << endl;
    cout << orange << "Total clock cycles: " << clock_cycle << reset << endl;
    cout << green << "Total requests finished: " << requests_finished << reset << endl;
    cout << "Starting queue size: " << beginning_reqqueue_size << endl;
    cout << "Starting amount of servers: " << beginning_server_size << endl;
    cout << "Requests remaining in queue: " << req_queue.size() << endl;
    cout << "Final server amount: " << web_servers.size() << endl;
    cout << "Final active servers: " << totalActiveServers() << endl;
    cout << "Final inactive servers: " << totalInactiveServers() << endl;
    cout << "Max servers during simulation: " << max_servers << endl;
    cout << yellow << "Total servers added: " << servers_added << reset << endl;
    cout << yellow << "Total servers removed: " << servers_removed << reset << endl;
    cout << red << "Total requests blocked by firewall: " << requests_blocked << reset << endl;
    cout << green << "Total requests allowed through firewall: " << requests_allowed << reset << endl;
    cout << "Ratio of p to s job types: " << p_jobtype_total << ":" << s_jobtype_total << endl;
    cout << yellow << "Total requests randomly added to the queue: " << randomRequestSuccesfulAdded << reset << endl;
    cout << yellow << "Total requests that failed to be randomly added to the queue: " << randomRequestFailedAdded << endl;
    cout << yellow << "Random requests added percentage: " << ((requestSuccessfulAdded * 100) / clock_cycle) << "%" << reset << endl;

    log << "FINAL OUTPUT SUMMARY" << endl;
    log << "------------------------------" << endl;
    log << endl;
    log << "Total clock cycles: " << clock_cycle << endl;
    log << "Total requests finished: " << requests_finished << endl;
    log << "Starting queue size: " << beginning_reqqueue_size << endl;
    log << "Starting amount of servers: " << beginning_server_size << endl;
    log << "Requests remaining in queue: " << req_queue.size() << endl;
    log << "Final server amount: " << web_servers.size() << endl;
    log << "Final active servers: " << totalActiveServers() << endl;
    log << "Final inactive servers: " << totalInactiveServers() << endl;
    log << "Max servers during simulation: " << max_servers << endl;
    log << "Total servers added: " << servers_added << endl;
    log << "Total servers removed: " << servers_removed << endl;
    log << "Total requests blocked by firewall: " << requests_blocked << endl;
    log << "Total requests allowed through firewall: " << requests_allowed << endl;
    log << "Ratio of p to s job types: " << p_jobtype_total << ":" << s_jobtype_total << endl;
    log << "Total requests randomly added to the queue: " << randomRequestSuccesfulAdded << endl;
    log << "Total requests that failed to be randomly added to the queue: " << randomRequestFailedAdded << endl;
    log << "Random requests added percentage: " << ((requestSuccessfulAdded * 100) / clock_cycle) << "%" << endl;

}

int Load_Balancer::totalActiveServers()
{
    int active_servers = 0;
    int servers_track = 0;
    while (servers_track < web_servers.size())
    {
        if (web_servers[servers_track].checkServerFree() == "busy")
        {
            active_servers++;
        }
        servers_track++;
    }
    return active_servers;
}

int Load_Balancer::totalInactiveServers()
{
    int inactive_servers = 0;
    int servers_track = 0;
    while (servers_track < web_servers.size())
    {
        if (web_servers[servers_track].checkServerFree() == "free")
        {
            inactive_servers++;
        }
        servers_track++;
    }
    return inactive_servers;
}