/**
 * @file LoadBalancer.h
 * @brief Defines the LoadBalancer class.
 *
 * The LoadBalancer manages incoming requests,
 * distributes them to web servers, and dynamically
 * adjusts the number of servers.
 *
 * A LoadBalancer can run in one of three modes (see LBMode):
 * it can own a pool of servers and process one job type
 * directly (PROCESSING or STREAMING), or it can act purely
 * as a DISPATCHER that owns no servers of its own and instead
 * routes each incoming request to the appropriate sub
 * load balancer based on job type.
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
#include <string>


/**
 * @enum LBMode
 * @brief Identifies what kind of work a LoadBalancer instance manages.
 */
enum class LBMode
{
    PROCESSING, /**< Owns servers and only handles 'p' (processing) requests */
    STREAMING,  /**< Owns servers and only handles 's' (streaming) requests */
    DISPATCHER  /**< Owns no servers; routes requests to a processing and a streaming LoadBalancer */
};


/**
 * @class LoadBalancer
 * @brief Controls request distribution and server scaling.
 */
class LoadBalancer
{

private:


    LBMode mode;                        /**< What role this instance plays */

    Queue requestQueue;                 /**< Incoming requests (unused in DISPATCHER mode) */


    std::vector<WebServer*> servers;    /**< Active servers (empty in DISPATCHER mode) */


    int clock;                          /**< Simulation clock */


    int minimumServers;                 /**< Minimum allowed servers */


    std::ofstream logFile;              /**< Simulation output file */


    LoadBalancer* processingTarget;     /**< Sub load balancer for 'p' jobs (DISPATCHER mode only) */

    LoadBalancer* streamingTarget;      /**< Sub load balancer for 's' jobs (DISPATCHER mode only) */


    int initialQueueSize;               /**< Queue size right after construction */

    int initialServerCount;             /**< Server count right after construction */

    int serversAdded;                   /**< Running total of addServer() calls */

    int serversRemoved;                 /**< Running total of removeServer() calls */



    /**
     * @brief Creates a random IP address.
     *
     * @return Generated IP
     */
    std::string generateIP();



    /**
     * @brief Generates a random request.
     *
     * The job type produced depends on this instance's mode:
     * PROCESSING always produces 'p', STREAMING always produces
     * 's', and DISPATCHER produces a random mix of both.
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



    /**
     * @brief Writes one cycle's status to the log file.
     */
    void logCycle();



public:


    /**
     * @brief Creates a PROCESSING or STREAMING load balancer with its own servers.
     *
     * The initial request queue is pre-filled with numberServers * 100
     * requests matching this instance's job type.
     *
     * @param numberServers Initial number of servers
     * @param mode Either LBMode::PROCESSING or LBMode::STREAMING
     * @param logFileName Path to the log file this instance writes to
     */
    LoadBalancer(int numberServers, LBMode mode, const std::string& logFileName);



    /**
     * @brief Creates a DISPATCHER load balancer.
     *
     * A dispatcher owns no servers. Each cycle it may generate a
     * new random request and routes it to the processing or
     * streaming target based on job type, then advances both
     * targets by one cycle.
     *
     * @param processingLB Sub load balancer that handles 'p' requests
     * @param streamingLB Sub load balancer that handles 's' requests
     * @param logFileName Path to the log file this instance writes to
     */
    LoadBalancer(LoadBalancer* processingLB, LoadBalancer* streamingLB, const std::string& logFileName);



    /**
     * @brief Destructor.
     */
    ~LoadBalancer();



    /**
     * @brief Advances the simulation by one clock cycle.
     *
     * @param generateNew Whether this instance should roll for and
     *        possibly enqueue a brand-new random request this cycle.
     *        Dispatchers pass false to their targets since the
     *        dispatcher generates and routes new requests itself.
     */
    void runCycle(bool generateNew = true);



    /**
     * @brief Runs simulation for a number of cycles.
     *
     * @param cycles Number of clock cycles
     */
    void run(int cycles);



    /**
     * @brief Adds an externally-created request directly to this
     *        instance's queue, bypassing generateRequest().
     *
     * Used by a DISPATCHER to hand a request off to whichever
     * sub load balancer matches its job type.
     *
     * @param request Request to enqueue
     */
    void addRequest(Request request);



    /**
     * @brief Gets queue size.
     *
     * @return Number of waiting requests (sum of both targets if DISPATCHER)
     */
    int getQueueSize();



    /**
     * @brief Gets number of servers.
     *
     * @return Server count (sum of both targets if DISPATCHER)
     */
    int getServerCount();



    /**
     * @brief Gets this instance's mode.
     *
     * @return PROCESSING, STREAMING, or DISPATCHER
     */
    LBMode getMode();



    /**
     * @brief Gets the queue size measured right after construction.
     *
     * @return Starting queue size (sum of both targets if DISPATCHER)
     */
    int getInitialQueueSize();



    /**
     * @brief Gets the server count measured right after construction.
     *
     * @return Starting server count (sum of both targets if DISPATCHER)
     */
    int getInitialServerCount();



    /**
     * @brief Gets the running total of servers added during the run.
     *
     * @return Total servers added (sum of both targets if DISPATCHER)
     */
    int getServersAdded();



    /**
     * @brief Gets the running total of servers removed during the run.
     *
     * @return Total servers removed (sum of both targets if DISPATCHER)
     */
    int getServersRemoved();



    /**
     * @brief Writes an end-of-run summary block to this instance's log file.
     *
     * Includes starting/ending queue size, starting/ending server
     * count, and total servers added/removed over the run.
     */
    void logSummary();


};

#endif
