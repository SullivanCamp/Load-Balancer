/**
 * @file WebServer.cpp
 * @brief Implements the WebServer class.
 *
 * @author Sullivan
 * @date July 2026
 */


#include "WebServer.h"

/**
 * @brief Creates a server.
 *
 * @param id Server number
 */
WebServer::WebServer(int id)
{
    serverID = id;
    busy = false;
}

/**
 * @brief Assigns work to this server.
 *
 * @param request Incoming request
 */
void WebServer::assignRequest(Request request)
{
    currentRequest = request;
    busy = true;
}

/**
 * @brief Runs one clock cycle of processing.
 */
void WebServer::processCycle()
{

    if(busy)
    {
        currentRequest.timeRemaining--;

        if(currentRequest.timeRemaining <= 0)
        {
            busy = false;
        }
    }

}

/**
 * @brief Checks server availability.
 *
 * @return True if not processing
 */
bool WebServer::isIdle()
{
    return !busy;
}

/**
 * @brief Gets server number.
 *
 * @return ID
 */
int WebServer::getID()
{
    return serverID;
}

/**
 * @brief Returns current request.
 *
 * @return Request
 */
Request WebServer::getRequest()
{
    return currentRequest;
}
