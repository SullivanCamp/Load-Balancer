/**
 * @file Request.h
 * @brief Defines the Request structure used by the load balancer.
 *
 * A Request represents a web request sent to the load balancer.
 * Each request contains source and destination IP addresses,
 * processing time, and a job type.
 *
 * @author Sullivan
 * @date July 2026
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @struct Request
 * @brief Stores information about a single web request.
 */
struct Request
{
    std::string ipIn;          /**< Incoming IP address */
    std::string ipOut;         /**< Destination IP address */
    int timeRemaining;         /**< Number of clock cycles needed */
    char jobType;              /**< Job type: p = processing, s = streaming */

    /**
     * @brief Default constructor.
     */
    Request();

    /**
     * @brief Parameterized constructor.
     *
     * @param input Source IP address
     * @param output Destination IP address
     * @param time Processing time
     * @param type Job type
     */
    Request(std::string input, std::string output, int time, char type);
};

#endif
