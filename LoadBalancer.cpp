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
 * @brief Creates a PROCESSING or STREAMING load balancer.
 *
 * @param numberServers Starting servers
 * @param lbMode Either LBMode::PROCESSING or LBMode::STREAMING
 * @param logFileName Log file path
 */
LoadBalancer::LoadBalancer(int numberServers, LBMode lbMode, const std::string& logFileName)
{

    mode = lbMode;

    clock = 0;

    minimumServers = numberServers;

    processingTarget = nullptr;
    streamingTarget = nullptr;

    serversAdded = 0;
    serversRemoved = 0;

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

    initialServerCount = servers.size();
    initialQueueSize = requestQueue.size();

    logFile.open(logFileName);

}



/**
 * @brief Creates a DISPATCHER load balancer.
 *
 * @param processingLB Sub load balancer that handles 'p' requests
 * @param streamingLB Sub load balancer that handles 's' requests
 * @param logFileName Log file path
 */
LoadBalancer::LoadBalancer(LoadBalancer* processingLB, LoadBalancer* streamingLB, const std::string& logFileName)
{

    mode = LBMode::DISPATCHER;

    clock = 0;

    minimumServers = 0;

    processingTarget = processingLB;
    streamingTarget = streamingLB;

    initialServerCount = 0;
    initialQueueSize = 0;
    serversAdded = 0;
    serversRemoved = 0;

    logFile.open(logFileName);

}



/**
 * @brief Destructor.
 */
