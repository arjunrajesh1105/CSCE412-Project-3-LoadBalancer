#ifndef IP_Firewall_H
#define IP_Firewall_H
#include <string>
using std::string;

class IP_Firewall
{
    public:
        string firewallAnalysis(string IP_address); // This functions checks if the IP address of the incoming request is a threat or not (threat or safe)

    private:
        string threat_IP_octets[5] = {"73.", "97.", "91.", "23.", "185."}; // This vector contains the beginning octets of dangerous IP addresses that should be blocked
        int threat_array_count = 5; // This expresses the size of the array of dangerous beginning octets of IP addresses
        int IP_block_count = 0; // This expresses the total number of blocked IP addresses and will update whenever an IP address is blocked
};


#endif