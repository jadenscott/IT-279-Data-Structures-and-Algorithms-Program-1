//IT 279 GRADING - Nick Dunne
//Spring 2021 Testing Program for a Queue of integers

/**
  class Queue {
    Queue();
    int getSize();
    bool isEmpty();
    bool enqueue(CustomerData newValue);
    CustomerData dequeue();
    CustomerData front();
  }
}
**/

#include "Queue.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//RUNNER: Will run one test at a time, based on the test name given.
bool runSpecificTest(string testName);

//Check contents and print out any differences, removes items from the queue
bool validateAndPrintContents(Queue& queue, int numItems, CustomerData* items, bool validateList);

//Method used to return a dynamically allocated queue with some items in it
Queue& getQueue(CustomerData arr[], int numItems);

bool testEmptyConstructor();

bool testAddOneCustomerData();

bool testRemoveOneCustomerData();

bool testAddAndRemoveManyCustomerData();

bool testIsEmpty();

bool testCopyConstructor();

bool testcopyConstructorIsDeepCopy();

bool testCopyConstructorEmptyQueue();

bool testAddAfterCopyConstructor();

bool testAssignmentOperator();

bool testassignmentOperatorIsDeepCopy();

bool testAssignmentOperatorSelfCopy();

bool testAddAfterAssignmentOperator();

bool testAssignmentOperatorEmptyQueue();

int main(int argc, char** argv)
{
    // Collection of test names
    vector<string> allTests = {
            "addOneCustomer", "removeOneCustomer",
            "addAndRemoveManyCustomers", "isEmpty",
            "copyConstructor", "copyConstructorIsDeepCopy", "addAfterCopy",
            "assignmentOperator", "assignmentOperatorIsDeepCopy", "assignmentOperatorSelfCopy", "addAfterAssignment",
            "copyConstructorEmptyQueue", "assignmentOperatorEmptyQueue"
    };

    cout << "EMPTY CONSTRUCTOR TEST RUNNING..." << endl;
    bool constructor = testEmptyConstructor();

    //If the empty constructor test passes, run the rest of the tests.
    if (constructor)
    {
        if (argc > 1)
        {
            string testName = argv[1];
            //Run the test
            if (testName != "constructor")
            {
                runSpecificTest(testName);
            }
        }
        else
        {
            bool passedPreviousTest = true;
            int testIndex = 0;

            //Since a test name wasn't passed in, we will run all the tests in sequential passing order
            while (passedPreviousTest && testIndex < allTests.size())
            {
                passedPreviousTest = runSpecificTest(allTests[testIndex++]);
            }

            if (passedPreviousTest)
            {
                cout << "Congrats! You have passed all of the required tests (" << allTests.size() << "). Please don't forget to double check your code quality and implementation :)." << endl;
            }
            else
            {
                cout << "You have failed the last test that ran " << "(" << testIndex << "/" << allTests.size() << "). Therefore, additional tests will not be run.\n"
                    << "Please fix your implementation so that the failing test passes, and run again once you feel you've fixed that functionality." << endl;
            }
        }
    }
    else
    {
        cout << "Constructor test failed, additional tests will not be run." << endl;
    }

    return 0;
}

