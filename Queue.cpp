/* 8/31/98 Mary Elaine Califf
 * a C++ array implementation of queues
 */
// Modifications:
//    8/20/2003 Mary Elaine Califf
//        Changed to ANSI C++
// Revised 12/25/2020
// _____ and Mary Elaine Califf S2021
//Implementation of a queue of CustomerData objects
// Jaden Winterpacht

#include <iostream>
#include "Queue.h"

using namespace std;

Queue::Queue() // constructor
{
  this->head = nullptr;
  this->tail = nullptr;
  size = 0;
}

Queue::~Queue() // destructor
{
  clear();
}

Queue::Queue(const Queue & rhs) // copy constructor
{
  copy(rhs);
}

Queue& Queue::operator=(const Queue & rhs) // copy assignment
{
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

void Queue::enqueue(CustomerData newValue) // add newValue to queue
{
  if (head == nullptr) // empty queue
  {
    size = 1;
    Node* insertNode = new Node(newValue, nullptr);
    head = insertNode;
    tail = insertNode;
  }
  else // standard insert logic
  {
    size++;
    Node* insertNode = new Node(newValue, nullptr);
    tail->next = insertNode;
    tail = insertNode;
  }
}

CustomerData Queue::dequeue()
{
  // Return the customer attribute of the head node and delete the head node
  Node* deleteNode = head;
  CustomerData retVal = head->customer;
  head = head->next;
  delete deleteNode;
  size--;

  return retVal;
}

void Queue::copy(const Queue& rhs)
{
  // Initialize instance variables
  head = nullptr;
  tail = nullptr;
  size = 0;

  // Current node being copied
  Node* currNode = rhs.head;

  // Traverse through the queue and copy each node
  while (currNode != nullptr) {
    Node* insertNode = new Node(currNode->customer, nullptr);

    if (head == nullptr) {
      head = insertNode;
    } else {
      tail->next = insertNode;
    }

    tail = insertNode;
    currNode = currNode->next;
    size++;
  }
}

void Queue::clear()
{
  Node* currNode = head;
  // Traverse the list and delete each node
  while (currNode != nullptr) {
    Node* deleteNode = currNode;
    currNode = currNode->next;
    delete deleteNode;
  }
}
