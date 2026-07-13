/**
 * @file Main.cpp
 * @brief Driver program for the Load Balancer simulation.
 *
 * Creates three LoadBalancer instances: one dedicated to
 * processing ('p') jobs, one dedicated to streaming ('s') jobs,
 * and a dispatcher that generates new requests, routes each one
 * to the correct sub load balancer by job type, and drives both
 * sub load balancers forward one cycle at a time.
 *
 * @author Sullivan
 * @date July 2026
 */
#include "LoadBalancer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

/**
 * @brief Prompts for a positive integer.
 *
 * @param prompt Text shown to the user
 * @return A validated positive integer
 */
int readPositiveInt(const std::string& prompt)
{
    int value;

    std::cout << prompt;
    std::cin >> value;

    while(value <= 0)
    {
        std::cout << "Enter a positive number: ";
        std::cin >> value;
    }

    return value;
}

/**
 * @brief Main driver function.
 *
 * @return Exit status
 */
int main()
{

    /*
      Seed the random number generator exactly once here. If each
      LoadBalancer seeded its own generator in its constructor,
      the processing and streaming instances (created within the
      same second) would produce identical "random" sequences.
    */
    srand(static_cast<unsigned int>(time(nullptr)));

    std::cout << "=============================\n";
    std::cout << " Load Balancer Simulation\n";
    std::cout << "=============================\n\n";

    int processingServers =
        readPositiveInt("Enter number of processing web servers: ");

    int streamingServers =
        readPositiveInt("Enter number of streaming web servers: ");

    int cycles =
        readPositiveInt("Enter simulation time (clock cycles): ");

    std::cout << "\nStarting simulation...\n";

    LoadBalancer processingLB(processingServers, LBMode::PROCESSING, "processing_log.txt");
    LoadBalancer streamingLB(streamingServers, LBMode::STREAMING, "streaming_log.txt");
    LoadBalancer dispatcherLB(&processingLB, &streamingLB, "dispatcher_log.txt");

    dispatcherLB.run(cycles);

    /*
      Write a run summary to each log file (per-instance, plus a
      combined view on the dispatcher's log).
    */
    processingLB.logSummary();
    streamingLB.logSummary();
    dispatcherLB.logSummary();

    std::cout << "\nSimulation Complete!\n\n";

    std::cout << "===== Combined Summary (all "
              << dispatcherLB.getInitialServerCount()
              << " servers) =====\n";

    std::cout
        << "Starting Queue Size: "
        << dispatcherLB.getInitialQueueSize()
        << "\n";

    std::cout
        << "Ending Queue Size: "
        << dispatcherLB.getQueueSize()
        << "\n";

    std::cout
        << "Starting Server Count: "
        << dispatcherLB.getInitialServerCount()
        << "\n";

    std::cout
        << "Ending Server Count: "
        << dispatcherLB.getServerCount()
        << "\n";

    std::cout
        << "Total Servers Added: "
        << dispatcherLB.getServersAdded()
        << "\n";

    std::cout
        << "Total Servers Removed: "
        << dispatcherLB.getServersRemoved()
        << "\n";

    std::cout << "\n----- Processing -----\n";
    std::cout << "Starting Queue: " << processingLB.getInitialQueueSize()
              << "  Ending Queue: " << processingLB.getQueueSize() << "\n";
    std::cout << "Starting Servers: " << processingLB.getInitialServerCount()
              << "  Ending Servers: " << processingLB.getServerCount() << "\n";
    std::cout << "Added: " << processingLB.getServersAdded()
              << "  Removed: " << processingLB.getServersRemoved() << "\n";

    std::cout << "\n----- Streaming -----\n";
    std::cout << "Starting Queue: " << streamingLB.getInitialQueueSize()
              << "  Ending Queue: " << streamingLB.getQueueSize() << "\n";
    std::cout << "Starting Servers: " << streamingLB.getInitialServerCount()
              << "  Ending Servers: " << streamingLB.getServerCount() << "\n";
    std::cout << "Added: " << streamingLB.getServersAdded()
              << "  Removed: " << streamingLB.getServersRemoved() << "\n";

    std::cout
        << "\nLogs saved to processing_log.txt, streaming_log.txt, and dispatcher_log.txt\n";
    std::cout
        << "(each log file now also ends with a Run Summary block)\n";

    return 0;

}
