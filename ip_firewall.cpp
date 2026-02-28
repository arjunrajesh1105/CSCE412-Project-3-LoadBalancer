/**
 * @file ip_firewall.cpp
 * @brief Implementation of the IP_Firewall class.
 * @details Contains the implementation of the firewall analysis method that
 *          detects and blocks requests from dangerous IP addresses.
 * @author Arjun Rajesh - CSCE 412 Project 3
 * @date 2026
 */

#include "ip_firewall.h"
using std::string;

/**
 * @brief Analyzes an IP address to determine if it's from a dangerous source.
 * @details Performs a prefix comparison of the provided IP address against
 *          known dangerous IP prefixes stored in the threat_IP_octets array.
 *          Uses character-by-character matching for prefix detection.
 *
 *          Algorithm:
 *          1. Loop through each threat octet prefix in the array
 *          2. For each threat prefix, compare characters with the IP address
 *          3. If all characters match the threat prefix length, IP is dangerous
 *          4. If any character doesn't match, move to the next threat prefix
 *          5. If no threat prefixes match, the IP is safe
 *
 * @param IP_address String representation of the IP address to analyze
 *                   Expected format: "###.###.###.###" (standard dotted-decimal)
 * @return String value:
 *         - "dangerous" if the IP address matches a known threat prefix
 *         - "safe" if the IP address does not match any threat prefixes
 *
 * @example
 * IP_Firewall firewall;
 * string result = firewall.firewallAnalysis("73.45.67.89");  // Returns "dangerous"
 * string result = firewall.firewallAnalysis("192.168.1.1");  // Returns "safe"
 */
string IP_Firewall::firewallAnalysis(string IP_address)
{
    int octet_count = 0;           /**< Index of the current threat octet being checked */
    int octet_character_count = 0; /**< Index of the current character being compared */
    
    // Loop through each threat octet prefix
    while (octet_count < threat_array_count)
    {
        // Compare each character of the threat prefix with the IP address
        while (threat_IP_octets[octet_count].length() > octet_character_count) 
        {
            // Check if characters match
            if (IP_address[octet_character_count] == threat_IP_octets[octet_count][octet_character_count])
            {
                octet_character_count++;
            }
            else
            {
                // No match for this threat prefix, move to next one
                break;
            }
        }

        // Check if all characters in this threat prefix matched
        if (octet_character_count == threat_IP_octets[octet_count].length())
        {
            return "dangerous"; // IP prefix matches a threat - block it
        }
        
        octet_count++;
        octet_character_count = 0; // Reset for next threat prefix
    }
    
    return "safe"; // No dangerous prefix match found - allow the request
}