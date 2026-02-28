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

class Load_Balancer
{
    public:
        Load_Balancer(int server_total); // This is the constructor
        void totalCycleHandle(); // This is the main function that will handle the cycle processing for the load balancer
        void outputEndingData(); // This outputs the data at the end of the simulation
    
    private:
        vector<Web_Server> web_servers; // This is the vector that contains the web servers that will handle the requests and be managed by the load balancer
        queue<Request> req_queue; // This is the queue that contains the requests that haven't been assigned to a server yet
        IP_Firewall firewall_check; // This is the firewall that will help determine if an incoming request is a threat or safe
        ofstream log;
        int clock_cycle; // This is the variable that will help to keep track of the current clock cycle for the load balancer
        int requests_finished;
        int requests_allowed;
        int requests_blocked;
        int rest_period; // This helps to have a rest between adding and removing servers
        int beginning_reqqueue_size;
        int beginning_server_size;
        int max_servers; // This is the max amount of servers that were present during the simulation
        int p_jobtype_total; // This is the total amount of p job type requests that were created during the simulation
        int s_jobtype_total; // This is the total amount of s job type requests that were created during the simulation
        int servers_added;
        int servers_removed;
        int randomRequestSuccesfulAdded;
        int randomRequestFailedAdded;
        void addServer(); // This allows for a server to be added when queue size is more than 80*servers
        void removeServer(); // This allows for a server to be removed when queue size is less than 50*servers
        Request createRandomRequest(); // This creates a random request with random attributes (incoming ip, outgoing ip, time track, and job type)
        string randomIPGenerator(); // This generates a random IP address that can be used for the incoming request (incoming ip and outgoing ip)
        void addRandomRequest(); // This adds a random request to the request queue at a random time
        void handleRestPeriod(); // This handles the rest period for adding and removing servers
        void outputBeginningData(); //This outputs the data at the beginning of the simulation
        void outputProgressiveData(); // This outputs the data progresively throughout the simulation
        int totalActiveServers(); // This returns the total number of active servers
        int totalInactiveServers(); // This returns the total number of inactive servers
        

};


#endif