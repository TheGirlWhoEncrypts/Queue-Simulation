#ifndef QUEUE_H
#define QUEUE_H

const int MAX_SIZE = 20;

class Queue
{
private:
  char queueID;
  int customerIDInQueue[MAX_SIZE];
  int front, rear; // Represent the array customerIDInQueue's Index
  int totalNoOfCustomersInQueue;

public:
  Queue(char queueID);
  bool isFull();                            // Return true if the queue is full
  bool isEmpty();                           // Return true if the queue is empty
  void enQueue(int customerID);             // Add the CustomerID to the queue
  int deQueue();                            // Remove the CustomerID LEAST recently added to the queue
  int getFront();                           // Return the CustomerID LEAST recently added to the queue BUT DO NOT REMOVE IT
  int getTotalNoOfCustomersInQueue() const; // Return no. of Customers in the queue
};
#endif