bool runSpecificTest(string testName)
{
    if (testName == "addOneCustomer")
    {
        //Call test
        cout << "ADD ONE ITEM (Needs front) TEST RUNNING..." << endl;
        return testAddOneCustomerData();
    }
    else if (testName == "removeOneCustomer")
    {
        //Call test
        cout << "REMOVE ONE ITEM TEST RUNNING..." << endl;
        return testRemoveOneCustomerData();
    }
    else if (testName == "addAndRemoveManyCustomers")
    {
        //Call test
        cout << "ADD & REMOVE MULTIPLE ITEMS (Needs getSize) TEST RUNNING..." << endl;
        return testAddAndRemoveManyCustomerData();
    }
    else if (testName == "isEmpty")
    {
        //Call test
        cout << "ISEMPTY TEST RUNNING..." << endl;
        return testIsEmpty();
    }
    else if (testName == "copyConstructor")
    {
        //Call test
        cout << "COPY CONSTRUCTOR TEST RUNNING..." << endl;
        return testCopyConstructor();
    }
    else if (testName == "copyConstructorIsDeepCopy")
    {
        //Call test
        cout << "COPY CONSTRUCTOR IS DEEP COPY TEST RUNNING..." << endl;
        return testcopyConstructorIsDeepCopy();
    }
    else if (testName == "addAfterCopy")
    {
        //Call test
        cout << "ADD AFTER COPY CONSTRUCTOR TEST RUNNING..." << endl;
        return testAddAfterCopyConstructor();
    }
    else if (testName == "copyConstructorEmptyQueue")
    {
        //Call test
        cout << "COPY CONSTRUCTOR EMPTY QUEUE TEST RUNNING..." << endl;
        return testCopyConstructorEmptyQueue();
    }
    else if (testName == "assignmentOperator")
    {
        //Call test
        cout << "ASSIGNMENT OPERATOR TEST RUNNING..." << endl;
        return testAssignmentOperator();
    }
    else if (testName == "assignmentOperatorIsDeepCopy")
    {
        //Call test
        cout << "ASSIGNMENT OPERATOR IS DEEP COPY TEST RUNNING..." << endl;
        return testassignmentOperatorIsDeepCopy();
    }
    else if (testName == "assignmentOperatorSelfCopy")
    {
        //Call test
        cout << "ASSIGNMENT OPERATOR SELF COPY TEST RUNNING..." << endl;
        return testAssignmentOperatorSelfCopy();
    }
    else if (testName == "addAfterAssignment")
    {
        //Call test
        cout << "ADD AFTER ASSIGNMENT OPERATOR TEST RUNNING..." << endl;
        return testAddAfterAssignmentOperator();
    }
    else if (testName == "assignmentOperatorEmptyQueue")
    {
        //Call test
        cout << "ASSIGNMENT OPERATOR EMPTY QUEUE TEST RUNNING..." << endl;
        return testAssignmentOperatorEmptyQueue();
    }
    else
    {
        cerr << "===TEST NAME NOT FOUND, HELP===" << endl;
        exit(1);
    }
}

bool validateAndPrintContents(Queue& queue, int numItems, CustomerData* items = nullptr, bool validateList = true)
{
    //Instantiate vars to store test result
    bool passedTests = true;
    string testResult = "";

    int queueNumItems = queue.getSize();

    try
    {
        //Check for the number of items
        if (queueNumItems != numItems)
        {
            //Print failure
            testResult += "\n\tnumItems " + std::to_string(queueNumItems) + "; SHOULD BE " + std::to_string(numItems) + "\n";

            //Failed count check
            passedTests = false;
        }
        else
        {
            //Check each item in the list
            if (numItems > 0 && validateList)
            {
                //Guard again nullptr in the items array
                if (items == nullptr)
                {
                    //Test failure - exit immeadiately
                    std::cerr << "TEST FAILURE: NULLPTR" << std::endl;
                    exit(1);
                }

                for (int i = 0; i < numItems; i++)
                {
                    CustomerData top = queue.front();
                    CustomerData& curItem = items[i];
                    //Check each item
                    if (
                        top.customerNumber != curItem.customerNumber ||
                        top.arrivalTime != curItem.arrivalTime ||
                        top.serviceTime != curItem.serviceTime
                        )
                    {
                        //Print a failed result
                        testResult += "\n\tqueue[" + std::to_string(i) + "] = CustomerData(" + std::to_string(top.customerNumber) + ", " + std::to_string(top.arrivalTime) + ", " + std::to_string(top.serviceTime) + ")" +
                            "; SHOULD BE CustomerData(" + std::to_string(curItem.customerNumber) + ", " + std::to_string(curItem.arrivalTime) + ", " + std::to_string(curItem.serviceTime) + ")\n";

                        //Some validation failed
                        passedTests = false;
                    }

                    queue.dequeue();
                }
            }
        }
    }
    catch (std::exception& e)
    {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
        passedTests = false;
    }

    if (passedTests)
    {
        std::cout << "PASSED" << std::endl;
    }
    else
    {
        std::cout << "FAILED" << testResult << std::flush;
    }

    return passedTests;
}

//Pass in nullptr, receive array of values copied to doublelist
Queue& getQueue(CustomerData arr[], int numItems)
{

    Queue* queue = new Queue();

    for (int i = 0; i < numItems; i++)
    {
        (*queue).enqueue(arr[i]);
    }

    return *queue;
}


bool testEmptyConstructor()
{
    Queue queue;

    return validateAndPrintContents(queue, 0, nullptr, false);
}


