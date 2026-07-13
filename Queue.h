/**
 * @file Queue.h
 * @brief Defines a custom queue implementation.
 *
 * This queue stores Request objects using a linked list.
 * It demonstrates a real-world queue implementation.
 *
 * @author Sullivan
 * @date July 2026
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "Request.h"


/**
 * @class Queue
 * @brief FIFO queue for Request objects.
 */
class Queue
{

private:

    /**
     * @struct Node
     * @brief Linked-list node containing a request.
     */
    struct Node
    {
        Request data;
        Node* next;

        Node(Request request)
        {
            data = request;
            next = nullptr;
        }
    };


    Node* frontNode;
    Node* rearNode;
    int count;


public:

    /**
     * @brief Constructor.
     */
    Queue();


    /**
     * @brief Destructor.
     */
    ~Queue();


    /**
     * @brief Adds a request to the back of the queue.
     *
     * @param request Request to add
     */
    void enqueue(Request request);


    /**
     * @brief Removes and returns the front request.
     *
     * @return Removed request
     */
    Request dequeue();


    /**
     * @brief Checks if queue is empty.
     *
     * @return true if empty
     */
    bool isEmpty();


    /**
     * @brief Gets the number of requests.
     *
     * @return Queue size
     */
    int size();


    /**
     * @brief Returns the front request without removing it.
     *
     * @return Front request
     */
    Request peek();

};

#endif