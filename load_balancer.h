/**
 * @file load_balancer.h
 * @brief Defines the Load_Balancer class for managing the load balancing system.
 * @details This header file declares the Load_Balancer class which orchestrates
 *          the entire load balancing simulation, managing web servers, request
 *          queues, firewall security, and dynamic scaling of server resources.
 * @author Arjun Rajesh - CSCE 412 Project 3
 * @date 2026
 */

#ifndef Load_Balancer_H
#define Load_Balancer_H
#include <string>
#include "request.h"
#include "web_server.h"
#include "ip_firewall.h"
#include <vector>
#include <queue>
#include <fstream>
using std::string;
using std::vector;
using std::queue;
using std::ofstream;

/**
 * @class Load_Balancer
 * @brief Main orchestrator for the load balancing simulation system.
 * @details This class manages multiple web servers, maintains request queues,
 *          implements firewall security, and dynamically scales server resources
 *          based on queue load. It simulates a complete load balancing system
 *          over multiple clock cycles.
 */
class Load_Balancer
{
    public:
        /**
         * @brief Constructor for Load_Balancer class.
         * @details Initializes the load balancer with a specified number of web servers,
         *          creates an initial queue of requests, and sets up logging.
         * @param server_total The initial number of web servers to create
         */
        Load_Balancer(int server_total);
        
        /**
         * @brief Processes one complete clock cycle of the load balancer.
         * @details Handles request processing on all servers, assigns new requests,
         *          manages server scaling, and logs progress data. Should be called
         *          repeatedly to simulate the system.
         */
        void totalCycleHandle();
        
        /**
         * @brief Outputs comprehensive statistics at simulation completion.
         * @details Generates final summary statistics including total requests processed,
         *          server scaling history, firewall statistics, and performance metrics.
         *          Outputs to both console and log file.
         */
        void outputEndingData();
    
    private:
        /** @brief Vector of all web servers managed by the load balancer */
        vector<Web_Server> web_servers;
        
        /** @brief Queue of requests waiting to be assigned to servers */
        queue<Request> req_queue;
        
        /** @brief Firewall instance for threat detection on incoming requests */
        IP_Firewall firewall_check;
        
        /** @brief Output file stream for logging simulation events */
        ofstream log;
        
        /** @brief Current clock cycle number in the simulation */
        int clock_cycle;
        
        /** @brief Total number of requests successfully completed */
        int requests_finished;
        
        /** @brief Total number of requests allowed through the firewall */
        int requests_allowed;
        
        /** @brief Total number of requests blocked by the firewall */
        int requests_blocked;
        
        /** @brief Cool-down period counter between server scaling operations */
        int rest_period;
        
        /** @brief Initial size of the request queue at simulation start */
        int beginning_reqqueue_size;
        
        /** @brief Initial number of servers at simulation start */
        int beginning_server_size;
        
        /** @brief Maximum number of servers present at any point during simulation */
        int max_servers;
        
        /** @brief Total count of processing-type ('p') jobs created */
        int p_jobtype_total;
        
        /** @brief Total count of streaming-type ('s') jobs created */
        int s_jobtype_total;
        
        /** @brief Total number of servers added during the simulation */
        int servers_added;
        
        /** @brief Total number of servers removed during the simulation */
        int servers_removed;
        
        /** @brief Count of successfully added random requests */
        int randomRequestSuccesfulAdded;
        
        /** @brief Count of random requests that failed to be added */
        int randomRequestFailedAdded;
        
        /**
         * @brief Adds a new server to the pool when queue load is high.
         * @details Server is added when queue size exceeds 80 times the current
         *          number of servers. Updates max_servers tracking if needed.
         */
        void addServer();
        
        /**
         * @brief Removes a server from the pool when queue load is low.
         * @details Server is removed when queue size falls below 50 times the
         *          current number of servers.
         */
        void removeServer();
        
        /**
         * @brief Creates a new request with random attributes.
         * @details Generates a request with random incoming/outgoing IPs,
         *          processing time (2-77 cycles), and job type (50/50 p or s).
         * @return A newly created Request object with random attributes
         */
        Request createRandomRequest();
        
        /**
         * @brief Generates a random IP address string.
         * @details Creates a valid IP address in dotted-decimal notation
         *          with each octet being a random value from 0-255.
         * @return String representation of a random IP address
         */
        string randomIPGenerator();
        
        /**
         * @brief Adds a random request to the queue with 10% probability.
         * @details Called each cycle, has a 1-in-10 chance of creating and
         *          queuing a new random request.
         */
        void addRandomRequest();
        
        /**
         * @brief Manages the cool-down period between server scaling operations.
         * @details Prevents rapid consecutive server additions/removals by
         *          implementing a rest period counter.
         */
        void handleRestPeriod();
        
        /**
         * @brief Outputs initial simulation parameters and statistics.
         * @details Displays and logs starting configuration data including
         *          initial queue size, number of servers, and processing time ranges.
         */
        void outputBeginningData();
        
        /**
         * @brief Outputs periodic progress statistics during simulation.
         * @details Logs detailed statistics at regular intervals to track
         *          simulation progress and operational metrics.
         */
        void outputProgressiveData();
        
        /**
         * @brief Counts the total number of active (busy) servers.
         * @return Integer count of servers currently processing requests
         */
        int totalActiveServers();
        
        /**
         * @brief Counts the total number of inactive (free) servers.
         * @return Integer count of servers currently idle and waiting for requests
         */
        int totalInactiveServers();
};


#endif