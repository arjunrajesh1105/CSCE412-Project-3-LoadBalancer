#include "ip_firewall.h"
using std::string;

string IP_Firewall::firewallAnalysis(string IP_address)
{
    int octet_count = 0; // This variable is used to keep track of the index of the octet in the threat_IP_octets array
    int octet_character_count = 0; // This variable is used to keep track of the index of the character of the octet in the threat_IP_octets array and IP_address
    
    // This loop allows for each of the threat octets to be looped through and compared to the incoming IP address
    while (octet_count < threat_array_count)
    {
        // This loop allows for each character (number) of the octet to be checked with the IP_address character
        // If there is a match then the the loop continues to check the next characters and if there isn't a match then the loop breaks and goes to the next octet
        while (threat_IP_octets[octet_count].length() > octet_character_count) 
        {
            if (IP_address[octet_character_count] == threat_IP_octets[octet_count][octet_character_count]) // Checks the IP_address character and the octer character
            {
                octet_character_count++;
            }
            else
            {
                break; // If no match, then this octet isn't a threat and the loop breaks to check the next octet in the array
            }
        }

        // This is a check to see there was a match for all the characters in an iteration of the loop above
        // If there was a match then the octet_character_count should be equal to the lenght of the current octet iteration, and if so then it's a threat
        if (octet_character_count == threat_IP_octets[octet_count].length())
        {
            return "dangerous";
        }
        octet_count++;
        octet_character_count = 0; 
    }
    
    return "safe"; // If no match was found then that means the IP address is safe to be let through
}