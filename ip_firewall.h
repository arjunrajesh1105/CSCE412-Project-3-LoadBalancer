/**
 * @file ip_firewall.h
 * @brief Defines the IP_Firewall class for security threat detection.
 * @details This header file declares the IP_Firewall class which analyzes incoming
 *          IP addresses to determine if they are from known dangerous sources and
 *          should be blocked from accessing the system.
 * @author CSCE 412 Project 3
 * @date 2026
 */

#ifndef IP_Firewall_H
#define IP_Firewall_H
#include <string>
using std::string;

/**
 * @class IP_Firewall
 * @brief Provides firewall functionality to block dangerous IP addresses.
 * @details This class maintains a list of dangerous IP address prefixes and
 *          analyzes incoming request IP addresses to determine if they match
 *          known threats. Requests from dangerous IPs are blocked, while others
 *          are allowed through.
 * @author Arjun Rajesh - CSCE 412 Project 3
 * @date 2026
 */
class IP_Firewall
{
    public:
        /**
         * @brief Analyzes an IP address for threat status.
         * @details Checks if the provided IP address begins with any of the known
         *          dangerous IP prefixes. Uses character-by-character comparison
         *          of the IP prefix.
         * @param IP_address The IP address string to analyze (format: "###.###.###.###")
         * @return String "dangerous" if IP matches a threat prefix, "safe" otherwise
         */
        string firewallAnalysis(string IP_address);

    private:
        /**
         * @brief Array of dangerous IP address prefixes to block.
         * @details Contains the first octet(s) of IP addresses that should be
         *          considered threats and blocked from the system.
         */
        string threat_IP_octets[5] = {"73.", "97.", "91.", "23.", "185."};
        
        /** @brief The number of threat prefixes in the threat_IP_octets array */
        int threat_array_count = 5;
};

#endif