bool testAddOneCustomerData()
{
    Queue queue;
    CustomerData items[1] = { CustomerData(1, 2, 3) };

    //Add int
    queue.enqueue(items[0]);

    CustomerData frontOfQueue = queue.front();

    if
        (
            frontOfQueue.customerNumber == items[0].customerNumber &&
            frontOfQueue.arrivalTime == items[0].arrivalTime &&
            frontOfQueue.serviceTime == items[0].serviceTime
            )
    {
        cout << "PASSED" << endl;
        return true;
    }
    else
    {
        cout << "FAILED\n\tqueue[front] = CustomerData(" << frontOfQueue.customerNumber << ", " << frontOfQueue.arrivalTime << ", " << frontOfQueue.serviceTime << ")" <<
            "; SHOULD BE CustomerData(" << items[0].customerNumber << ", " << items[0].arrivalTime << ", " << items[0].serviceTime << ")\n";
        return false;
    }
}

bool testRemoveOneCustomerData()
{
    Queue queue;
    CustomerData items[1] = { CustomerData(1, 2, 3) };

    //Add int
    queue.enqueue(items[0]);

    CustomerData dequeueItem = queue.dequeue();

    if
        (
            dequeueItem.customerNumber == items[0].customerNumber &&
            dequeueItem.arrivalTime == items[0].arrivalTime &&
            dequeueItem.serviceTime == items[0].serviceTime
            )
    {
        cout << "PASSED" << endl;
        return true;
    }
    else
    {
        cout << "FAILED\n\tqueue removed CustomerData(" << dequeueItem.customerNumber << ", " << dequeueItem.arrivalTime << ", " << dequeueItem.serviceTime << ")" <<
            "; SHOULD BE CustomerData(" << items[0].customerNumber << ", " << items[0].arrivalTime << ", " << items[0].serviceTime << ")\n";
        return false;
    }
}

bool testAddAndRemoveManyCustomerData()
{
    CustomerData items[5] = {
        CustomerData(1, 2, 3),
        CustomerData(4, 5, 6),
        CustomerData(7, 8, 9),
        CustomerData(10, 11, 12),
        CustomerData(13, 14, 15)
    };
    int numItems = 5;

    //Add ints
    Queue& queue = getQueue(items, numItems);

    //Verify test results
    bool result = validateAndPrintContents(queue, numItems, items);
    delete& queue;
    return result;
}

bool testIsEmpty()
{
    Queue queue;
    bool passedTest = false;
    CustomerData items[1] = { CustomerData(1, 2, 3) };

    bool isEmptyNoItems = queue.isEmpty();

    if (!isEmptyNoItems)
    {
        cout << "FAILED\n\tempty queue claims to not be empty" << endl;
    }
    else
    {
        //Add int
        queue.enqueue(items[0]);

        bool isEmptyHasItems = queue.isEmpty();

        if (isEmptyHasItems)
        {
            cout << "FAILED\n\tnot empty queue claims to be empty" << endl;
        }
        else
        {
            cout << "PASSED" << endl;
            passedTest = true;
        }
    }

    return passedTest;
}

bool testCopyConstructor()
{
    CustomerData items[5] = {
        CustomerData(1, 2, 3),
        CustomerData(4, 5, 6),
        CustomerData(7, 8, 9),
        CustomerData(10, 11, 12),
        CustomerData(13, 14, 15)
    };
    int numItems = 5;

    //Add ints
    Queue& queue = getQueue(items, numItems);
    Queue queue2(queue);

    //Verify test results
    bool result = validateAndPrintContents(queue2, numItems, items);
    delete& queue;
    return result;
}

bool testcopyConstructorIsDeepCopy()
{
    CustomerData items[6] = {
        CustomerData(1, 2, 3),
        CustomerData(4, 5, 6),
        CustomerData(7, 8, 9),
        CustomerData(10, 11, 12),
        CustomerData(13, 14, 15),
        CustomerData(16, 17, 18)
    };
    int numItems = 6;

    //Add ints
    Queue& queue = getQueue(items, numItems - 1);
    Queue queue2(queue);

    //Changing the queues so that they are reasonably different
    queue.enqueue(CustomerData(-1, -1, -1));
    queue2.enqueue(CustomerData(16, 17, 18));

    //Verify test results
    bool result = validateAndPrintContents(queue2, numItems, items);
    delete& queue;
    return result;
}

