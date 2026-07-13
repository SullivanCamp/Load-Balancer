/**
 * @file LoadBalancer.h
 * @brief Defines the LoadBalancer class.
 *
 * The LoadBalancer manages incoming requests,
 * distributes them to web servers, and dynamically
 * adjusts the number of servers.
 *
 * @author Sullivan
 * @date July 2026
 */


#ifndef LOADBALANCER_H
#define LOADBALANCER_H


#include "Queue.h"
#include "WebServer.h"

#include <vector>
#include <fstream>



/**
 * @class LoadBalancer
 * @brief Controls request distribution and server scaling.
 */
class LoadBalancer
{

private:


    Queue requestQueue;                 /**< Incoming requests */


    std::vector<WebServer*> servers;    /**< Active servers */


    int clock;                          /**< Simulation clock */


    int minimumServers;                 /**< Minimum allowed servers */


    std::ofstream logFile;              /**< Simulation output file */



    /**
     * @brief Creates a random IP address.
     *
     * @return Generated IP
     */
    std::string generateIP();



    /**
     * @brief Generates a random request.
     *
     * @return New request
     */
    Request generateRequest();



    /**
     * @brief Adds a server.
     */
    void addServer();



    /**
     * @brief Removes a server.
     */
    void removeServer();



    /**
     * @brief Assigns waiting requests.
     */
    void distributeRequests();



    /**
     * @brief Balances number of servers.
     */
    void balanceServers();



public:


    /**
     * @brief Creates a load balancer.
     *
     * @param numberServers Initial number of servers
     */
    LoadBalancer(int numberServers);



    /**
     * @brief Destructor.
     */
    ~LoadBalancer();



    /**
     * @brief Runs simulation.
     *
     * @param cycles Number of clock cycles
     */
    void run(int cycles);



    /**
     * @brief Gets queue size.
     *
     * @return Number of waiting requests
     */
    int getQueueSize();



    /**
     * @brief Gets number of servers.
     *
     * @return Server count
     */
    int getServerCount();


};

#endif