/**
 * @file Request.cpp
 * @brief Implements the Request structure constructors.
 *
 * @author Sullivan
 * @date July 2026
 */

#include "Request.h"

/**
 * @brief Creates an empty request.
 */
Request::Request()
{
    ipIn = "";
    ipOut = "";
    timeRemaining = 0;
    jobType = 'p';
}

/**
 * @brief Creates a request with supplied values.
 *
 * @param input Source IP
 * @param output Destination IP
 * @param time Processing time
 * @param type Job type
 */
Request::Request(std::string input, std::string output, int time, char type)
{
    ipIn = input;
    ipOut = output;
    timeRemaining = time;
    jobType = type;
}