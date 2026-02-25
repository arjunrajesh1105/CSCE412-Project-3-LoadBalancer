#include "request.h"
#include "load_balancer.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
using std::cout;
using std::cin;
using std::endl;

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
    Load_Balancer loadbalancer(server_input);

    while (clock_cycle_track < cycle_input)
    {
        loadbalancer.totalCycleHandle();
        clock_cycle_track++;
    }
}