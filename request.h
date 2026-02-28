/**
 * @file request.h
 * @brief Defines the Request structure for the load balancer system.
 * @details This file contains the Request struct which represents a network request
 *          with incoming and outgoing IP addresses, processing time, and job type.
 * @author Arjun Rajesh - CSCE 412 Project 3
 * @date 2026
 */

#ifndef Request_H
#define Request_H
#include <string>

/**
 * @struct Request
 * @brief Structure representing a network request in the load balancer system.
 * @details Encapsulates all necessary information for a network request including
 *          source and destination IP addresses, processing time requirements,
 *          and the type of job (processing or streaming).
 */
struct Request 
{
    /** @brief The incoming IP address of the request source */
    std::string incoming_IP;
    
    /** @brief The outgoing IP address where the request is sent */
    std::string outgoing_IP;
    
    /** @brief Time remaining to fully process the request (in clock cycles) */
    int time_track;
    
    /** @brief Job type: 'p' for processing, 's' for streaming */
    char job_type;
    
    /**
     * @brief Default constructor for Request struct.
     * @details Initializes all fields to safe default values:
     *          - incoming_IP and outgoing_IP to "0.0.0.0"
     *          - time_track to 0
     *          - job_type to 'p' (processing)
     */
    Request() : incoming_IP("0.0.0.0"), outgoing_IP("0.0.0.0"), time_track(0), job_type('p') {}
};
#endif