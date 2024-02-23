// Simulation to demonstrate that one queue feeding multiple cash registers is more efficient than have one queue per cash register
// Jaden Winterpacht (don't forget to put your name on the file)
#include "CustomerData.h"
#include "Queue.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
using namespace std;

// Simulation is performed for 720 minutes
const int SIMULATION_DURATION = 720;
// There are three cash registers
const int NUM_REGISTERS = 3;

// Struct for cash registers
struct CashRegister {
    int registerNum;
    int custServed;
    int openTime;
    bool hasCustomer;

    // Constructors
    CashRegister() {}
    CashRegister(int num, int cust, int time, bool hasCust) 
        : registerNum(num), custServed(cust), openTime(time), hasCustomer(hasCust) {}
};

// Function prototypes
void doSimulation(int maxArrival, int maxService);

void handleDepartureSingleLine(CashRegister registersSingle[], int currentTime, ofstream& outFileOne);

void handleDepartureMultLines(CashRegister registersMult[], int currentTime, ofstream& outFileMult);

void serveExistingCustSingleLine(CashRegister registersSingle[], Queue& singleLine, ofstream& outFileOne, int currentTime,
double& totalWaitTimeSingle, int& numCustsServedSingle, int& maxWaitTimeSingle);

void serveExistingCustMultLines(CashRegister registersMult[], Queue lines[], ofstream& outFileMult, int currentTime, 
double& totalWaitTimeMult, int& numCustsServedMult, int& maxWaitTimeMult);

void handleArrivalSingleLine(CashRegister registersSingle[], int& numCustsServedSingle, ofstream& outFileOne, int currentTime, 
CustomerData& newCust, Queue& singleLine, int& maxSingleLineLength);

void handleArrivalMultLines(CashRegister registersMult[], int& numCustsServedMult, ofstream& outFileMult, int currentTime, 
CustomerData& newCust, Queue lines[], int& maxMultLinesLength);

void updateMaxMultLinesLength(Queue lines[], int& maxMultLinesLength);

int generateTime(int maxInterval);

int main(int argc, char** argv)
{
    // code to get the command line arguments
    if (argc < 4)
    {
        cout << "Usage: " << argv[0] << " maxTimeBetweenCustomers maxServiceTime randomSeed" << endl;
        return 1;
    }
    int maxTimeBetweenCustomers = atoi(argv[1]);
    int maxServiceTime = atoi(argv[2]);
    int randomSeed = atoi(argv[3]);
    if (randomSeed > 0)
    {
        srand(randomSeed);
    }
    else
    {
        srand(time(nullptr));
    }
    // if doing extra credit -- need to handle additional OPTIONAL command line argument

    // Perform the restaurant simulation
    doSimulation(maxTimeBetweenCustomers, maxServiceTime);
}

