/**
 * @file LoadBalancer.cpp
 * @brief Implements load balancing simulation.
 *
 * @author Sullivan
 * @date July 2026
 */


#include "LoadBalancer.h"

#include <iostream>
#include <cstdlib>
#include <ctime>



/**
 * @brief Creates a load balancer.
 *
 * @param numberServers Starting servers
 */
LoadBalancer::LoadBalancer(int numberServers)
{

    srand(time(nullptr));

    clock = 0;

    minimumServers = numberServers;

    for(int i = 0; i < numberServers; i++)
    {
        servers.push_back(new WebServer(i + 1));
    }

    /*
       Initial queue size:
       servers * 100
    */
    for(int i = 0; i < numberServers * 100; i++)
    {
        requestQueue.enqueue(generateRequest());
    }

    logFile.open("simulation_log.txt");

}

/**
 * @brief Destructor.
 */
LoadBalancer::~LoadBalancer()
{

    for(auto server : servers)
    {
        delete server;
    }

    logFile.close();

}



/**
 * @brief Creates random IP address.
 */
std::string LoadBalancer::generateIP()
{

    return std::to_string(rand()%256)
    + "."
    + std::to_string(rand()%256)
    + "."
    + std::to_string(rand()%256)
    + "."
    + std::to_string(rand()%256);

}



/**
 * @brief Creates a random request.
 */
Request LoadBalancer::generateRequest()
{

    std::string input = generateIP();
    std::string output = generateIP();

    char type;


    if(rand()%2 == 0)
        type='p';

    else
        type='s';
    int time;

    if(type=='p')
        time = 10 + rand()%41;

    else
        time = 50 + rand()%151;

    return Request(input, output, time, type);

}



/**
 * @brief Adds another server.
 */
void LoadBalancer::addServer()
{

    int id = servers.size()+1;

    servers.push_back(new WebServer(id));

    logFile << "Added Server "
            << id
            << "\n";

}



/**
 * @brief Removes a server.
 */
void LoadBalancer::removeServer()
{

    if(servers.size() <= minimumServers)
        return;

    WebServer* removed = servers.back();

    logFile << "Removed Server "
            << removed->getID()
            << "\n";

    delete removed;

    servers.pop_back();

}

/**
 * @brief Sends queued requests to available servers.
 */
void LoadBalancer::distributeRequests()
{

    for(auto server : servers)
    {

        if(server->isIdle() &&
           !requestQueue.isEmpty())
        {

            Request request =
                requestQueue.dequeue();


            server->assignRequest(request);

        }

    }

}



/**
 * @brief Automatically adjusts servers.
 */
void LoadBalancer::balanceServers()
{

    int queueSize = requestQueue.size();

    if(queueSize > servers.size()*120)
    {
        addServer();
    }

    if(queueSize < servers.size()*20)
    {
        removeServer();
    }

}

/**
 * @brief Runs simulation.
 *
 * @param cycles Number of cycles
 */
void LoadBalancer::run(int cycles)
{

    for(clock = 1; clock <= cycles; clock++)
    {

        /*
          Randomly create new requests
        */
        if(rand()%100 < 30)
        {
            requestQueue.enqueue(generateRequest());
        }

        for(auto server : servers)
        {
            server->processCycle();
        }

        distributeRequests();

        balanceServers();

        logFile
        << "Clock Cycle: "
        << clock
        << "\n";

        logFile
        << "Servers: "
        << servers.size()
        << "\n";

        logFile
        << "Queue Size: "
        << requestQueue.size()
        << "\n\n";
    }
}



/**
 * @brief Returns queue size.
 */
int LoadBalancer::getQueueSize()
{
    return requestQueue.size();
}



/**
 * @brief Returns server count.
 */
int LoadBalancer::getServerCount()
{
    return servers.size();
}