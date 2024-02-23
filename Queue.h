/* 8/31/98 Mary Elaine Califf
 * a C++ array implementation of a queue of integers which may have length of up to 100
 *
 * Revised 12/25/2020
 */
 // _____ and Mary Elaine Califf S2021
 //Header file for a queue of CustomerData objects
 // Jaden Winterpacht

#ifndef QUEUE_H
#define QUEUE_H
#include "CustomerData.h"

class Queue
{
  
public:

  // constructor
  Queue();

  // destructor
  ~Queue();

  // copy constructor
  Queue(const Queue & rhs);

  // copy assignment
  Queue& operator=(const Queue & rhs);

  // getter for size
  int getSize() { return size; }

  // is the queue empty
  bool isEmpty() { return (head == nullptr); }

  // adds a new value to the queue
  void enqueue(CustomerData newValue);

  // removes a value from the queue and returns it
  // Precondition: queue must not be empty
  CustomerData dequeue();

  // return item at the front of the queue without removing it
  // Precondtion: queue must not be empty
  CustomerData front() { return head->customer; }

private:
  // Struct for nodes of singly-linked list
  struct Node {
    CustomerData customer;
    Node* next;

    // Constructors
    Node() {}
    Node(CustomerData cust, Node* nextPtr = nullptr) : customer(cust), next(nextPtr) {}
  };

  // instance variables
  Node* head;
  Node* tail;
  int size;

  // Makes a deep copy of Queue data
  void copy(const Queue& rhs);
  // Frees dynamic memory
  void clear();

};

#endif