// Performs the simulation
void doSimulation(int maxArrival, int maxService) {
    // Create a Queue object for the single line
    Queue singleLine;
    // Create an array storing the Queue objects for multiple lines
    Queue lines[3];

    // Generate the arrival time of the first customer
    int firstArrival = generateTime(maxArrival);

    int newCustArrival = firstArrival;

    // Current minute counter
    int currentTime = firstArrival;

    // Customer number counter
    int custNum = 0;

    // Keeps track of the total wait times for the single line and the multiple lines
    double totalWaitTimeSingle = 0;
    double totalWaitTimeMult = 0;

    // Keeps a counter of the amount of customers who have begun service
    int numCustsServedSingle = 0;
    int numCustsServedMult = 0;

    // Keeps track of maximum wait time for the single line and the multiple lines
    int maxWaitTimeSingle = 0;
    int maxWaitTimeMult = 0;

    // Keeps track of maximum line length for the single line and the multiple lines
    int maxSingleLineLength = 0;
    int maxMultLinesLength = 0;

    // Create and initialize six CashRegister objects
    CashRegister registersSingle[3];
    for (int i = 0; i < 3; i++) {
        registersSingle[i] = CashRegister(i + 1, 0, firstArrival, false);
    }

    CashRegister registersMult[3];
    for (int i = 0; i < 3; i++) {
        registersMult[i] = CashRegister(i + 1, 0, firstArrival, false);
    }

    // Create file objects to write output to
    ofstream outFileOne("singleline.txt");
    ofstream outFileMult("multiplelines.txt");

    // Simulation begins at the arrival of the first customer and ends at minute 720
    for (int i = firstArrival; i <= SIMULATION_DURATION; i++) {
        handleDepartureSingleLine(registersSingle, currentTime, outFileOne);
        handleDepartureMultLines(registersMult, currentTime, outFileMult);

        serveExistingCustSingleLine(registersSingle, singleLine, outFileOne, currentTime, totalWaitTimeSingle, numCustsServedSingle, 
        maxWaitTimeSingle);
        serveExistingCustMultLines(registersMult, lines, outFileMult, currentTime, totalWaitTimeMult, numCustsServedMult, 
        maxWaitTimeMult);

        // Handle the arrival of a new customer
        if (currentTime == newCustArrival) {
            CustomerData newCust(++custNum, currentTime, generateTime(maxService));
            // Generate the next customer's arrival time
            newCustArrival = currentTime + generateTime(maxArrival);
            outFileOne << "Customer " << newCust.customerNumber << " arrived at " << currentTime << ".\n";
            outFileMult << "Customer " << newCust.customerNumber << " arrived at " << currentTime << ".\n";

            handleArrivalSingleLine(registersSingle, numCustsServedSingle, outFileOne, currentTime, newCust, singleLine, 
            maxSingleLineLength);
            handleArrivalMultLines(registersMult, numCustsServedMult, outFileMult, currentTime, newCust, lines, maxMultLinesLength);
        }
        currentTime++;
    }
    // Calculate average wait time
    outFileOne << "\nAverage wait time: " << fixed << setprecision(1) << (totalWaitTimeSingle / numCustsServedSingle) << " minutes";
    outFileMult << "\nAverage wait time: " << fixed << setprecision(1) << (totalWaitTimeMult / numCustsServedMult) << " minutes";
    // Output maximum wait time
    outFileOne << "\nMaximum wait time: " << maxWaitTimeSingle << " minutes";
    outFileMult << "\nMaximum wait time: " << maxWaitTimeMult << " minutes";
    // Output maximum line length
    outFileOne << "\nMaximum line length: " << maxSingleLineLength << " customers\n";
    outFileMult << "\nMaximum line length: " << maxMultLinesLength << " customers\n";

    // Close the output files
    outFileOne.close();
    outFileMult.close();
}

// Handles the departure of a customer who is done being served
void handleDepartureSingleLine(CashRegister registersSingle[], int currentTime, ofstream& outFileOne) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (currentTime >= registersSingle[i].openTime && registersSingle[i].hasCustomer) {
            outFileOne << "Customer " << registersSingle[i].custServed << " departed at " << currentTime << ".\n";
            registersSingle[i].hasCustomer = false;
        }
    }
}

// Handles departure for multiple lines
void handleDepartureMultLines(CashRegister registersMult[], int currentTime, ofstream& outFileMult) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (currentTime >= registersMult[i].openTime && registersMult[i].hasCustomer) {
            outFileMult << "Customer " << registersMult[i].custServed << " departed at " << currentTime << ".\n";
            registersMult[i].hasCustomer = false;
        }
    }
}

// If there is an open register and there is a line, dequeue a customer and begin service
void serveExistingCustSingleLine(CashRegister registersSingle[], Queue& singleLine, ofstream& outFileOne, int currentTime,
double& totalWaitTimeSingle, int& numCustsServedSingle, int& maxWaitTimeSingle) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (!registersSingle[i].hasCustomer && !singleLine.isEmpty()) {
            CustomerData nextCust = singleLine.dequeue();
            outFileOne << "Customer " << nextCust.customerNumber << " served by " << registersSingle[i].registerNum
                    << " at " << currentTime << ".\n";
            // Increment the number of customers served counter
            numCustsServedSingle++;
            // Calculate wait time for this customer and add it to total wait time
            totalWaitTimeSingle += currentTime - nextCust.arrivalTime;
            // Update maximum wait time if necessary
            if ((currentTime - nextCust.arrivalTime) > maxWaitTimeSingle) {
                maxWaitTimeSingle = currentTime - nextCust.arrivalTime;
            }
            registersSingle[i].hasCustomer = true;
            registersSingle[i].custServed = nextCust.customerNumber;
            registersSingle[i].openTime = currentTime + nextCust.serviceTime;   
        }
    }
}

