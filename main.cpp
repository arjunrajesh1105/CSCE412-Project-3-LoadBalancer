/**
 * @file main.cpp
 * @brief Entry point for the load balancer simulation program.
 * @details This file contains the main function which initializes and executes
 *          the load balancer simulation. It prompts the user for configuration
 *          parameters and orchestrates the simulation execution.
 * @author Arjun Rajesh - CSCE 412 Project 3
 * @date 2026
 */

#include "load_balancer.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
using std::cout;
using std::cin;
using std::endl;

/**
 * @brief Main entry point for the load balancer simulation.
 * @details Prompts the user to input:
 *          1. Number of web servers to create initially
 *          2. Number of clock cycles to simulate
 *
 *          Then initializes the Load_Balancer with the specified number of servers
 *          and executes the simulation for the specified number of clock cycles.
 *          After completion, outputs final statistics.
 *
 *          The simulation proceeds as follows:
 *          1. Seed random number generator with current time
 *          2. Get user input for server count and cycle count
 *          3. Create Load_Balancer instance (initializes servers and request queue)
 *          4. Execute totalCycleHandle() for each cycle
 *          5. Output final statistics and data
 *
 * @return int - Exit status (0 for successful completion)
 *
 * @note The Load_Balancer constructor outputs initial data, and outputEndingData()
 *       displays the final summary statistics to console and log file.
 *
 * @see Load_Balancer::Load_Balancer(), Load_Balancer::totalCycleHandle(),
 *      Load_Balancer::outputEndingData()
 */
int main() 
{
    int server_input;
    int cycle_input;
    int clock_cycle_track = 0;
    srand(time(0));

    cout << "Please input the amount of web servers you would like: " << endl;
    cin >> server_input;
    cout << "Please input the amount of clock cycles you would like: " << endl;
    cin >> cycle_input;
    cout << endl;
    Load_Balancer loadbalancer(server_input);

    while (clock_cycle_track < cycle_input)
    {
        loadbalancer.totalCycleHandle();
        clock_cycle_track++;
    }
    
    loadbalancer.outputEndingData();
}