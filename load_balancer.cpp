/**
 * @file load_balancer.cpp
 * @brief Implementation of the Load_Balancer class.
 * @details Contains implementations for all load balancer operations including
 *          server management, request queue handling, firewall integration, and
 *          statistics tracking.
 * @author Arjun Rajesh - CSCE 412 Project 3
 * @date 2026
 */

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

/**
 * @brief Constructor implementation for Load_Balancer class.
 * @details Initializes the load balancer system with initial servers and request queue.
 *          Opens log file, creates specified number of web servers, and populates
 *          initial request queue with randomly generated requests. All tracking
 *          variables are initialized to zero.
 * @param server_total The number of initial web servers to create
 * @note Beginning queue size is set to 100 times the number of initial servers
 * @see outputBeginningData()
 */
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

/**
 * @brief Processes one complete cycle of the load balancer simulation.
 * @details Executes the following operations:
 *          1. Increments clock cycle counter
 *          2. Processes all server requests (decrement time, update status)
 *          3. Assigns queued requests to free servers
 *          4. Applies firewall analysis to new requests
 *          5. Manages server scaling based on queue load
 *          6. Adds random requests with 10% probability
 *          7. Outputs progressive statistics every 5 cycles
 *
 *          Server scaling logic:
 *          - Adds server if queue size > 80 * current server count
 *          - Removes server if queue size < 50 * current server count
 *          - Respects rest period (cooldown) between scaling operations
 *
 * @see addServer(), removeServer(), handleRestPeriod(), addRandomRequest()
 */
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

/**
 * @brief Attempts to add a new web server to the pool.
 * @details Creates a new Web_Server with the next available server number,
 *          increments the servers_added counter, and updates max_servers if
 *          the new total exceeds the previous maximum. Logs the addition to
 *          the output log file.
 * @see removeServer()
 */
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

/**
 * @brief Removes the last web server from the pool.
 * @details Removes the server with the highest number (last in vector),
 *          increments the servers_removed counter, and logs the removal event.
 *          Should only be called when at least 2 servers exist in the pool.
 * @note Ensures minimum of 1 server is always available
 * @see addServer()
 */
void Load_Balancer::removeServer()
{
    int last_index = web_servers.size() - 1;
    web_servers.pop_back();
    servers_removed++;

    log << "CYCLE " << clock_cycle << ": SERVER REMOVED - Total Servers: " << web_servers.size() << endl;
    log << endl;
}

/**
 * @brief Creates a request with randomized attributes.
 * @details Generates a new Request with random incoming and outgoing IP addresses,
 *          processing time between 2-77 cycles, and job type (50% processing, 50% streaming).
 *          Increments appropriate job type counters (p_jobtype_total or s_jobtype_total).
 *
 * @return A newly created Request object with all fields randomly populated
 *
 * Job type distribution:
 * - 'p' (processing): 50% probability
 * - 's' (streaming): 50% probability
 *
 * @see randomIPGenerator()
 */
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

/**
 * @brief Generates a random IP address string.
 * @details Creates a valid IPv4 address in dotted-decimal notation (xxx.xxx.xxx.xxx)
 *          where each octet is a random value from 0 to 255 inclusive.
 *
 * @return String representation of a randomly generated IP address
 * @example randomIPGenerator() might return "192.45.231.56"
 */
string Load_Balancer::randomIPGenerator()
{
    int octet1 = rand() % 256;
    int octet2 = rand() % 256;
    int octet3 = rand() % 256;
    int octet4 = rand() % 256;
    return to_string(octet1) + "." + to_string(octet2) + "." + to_string(octet3) + "." + to_string(octet4);
}

/**
 * @brief Adds a randomized request to the queue probabilistically.
 * @details Called each cycle, this function has a 1-in-10 (10%) probability of
 *          creating and enqueueing a new random request. Tracks successful and
 *          failed attempts separately for statistical analysis.
 *
 * Probability: 10% chance to add a request per cycle
 * Success tracking: Increments randomRequestSuccesfulAdded or randomRequestFailedAdded
 *
 * @see createRandomRequest()
 */
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

/**
 * @brief Manages the cooldown period for server scaling operations.
 * @details Implements a rest period between consecutive server add/remove operations
 *          to prevent rapid server scaling fluctuations.
 *
 *          Scaling thresholds:
 *          - Add server: Queue size > 80 * (number of current servers)
 *          - Remove server: Queue size < 50 * (number of current servers)
 *          - Rest period: 50 cycles after each scaling operation
 *          - Minimum servers: Always maintains at least 1 server
 *
 *          Logic flow:
 *          1. If rest_period == 0, check scaling conditions
 *          2. If queue size indicates need for scaling, perform operation
 *          3. Set rest_period to 50 to prevent rapid consecutive scaling
 *          4. If rest_period > 0, decrement it each cycle
 *
 * @see addServer(), removeServer()
 */
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

/**
 * @brief Outputs initial simulation parameters and configuration.
 * @details Displays and logs the starting state of the load balancer system including:
 *          - Initial queue size (beginning_reqqueue_size)
 *          - Initial server count
 *          - Request processing time range (2-77 cycles)
 *
 *          Output is sent to both:
 *          1. Console (stdout) with colored formatting
 *          2. Log file (load_balancer_log.txt)
 *
 * @see outputProgressiveData(), outputEndingData()
 */
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

/**
 * @brief Outputs periodic progress statistics during simulation.
 * @details Logs detailed statistics at regular intervals:
 *          - Every 500 cycles: Console output with colored formatting
 *          - Every 5 cycles: File logging
 *
 *          Statistics tracked:
 *          - Current clock cycle number
 *          - Queue status (size)
 *          - Server status (current count, added, removed)
 *          - Request status (finished, blocked)
 *          - Random request metrics and percentage
 *
 * @note This provides real-time monitoring of system behavior without waiting for completion
 * @see outputBeginningData(), outputEndingData()
 */
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

/**
 * @brief Outputs comprehensive final statistics at simulation completion.
 * @details Generates and displays complete summary of the simulation including:
 *          - Total clock cycles executed
 *          - Request processing statistics (finished, blocked, allowed)
 *          - Server management metrics (added, removed, maximum)
 *          - Job type distribution (p vs s ratio)
 *          - Random request generation statistics
 *
 *          Output destinations:
 *          1. Console (stdout) with colored formatting for emphasis
 *          2. Log file (load_balancer_log.txt)
 *
 *          Color coding:
 *          - Blue: Headers
 *          - Green: Successful metrics
 *          - Red: Blocked requests
 *          - Yellow: Scaling and random request metrics
 *          - Orange: Clock cycle count
 *
 * @see outputBeginningData(), outputProgressiveData()
 */
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

/**
 * @brief Counts the number of servers currently processing requests.
 * @details Iterates through all servers and counts those with status "busy".
 *          This represents servers actively working on requests.
 *
 * @return Integer count of currently active (busy) servers
 * @see totalInactiveServers()
 */
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

/**
 * @brief Counts the number of servers currently idle and available.
 * @details Iterates through all servers and counts those with status "free".
 *          This represents servers waiting for requests to process.
 *
 * @return Integer count of currently inactive (free) servers
 * @see totalActiveServers()
 */
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