// If there is an open register and the corresponding line is not empty, dequeue the customer
void serveExistingCustMultLines(CashRegister registersMult[], Queue lines[], ofstream& outFileMult, int currentTime, 
double& totalWaitTimeMult, int& numCustsServedMult, int& maxWaitTimeMult) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (!registersMult[i].hasCustomer && !lines[i].isEmpty()) {
            CustomerData nextCust = lines[i].dequeue();
            outFileMult << "Customer " << nextCust.customerNumber << " served by " << registersMult[i].registerNum
                    << " at " << currentTime << ".\n";
            numCustsServedMult++;
            totalWaitTimeMult += currentTime - nextCust.arrivalTime;
            if ((currentTime - nextCust.arrivalTime) > maxWaitTimeMult) {
                maxWaitTimeMult = currentTime - nextCust.arrivalTime;
            }
            registersMult[i].hasCustomer = true;
            registersMult[i].custServed = nextCust.customerNumber;
            registersMult[i].openTime = currentTime + nextCust.serviceTime;  
        }
    }
}

// Process the arrival of a new customer
void handleArrivalSingleLine(CashRegister registersSingle[], int& numCustsServedSingle, ofstream& outFileOne, int currentTime, 
CustomerData& newCust, Queue& singleLine, int& maxSingleLineLength) {
    // Check for an open register to serve the new customer
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (!registersSingle[i].hasCustomer) {
            outFileOne << "Customer " << newCust.customerNumber << " served by " << registersSingle[i].registerNum
                << " at " << currentTime << ".\n";
            numCustsServedSingle++;
            registersSingle[i].hasCustomer = true;
            registersSingle[i].custServed = newCust.customerNumber;
            registersSingle[i].openTime = currentTime + newCust.serviceTime;
            break;
        // Otherwise, if all registers are occupied, the customer will join the line
        } else if (registersSingle[0].hasCustomer && registersSingle[1].hasCustomer && registersSingle[2].hasCustomer) {
            singleLine.enqueue(newCust);
            if (singleLine.getSize() > maxSingleLineLength) {
                maxSingleLineLength = singleLine.getSize();
            }
            break;
        }
    }
}

// Handles arrival for multiple lines
void handleArrivalMultLines(CashRegister registersMult[], int& numCustsServedMult, ofstream& outFileMult, int currentTime, 
CustomerData& newCust, Queue lines[], int& maxMultLinesLength) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (!registersMult[i].hasCustomer) {
            outFileMult << "Customer " << newCust.customerNumber << " served by " << registersMult[i].registerNum
                << " at " << currentTime << ".\n";
            numCustsServedMult++;
            registersMult[i].hasCustomer = true;
            registersMult[i].custServed = newCust.customerNumber;
            registersMult[i].openTime = currentTime + newCust.serviceTime;
            break;
        } else if (registersMult[0].hasCustomer && registersMult[1].hasCustomer && registersMult[2].hasCustomer) {
            // If all registers are busy, the customer will join the shortest line
            if (lines[0].getSize() <= lines[1].getSize() && lines[0].getSize() <= lines[2].getSize()) {
                lines[0].enqueue(newCust);
                updateMaxMultLinesLength(lines, maxMultLinesLength);
                break;
            } else if (lines[1].getSize() <= lines[0].getSize() && lines[1].getSize() <= lines[2].getSize()) {
                lines[1].enqueue(newCust);
                updateMaxMultLinesLength(lines, maxMultLinesLength);
                break;
            } else {
                lines[2].enqueue(newCust);
                updateMaxMultLinesLength(lines, maxMultLinesLength);
                break;
            }
        }
    }
}

// Updates the maximum length of all lines combined for multiple lines
void updateMaxMultLinesLength(Queue lines[], int& maxMultLinesLength) {
    if ((lines[0].getSize() + lines[1].getSize() + lines[2].getSize()) > maxMultLinesLength) {
        maxMultLinesLength = lines[0].getSize() + lines[1].getSize() + lines[2].getSize();
    }
}

// Generates a random time given a maximum time interval
int generateTime(int maxInterval) {
    return rand() % maxInterval + 1;
}