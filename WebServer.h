/**
 * @file WebServer.h
 * @brief Defines the WebServer class.
 *
 * A WebServer processes requests assigned by the LoadBalancer.
 * Each server can either be idle or processing a request.
 *
 * @author Sullivan
 * @date July 2026
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"


/**
 * @class WebServer
 * @brief Represents one simulated web server.
 */
class WebServer
{

private:

    int serverID;              /**< Unique server identifier */
    bool busy;                 /**< Server status */
    Request currentRequest;    /**< Request currently being processed */


public:
    /**
     * @brief Creates a web server.
     *
     * @param id Server identifier
     */
    WebServer(int id);

    /**
     * @brief Assigns a request to the server.
     *
     * @param request Request to process
     */
    void assignRequest(Request request);

    /**
     * @brief Processes one clock cycle.
     *
     * Decreases remaining request time.
     */
    void processCycle();

    /**
     * @brief Checks whether server is available.
     *
     * @return True if idle
     */
    bool isIdle();

    /**
     * @brief Gets server ID.
     *
     * @return Server ID
     */
    int getID();

    /**
     * @brief Returns current request.
     *
     * @return Current request
     */
    Request getRequest();

};

#endif