bool testAddAfterCopyConstructor()
{
    CustomerData items[8] = {
        CustomerData(1, 2, 3),
        CustomerData(4, 5, 6),
        CustomerData(7, 8, 9),
        CustomerData(10, 11, 12),
        CustomerData(13, 14, 15),
        CustomerData(16, 17, 18),
        CustomerData(19, 20, 21),
        CustomerData(22, 23, 24)
    };
    int numItems = 8;

    //Add ints
    Queue& queue = getQueue(items, numItems - 3);
    Queue queue2(queue);

    //Changing the queues so that they are reasonably different
    queue.enqueue(CustomerData(-1, -1, -1));
    queue2.enqueue(CustomerData(16, 17, 18));
    queue2.enqueue(CustomerData(19, 20, 21));
    queue2.enqueue(CustomerData(22, 23, 24));

    //Verify test results
    bool result = validateAndPrintContents(queue2, numItems, items);
    delete& queue;
    return result;
}

bool testCopyConstructorEmptyQueue()
{
    Queue queue;
    Queue queue2(queue);
    cout << "PASSED" << "\n";
    return true;
}

bool testAssignmentOperator()
{
    CustomerData items[6] = {
        CustomerData(1, 2, 3),
        CustomerData(4, 5, 6),
        CustomerData(7, 8, 9),
        CustomerData(10, 11, 12),
        CustomerData(13, 14, 15),
        CustomerData(16, 17, 18)
    };
    int numItems = 6;

    //Add ints
    Queue& queue = getQueue(items, numItems);
    Queue queue2;

    //Call assignment operator
    queue2 = queue;

    //Verify test results
    bool result = validateAndPrintContents(queue2, numItems, items);
    delete& queue;
    return result;
}

bool testassignmentOperatorIsDeepCopy()
{
    CustomerData items[6] = {
        CustomerData(1, 2, 3),
        CustomerData(4, 5, 6),
        CustomerData(7, 8, 9),
        CustomerData(10, 11, 12),
        CustomerData(13, 14, 15),
        CustomerData(16, 17, 18)
    };
    CustomerData dummyArr[4] = {
        CustomerData(1, 1, 1),
        CustomerData(2, 2, 2),
        CustomerData(3, 3, 3),
        CustomerData(4, 4, 4)
    };
    int numItems = 6;

    //Add ints
    Queue& queue = getQueue(items, numItems - 1);
    Queue& queue2 = getQueue(dummyArr, 4);

    //Call assignment operator
    queue2 = queue;

    //Changing the queues so that they are reasonably different
    queue.enqueue(CustomerData(-1, -1, -1));
    queue2.enqueue(CustomerData(16, 17, 18));

    //Verify test results
    bool result = validateAndPrintContents(queue2, numItems, items);
    delete& queue;
    delete& queue2;
    return result;
}

bool testAssignmentOperatorSelfCopy()
{
    CustomerData items[6] = {
        CustomerData(1, 2, 3),
        CustomerData(4, 5, 6),
        CustomerData(7, 8, 9),
        CustomerData(10, 11, 12),
        CustomerData(13, 14, 15),
        CustomerData(16, 17, 18)
    };
    int numItems = 6;

    //Add ints
    Queue& queue = getQueue(items, numItems);

    //Call assignment operator
    queue = queue;

    //Verify test results
    bool result = validateAndPrintContents(queue, numItems, items);
    delete& queue;
    return result;
}

bool testAddAfterAssignmentOperator()
{
    CustomerData items[8] = {
        CustomerData(1, 2, 3),
        CustomerData(4, 5, 6),
        CustomerData(7, 8, 9),
        CustomerData(10, 11, 12),
        CustomerData(13, 14, 15),
        CustomerData(16, 17, 18),
        CustomerData(19, 20, 21),
        CustomerData(22, 23, 24)
    };
    CustomerData dummyArr[4] = {
        CustomerData(1, 1, 1),
        CustomerData(2, 2, 2),
        CustomerData(3, 3, 3),
        CustomerData(4, 4, 4)
    };
    int numItems = 8;

    //Add ints
    Queue& queue = getQueue(items, numItems - 3);
    Queue& queue2 = getQueue(dummyArr, 4);

    //Call assignment operator
    queue2 = queue;

    //Changing the queues so that they are reasonably different
    queue.enqueue(CustomerData(-1, -1, -1));
    queue2.enqueue(CustomerData(16, 17, 18));
    queue2.enqueue(CustomerData(19, 20, 21));
    queue2.enqueue(CustomerData(22, 23, 24));

    //Verify test results
    bool result = validateAndPrintContents(queue2, numItems, items);
    delete& queue;
    delete& queue2;

    return result;
}

bool testAssignmentOperatorEmptyQueue()
{
    Queue queue;
    Queue queue2;
    queue2.enqueue(CustomerData(1, 2, 3));
    queue = queue2;
    cout << "PASSED" << "\n";
    return true;
}

