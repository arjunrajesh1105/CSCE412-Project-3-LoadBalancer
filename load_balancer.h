#ifndef Load_Balancer_H
#define Load_Balancer_H
#include <string>
#include "request.h"
#include "web_server.h"
#include "ip_firewall.h"
#include <vector>
#include <queue>
using std::string;
using std::vector;
using std::queue;

class Load_Balancer
{
    public:
        Load_Balancer(int server_total); // This is the constructor
        void totalCycleHandle(); // This is the main function that will handle the cycle processing for the load balancer
    
    private:
        vector<Web_Server> web_servers; // This is the vector that contains the web servers that will handle the requests and be managed by the load balancer
        queue<Request> req_queue; // This is the queue that contains the requests that haven't been assigned to a server yet
        IP_Firewall firewall_check; // This is the firewall that will help determine if an incoming request is a threat or safe
        int clock_cycle; // This is the variable that will help to keep track of the current clock cycle for the load balancer
        int requests_finshed;
        int requests_blocked;
        int rest_period; // This helps to have a rest between adding and removing servers
        void addServer(); // This allows for a server to be added when queue size is more than 80*servers
        void removeServer(); // This allows for a server to be removed when queue size is less than 50*servers
        Request createRandomRequest(); // This creates a random request with random attributes (incoming ip, outgoing ip, time track, and job type)
        string randomIPGenerator(); // This generates a random IP address that can be used for the incoming request (incoming ip and outgoing ip)

};


#endif