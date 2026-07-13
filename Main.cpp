/**
 * @file main.cpp
 * @brief Driver program for the Load Balancer simulation.
 *
 * Creates a LoadBalancer object and runs the simulation
 * based on user input.
 *
 * @author Sullivan
 * @date July 2026
 */
#include "LoadBalancer.h"
#include <iostream>

/**
 * @brief Main driver function.
 *
 * @return Exit status
 */
int main()
{

    int servers;
    int cycles;

    std::cout << "=============================\n";
    std::cout << " Load Balancer Simulation\n";
    std::cout << "=============================\n\n";

    std::cout 
        << "Enter number of web servers: ";

    std::cin >> servers;

    while(servers <= 0)
    {
        std::cout 
            << "Enter a positive number: ";

        std::cin >> servers;
    }

    std::cout
        << "Enter simulation time (clock cycles): ";

    std::cin >> cycles;

    while(cycles <= 0)
    {
        std::cout
            << "Enter a positive number: ";

        std::cin >> cycles;
    }

    std::cout << "\nStarting simulation...\n";
    LoadBalancer loadBalancer(servers);
    loadBalancer.run(cycles);

    std::cout << "\nSimulation Complete!\n";

    std::cout
        << "Final Servers: "
        << loadBalancer.getServerCount()
        << "\n";

    std::cout
        << "Final Queue Size: "
        << loadBalancer.getQueueSize()
        << "\n";

    std::cout
        << "\nLog saved to simulation_log.txt\n";

    return 0;

}