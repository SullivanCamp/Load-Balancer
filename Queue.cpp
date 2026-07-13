/**
 * @file Queue.cpp
 * @brief Implements the custom Request queue.
 *
 * @author Sullivan
 * @date July 2026
 */


#include "Queue.h"
#include <stdexcept>

/**
 * @brief Creates an empty queue.
 */
Queue::Queue()
{
    frontNode = nullptr;
    rearNode = nullptr;
    count = 0;
}

/**
 * @brief Deletes all nodes.
 */
Queue::~Queue()
{
    while(!isEmpty())
    {
        dequeue();
    }
}

/**
 * @brief Adds a request to the queue.
 *
 * @param request Request being added
 */
void Queue::enqueue(Request request)
{
    Node* newNode = new Node(request);

    if(isEmpty())
    {
        frontNode = newNode;
        rearNode = newNode;
    }

    else
    {
        rearNode->next = newNode;
        rearNode = newNode;
    }


    count++;
}

/**
 * @brief Removes the front request.
 *
 * @return Removed request
 */
Request Queue::dequeue()
{

    if(isEmpty())
    {
        throw std::runtime_error("Queue is empty");
    }

    Node* temp = frontNode;

    Request value = temp->data;

    frontNode = frontNode->next;

    if(frontNode == nullptr)
    {
        rearNode = nullptr;
    }

    delete temp;

   count--;

    return value;
}

/**
 * @brief Checks whether queue contains requests.
 *
 * @return True if empty
 */
bool Queue::isEmpty()
{
    return count == 0;
}

/**
 * @brief Returns number of requests.
 *
 * @return Queue size
 */
int Queue::size()
{
    return count;
}

/**
 * @brief Returns the next request.
 *
 * @return Front request
 */
Request Queue::peek()
{
    if(isEmpty())
    {
        throw std::runtime_error("Queue is empty");
    }

    return frontNode->data;
}
