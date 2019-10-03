/* Circular Queue */
#include "Queue.h"

#include <vector>
#include <iostream>

using namespace std;

Queue::Queue(char queueID)
{
    this->queueID = queueID;
    front = -1, rear = -1;
    totalNoOfCustomersInQueue = 0;
}

// Return true if the queue is full
bool Queue::isFull() { return totalNoOfCustomersInQueue == MAX_SIZE; }

// Return true if the queue is empty
bool Queue::isEmpty() { return (front == -1) ? true : false; }

// Add the CustomerID to the queue
void Queue::enQueue(int customerID)
{
    if (isFull())
    {
        cout << "\nQueue is Full. Error adding Customer ID : " << customerID << endl;
    }
    else
    {
        if (front == -1)
        {
            front = 0;
        }
        rear = (rear + 1) % MAX_SIZE;
        customerIDInQueue[rear] = customerID;
        totalNoOfCustomersInQueue++;
    }
}

// Remove the CustomerID LEAST recently added to the queue
int Queue::deQueue()
{
    int customerID;
    int frontIndexBeforeDeleteion = front; // TBD
    if (isEmpty())
    {
        cout << "\nQueue is empty" << endl;
        return -1;
    }
    else
    {
        customerID = customerIDInQueue[front];
        if (front == rear)
        {
            // The queue has only 1 customerID, hence we reset the queue after deleting it.
            front = -1;
            rear = -1;
        }
        else
        {
            front = (front + 1) % MAX_SIZE;
        }
    }
    totalNoOfCustomersInQueue--;
    return customerID;
}

// Return the CustomerID LEAST recently added to the queue BUT DO NOT REMOVE IT
int Queue::getFront() { return customerIDInQueue[front]; }

// Return no. of Customers in the queue
int Queue::getTotalNoOfCustomersInQueue() const { return totalNoOfCustomersInQueue; }