LoadBalancer::~LoadBalancer()
{

    /*
      Note: in DISPATCHER mode, `servers` is always empty and
      processingTarget/streamingTarget are owned by whoever
      constructed this dispatcher (typically main), so they are
      intentionally not deleted here.
    */
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
 *
 * PROCESSING instances only ever create 'p' jobs and STREAMING
 * instances only ever create 's' jobs, since each is a dedicated
 * load balancer for that job type. A DISPATCHER has no job type
 * of its own, so it creates a random mix and routes accordingly.
 */
Request LoadBalancer::generateRequest()
{

    std::string input = generateIP();
    std::string output = generateIP();

    char type;

    if(mode == LBMode::PROCESSING)
        type = 'p';

    else if(mode == LBMode::STREAMING)
        type = 's';

    else if(rand()%2 == 0)
        type = 'p';

    else
        type = 's';

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

    serversAdded++;

    logFile << "Added Server "
            << id
            << "\n";

}



/**
 * @brief Removes a server.
 */
void LoadBalancer::removeServer()
{

    if((int)servers.size() <= minimumServers)
        return;

    WebServer* removed = servers.back();

    serversRemoved++;

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

    if(queueSize > (int)servers.size()*120)
    {
        addServer();
    }

    if(queueSize < (int)servers.size()*20)
    {
        removeServer();
    }

}



/**
 * @brief Writes one cycle's status to the log file.
 *
 * DISPATCHER instances log a combined summary of both targets
 * instead of server/queue stats of their own, since a dispatcher
 * owns no servers or queue directly.
 */
void LoadBalancer::logCycle()
{

    if(mode == LBMode::DISPATCHER)
    {

        logFile << "Clock Cycle: "
                << clock
                << "\n";

        logFile << "Processing Servers: "
                << processingTarget->getServerCount()
                << "\n";

        logFile << "Processing Queue Size: "
                << processingTarget->getQueueSize()
                << "\n";

        logFile << "Streaming Servers: "
                << streamingTarget->getServerCount()
                << "\n";

        logFile << "Streaming Queue Size: "
                << streamingTarget->getQueueSize()
                << "\n\n";

        return;

    }

    int activeServers = 0;
    int inactiveServers = 0;

    for(auto server : servers)
    {
        if(server->isIdle())
            inactiveServers++;
        else
            activeServers++;
    }

    logFile << "Clock Cycle: "
            << clock << "\n";

    logFile << "Total Servers: "
            << servers.size()
            << "\n";

    logFile << "Active Servers: "
            << activeServers
            << "\n";

    logFile << "Inactive Servers: "
            << inactiveServers
            << "\n";

    logFile << "Requests Waiting: "
            << requestQueue.size()
            << "\n\n";

}



/**
 * @brief Advances the simulation by one clock cycle.
 *
 * @param generateNew Whether to roll for a new random request this cycle.
 */
void LoadBalancer::runCycle(bool generateNew)
{

    clock++;

    if(mode == LBMode::DISPATCHER)
    {

        /*
          The dispatcher owns no servers of its own. It generates
          new requests, routes each one to the processing or
          streaming load balancer based on job type, then lets
          each of those advance by one cycle.
        */
        if(generateNew && rand()%100 < 30)
        {

            Request newRequest = generateRequest();

            if(newRequest.jobType == 'p')
                processingTarget->addRequest(newRequest);

            else
                streamingTarget->addRequest(newRequest);

        }

        processingTarget->runCycle(false);
        streamingTarget->runCycle(false);

        logCycle();

        return;

    }

    /*
      Randomly create new requests
    */
    if(generateNew && rand()%100 < 30)
    {
        requestQueue.enqueue(generateRequest());
    }

    for(auto server : servers)
    {
        server->processCycle();
    }

    distributeRequests();

    balanceServers();

    logCycle();

}



/**
 * @brief Runs simulation.
 *
 * @param cycles Number of cycles
 */
void LoadBalancer::run(int cycles)
{

    for(int i = 0; i < cycles; i++)
    {
        runCycle(true);
    }

}



/**
 * @brief Adds an externally-created request directly to the queue.
 */
void LoadBalancer::addRequest(Request request)
{
    requestQueue.enqueue(request);
}



/**
 * @brief Returns queue size.
 */
int LoadBalancer::getQueueSize()
{
    if(mode == LBMode::DISPATCHER)
        return processingTarget->getQueueSize() + streamingTarget->getQueueSize();

    return requestQueue.size();
}



/**
 * @brief Returns server count.
 */
int LoadBalancer::getServerCount()
{
    if(mode == LBMode::DISPATCHER)
        return processingTarget->getServerCount() + streamingTarget->getServerCount();

    return servers.size();
}



/**
 * @brief Returns this instance's mode.
 */
LBMode LoadBalancer::getMode()
{
    return mode;
}



/**
 * @brief Returns the queue size measured right after construction.
 */
int LoadBalancer::getInitialQueueSize()
{
    if(mode == LBMode::DISPATCHER)
        return processingTarget->getInitialQueueSize() + streamingTarget->getInitialQueueSize();

    return initialQueueSize;
}



/**
 * @brief Returns the server count measured right after construction.
 */
int LoadBalancer::getInitialServerCount()
{
    if(mode == LBMode::DISPATCHER)
        return processingTarget->getInitialServerCount() + streamingTarget->getInitialServerCount();

    return initialServerCount;
}



/**
 * @brief Returns the running total of servers added during the run.
 */
int LoadBalancer::getServersAdded()
{
    if(mode == LBMode::DISPATCHER)
        return processingTarget->getServersAdded() + streamingTarget->getServersAdded();

    return serversAdded;
}



/**
 * @brief Returns the running total of servers removed during the run.
 */
int LoadBalancer::getServersRemoved()
{
    if(mode == LBMode::DISPATCHER)
        return processingTarget->getServersRemoved() + streamingTarget->getServersRemoved();

    return serversRemoved;
}



/**
 * @brief Writes an end-of-run summary block to this instance's log file.
 */
void LoadBalancer::logSummary()
{

    logFile << "\n";
    logFile << "===== Run Summary =====\n";

    logFile << "Starting Queue Size: "
            << getInitialQueueSize()
            << "\n";

    logFile << "Ending Queue Size: "
            << getQueueSize()
            << "\n";

    logFile << "Starting Server Count: "
            << getInitialServerCount()
            << "\n";

    logFile << "Ending Server Count: "
            << getServerCount()
            << "\n";

    logFile << "Total Servers Added: "
            << getServersAdded()
            << "\n";

    logFile << "Total Servers Removed: "
            << getServersRemoved()
            << "\n";

    logFile << "========================\n";

}
