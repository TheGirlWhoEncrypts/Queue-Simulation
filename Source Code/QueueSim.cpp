// g++ -std=c++11 QueueSim.cpp Customer.cpp Queue.cpp Server.cpp -o QueueSim.exe
#include "Queue.h"
#include "Customer.h"
#include "Server.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

/* ------------------------------------ Global Variable ------------------------------------ */
// For void startSimulation()
vector<Customer> customers;

int noOfPendingCustomersIn_sServers = 0;
int totalNoOfCustomers = 0;

int noOfCustomerArrive = 0, noOfCustomerExited = 0, simulationTime = 0;

int minutesIn_pQueue = 0, max_pQueueSize = 0;
int minutesIn_sQueue = 0, max_sQueueSize = 0;

vector<int> pQueueSizes;
vector<int> sQueueSizes;

int noOfCustomerIn_pQueue = 0;
int noOfCustomerIn_pServers = 0;

int noOfCustomerIn_sQueue = 0;
int noOfCustomerIn_sServers = 0;

int noOfAvailable_pServers = 0;
int noOfAvailable_sServers = 0;

/* ------------------------------------ Struct(s) ------------------------------------ */
struct ServerInfo
{
    int noOf_pServer, noOf_sServer;
} serverInfo;

/* ------------------------------------ Function Prototype ------------------------------------ */
void processFile(const string fileName, int &noOfLines, vector<Customer> &customers);
vector<string> tokenizeString(string str, string delimiter);
// To check vector<Customer> &customers;
void printCustomer(const vector<Customer> &customers); // TBD

void allocateServer(vector<Server> &servers, const char serverType, const int size);
void printServer(const vector<Server> &servers); // TBD

// Simulation
void startSimulation(vector<Server> &pServers, vector<Server> &sServers, Queue &pQueue, Queue &sQueue);

// For Existing Custoner at Server
void serveExistingCustomerAtServer(vector<Server> &servers, int &noOfCustomerInServers, int &noOfAvailableServers, const bool is_p);
void serveCompletedCustomerAtServer(Customer &customer, vector<Server> &servers, int &noOfCustomerInServers, int &noOfAvailableServers, const bool is_p);
void removeCompletedCustomerFromServer(Customer &customer, Server &server, int &noOfCustomerInServers, int &noOfAvailableServers, const bool is_p);

// Existing Customer In Queue
void serveExistingCustomerInQueue(vector<Server> &servers, Queue &queue, int &noOfCustomerInServers, int &noOfCustomerInQueue, int &noOfAvailableServers, const bool is_p);
void updateQueue(Customer &customer, Queue &queue, int &noOfCustomerInQueue, const bool is_p, const bool isCustomerInQueue);
void removeCustomerFromQueue(Queue &queue, int &noOfCustomerInQueue);

// For Customers that has completed in pServer & await to proceed to sServer + NOT IN sQueue
void servePendingCustomerIn_sServer(vector<Server> &servers, Queue &queue, int &noOfCustomerInQueue, int &noOfCustomerInServers, int &noOfAvailableServers);

// Remaining Customer (Has not enter system yet)
void serveRemainingCustomer(vector<Server> &servers, Queue &queue, int &noOfCustomerInQueue, int &noOfCustomerInServers, int &noOfAvailableServers);
// // Arrival Customer
void serveArrivalCustomer(Customer &customer, vector<Server> &servers, Queue &queue, int &noOfCustomerInQueue, int &noOfCustomerInServers, int &noOfAvailableServers);
void findAvailableServer(Customer &customer, vector<Server> &servers, Queue &queue, int &noOfCustomerInQueue, int &noOfCustomerInServers, int &noOfAvailableServers, const bool is_p, const bool isCustomerInQueue);
void addCustomerToAvailableServer(Customer &customer, Server &availableServer, Queue &queue, int &noOfCustomerInQueue, int &noOfCustomerInServers, int &noOfAvailableServers, const bool is_p, const bool isCustomerInQueue);
void addCustomerToQueue(Customer &customer, Queue &queue, int &noOfCustomerInQueue, const bool is_p);

// Others
bool isAllCustomerCompleted();
void calculateQueueDetails(int &noOfCustomerInQueue, int &minutesInQueue, int &maxQueueSize, vector<int> &queueSizes);

// Print Report
void printReport(vector<Server> &pServers, vector<Server> &sServers);
double getAverageServiceTime();
double getAverageQueueLength(vector<int> queue);
void printServer(vector<Server> &servers);
void printInformation(string dataDetail, int data);
/* ---------------------------------------- int main() ---------------------------------------- */
int main(int argc, char **argv)
{
    string fileName = argv[1];

    int noOfLines = 0;
    processFile(fileName, noOfLines, customers);

    totalNoOfCustomers = noOfLines - 1;

    vector<Server> pServers, sServers;
    allocateServer(pServers, 'P', serverInfo.noOf_pServer);
    allocateServer(sServers, 'S', serverInfo.noOf_sServer);

    Queue pQueue('P');
    Queue sQueue('Q');

    startSimulation(pServers, sServers, pQueue, sQueue);

    printReport(pServers, sServers);
}

/* ---------------------------------------- Function Implementations ---------------------------------------- */
void processFile(const string fileName, int &noOfLines, vector<Customer> &customers)
{
    fstream inputFile(fileName.c_str(), fstream::in);
    if (!inputFile)
    {
        cerr << "Error opening : " << fileName << "\n"
             << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else
    {
        string readFileLine, endOfFile = "0 0 0";

        while (getline(inputFile, readFileLine) && readFileLine.find(endOfFile) == string::npos)
        {
            vector<string> data = tokenizeString(readFileLine, " ");
            if (noOfLines == 0)
            {
                serverInfo.noOf_pServer = stoi(data[0]);
                serverInfo.noOf_sServer = stoi(data[1]);
            }
            else
            {
                Customer customer(stoi(data[0]), stoi(data[1]), stoi(data[2]));
                customers.push_back(customer);
            }
            noOfLines++;
        }
    }
    inputFile.close();
}

vector<string> tokenizeString(string str, string delimiter)
{
    size_t pos = 0;
    string token;
    vector<string> result;

    while ((pos = str.find(delimiter)) != string::npos)
    {
        token = str.substr(0, pos);
        result.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    result.push_back(str);
    return result;
}

// To check vector<Customer> &customers
// TBD
void printCustomer(const vector<Customer> &customers)
{
    for (int i = 0; i < customers.size(); i++)
    {
        customers.at(i).printCustomerInfo();
    }
}

void allocateServer(vector<Server> &servers, const char serverType, const int size)
{
    for (int i = 0; i < size; i++)
    {
        Server server(serverType, (i + 1));
        servers.push_back(server);
    }
}

// To check vector<Server> &sever)
// TBD
void printServer(const vector<Server> &servers)
{
    for (int i = 0; i < servers.size(); i++)
    {
        servers.at(i).printServerInformation();
    }
}

/* --------------------------------------- Simulation --------------------------------------- */
void startSimulation(vector<Server> &pServers, vector<Server> &sServers, Queue &pQueue, Queue &sQueue)
{
    noOfAvailable_pServers = pServers.size();
    noOfAvailable_sServers = sServers.size();
    while (!isAllCustomerCompleted())
    {
        // For Existing Customer in pServer
        serveExistingCustomerAtServer(pServers, noOfCustomerIn_pServers, noOfAvailable_pServers, true);

        // For Existing Customer in pQueue
        serveExistingCustomerInQueue(pServers, pQueue, noOfCustomerIn_pServers, noOfCustomerIn_pQueue, noOfAvailable_pServers, true);

        // For Existing Customer in sServer
        serveExistingCustomerAtServer(sServers, noOfCustomerIn_sServers, noOfAvailable_sServers, false);

        // For Existing Customer in sQueue
        serveExistingCustomerInQueue(sServers, sQueue, noOfCustomerIn_sServers, noOfCustomerIn_sQueue, noOfAvailable_sServers, false);

        // For Customers that has completed in pServer & await to proceed to sServer + NOT IN sQueue
        servePendingCustomerIn_sServer(sServers, sQueue, noOfCustomerIn_sQueue, noOfCustomerIn_sServers, noOfAvailable_sServers);

        // For Remaining Customer
        serveRemainingCustomer(pServers, pQueue, noOfCustomerIn_pQueue, noOfCustomerIn_pServers, noOfAvailable_pServers);

        // Update
        calculateQueueDetails(noOfCustomerIn_pQueue, minutesIn_pQueue, max_pQueueSize, pQueueSizes);
        calculateQueueDetails(noOfCustomerIn_sQueue, minutesIn_sQueue, max_sQueueSize, sQueueSizes);

        simulationTime += 1;
    }
}
/* ---------------------------------- Existing Customer At Server ---------------------------------- */
void serveExistingCustomerAtServer(vector<Server> &servers, int &noOfCustomerInServers, int &noOfAvailableServers, const bool is_p)
{
    int c = 0;
    while (noOfCustomerInServers > 0 && c < customers.size())
    {
        Customer &customer = customers.at(c);

        bool isInServer = (is_p) ? (customer.get_pServerID().compare("") != 0 && customer.getDepartTime_pServer() == 0) : (customer.get_sServerID().compare("") != 0 && customer.getDepartTime_sServer() == 0);

        if (isInServer && !is_p)
        {
            if (customer.getIsCompletedAt_sServer(simulationTime))
            {
                serveCompletedCustomerAtServer(customer, servers, noOfCustomerInServers, noOfAvailableServers, is_p);
            }
        }
        if (isInServer && is_p)
        {
            if (customer.getIsCompletedAt_pServer(simulationTime))
            {
                serveCompletedCustomerAtServer(customer, servers, noOfCustomerInServers, noOfAvailableServers, is_p);
            }
        }

        c++;
    }
}

void serveCompletedCustomerAtServer(Customer &customer, vector<Server> &servers, int &noOfCustomerInServers, int &noOfAvailableServers, const bool is_p)
{
    const string serverID = (is_p) ? customer.get_pServerID() : customer.get_sServerID();

    bool isFoundServer = false;
    int d = 0;

    while (!isFoundServer && d < servers.size())
    {
        if (servers.at(d).getServerID().compare(serverID) == 0)
        {
            isFoundServer = true;
            Server &server = servers.at(d);

            removeCompletedCustomerFromServer(customer, server, noOfCustomerInServers, noOfAvailableServers, is_p);
        }
        else
        {
            d++;
        }
    }
}

void removeCompletedCustomerFromServer(Customer &customer, Server &server, int &noOfCustomerInServers, int &noOfAvailableServers, const bool is_p)
{
    // Server
    server.setCurrentCustomerID(-1);
    server.setIsAvailable(true);

    // Customer
    (is_p) ? customer.setDepartTime_pServer(simulationTime) : customer.setDepartTime_sServer(simulationTime);

    // Main
    --noOfCustomerInServers;
    ++noOfAvailableServers;

    (is_p) ? ++noOfPendingCustomersIn_sServers : ++noOfCustomerExited;
}

/* ------------------------------------------ Existing Customer In Queue ------------------------------------------ */
void serveExistingCustomerInQueue(vector<Server> &servers, Queue &queue, int &noOfCustomerInServers, int &noOfCustomerInQueue, int &noOfAvailableServers, const bool is_p)
{
    int e = 0;
    while (noOfCustomerInQueue > 0 && e < customers.size())
    {
        Customer &customer = customers.at(e);
        bool isInQueue = (is_p) ? (customer.get_pServerID().compare("") == 0 && customer.get_pQueueWaitTime() > 0) : (customer.get_sServerID().compare("") == 0 && customer.get_sQueueWaitTime() > 0);

        if (isInQueue)
        {
            findAvailableServer(customer, servers, queue, noOfCustomerInQueue, noOfCustomerInServers, noOfAvailableServers, is_p, true);
        }
        e++;
    }
}

void updateQueue(Customer &customer, Queue &queue, int &noOfCustomerInQueue, const bool is_p, const bool isCustomerInQueue)
{

    if (isCustomerInQueue && is_p)
    {
        customer.set_pQueueWaitTime(1);
    }
    else if (isCustomerInQueue && !is_p)
    {
        customer.set_sQueueWaitTime(1);
    }
    else
    {
        addCustomerToQueue(customer, queue, noOfCustomerInQueue, is_p);
    }
}

void removeCustomerFromQueue(Queue &queue, int &noOfCustomerInQueue)
{
    // Queue
    queue.deQueue();

    // Main
    --noOfCustomerInQueue;
}

/* ------------------------------------------ Remaining Customer (Has not enter Secondary Server yet) ------------------------------------------ */
void servePendingCustomerIn_sServer(vector<Server> &servers, Queue &queue, int &noOfCustomerInQueue, int &noOfCustomerInServers, int &noOfAvailableServers)
{
    int f = 0;
    while (noOfPendingCustomersIn_sServers > 0 && f < customers.size())
    {
        Customer &customer = customers.at(f);

        if (customer.getDepartTime_pServer() != 0 && customer.get_sServerID().compare("") == 0 && customer.getDepartTime_pServer() == simulationTime)
        {
            findAvailableServer(customer, servers, queue, noOfCustomerInQueue, noOfCustomerInServers, noOfAvailableServers, false, false);
        }
        f++;
    }
}

/* ------------------------------------------ Remaining Customer (Has not enter system yet) ------------------------------------------ */
void serveRemainingCustomer(vector<Server> &servers, Queue &queue, int &noOfCustomerInQueue, int &noOfCustomerInServers, int &noOfAvailableServers)
{
    for (int a = noOfCustomerArrive; a < customers.size(); a++)
    {
        if (customers.at(a).getArrivalTime() == simulationTime)
        {
            Customer &customerArrive = customers.at(a);
            serveArrivalCustomer(customerArrive, servers, queue, noOfCustomerInQueue, noOfCustomerInServers, noOfAvailableServers);
            ++noOfCustomerArrive;
        }
    }
}

// Arrival Customer
void serveArrivalCustomer(Customer &customer, vector<Server> &servers, Queue &queue, int &noOfCustomerInQueue, int &noOfCustomerInServers, int &noOfAvailableServers)
{
    findAvailableServer(customer, servers, queue, noOfCustomerInQueue, noOfCustomerInServers, noOfAvailableServers, true, false);
}

void findAvailableServer(Customer &customer, vector<Server> &servers, Queue &queue, int &noOfCustomerInQueue, int &noOfCustomerInServers, int &noOfAvailableServers, const bool is_p, const bool isCustomerInQueue)
{
    bool isFoundServer = false;
    int b = 0;
    while (noOfAvailableServers > 0 && !isFoundServer && b < servers.size())
    {
        if (servers.at(b).getIsAvailable())
        {
            isFoundServer = true;
            Server &available_Server = servers.at(b);

            addCustomerToAvailableServer(customer, available_Server, queue, noOfCustomerInQueue, noOfCustomerInServers, noOfAvailableServers, is_p, isCustomerInQueue);
        }
        else
        {
            b++;
        }
    }

    if (!isFoundServer)
    {
        updateQueue(customer, queue, noOfCustomerInQueue, is_p, isCustomerInQueue);
    }
}

void addCustomerToAvailableServer(Customer &customer, Server &availableServer, Queue &queue, int &noOfCustomerInQueue, int &noOfCustomerInServers, int &noOfAvailableServers, const bool is_p, const bool isCustomerInQueue)
{
    if (is_p)
    {
        // Server
        availableServer.setCurrentCustomerID(customer.getCustomerID());
        availableServer.setTotalServiceTime(customer.getTimeSpent_pServer());
        availableServer.setIsAvailable(false);

        // Customer
        customer.set_pServerID(availableServer.getServerID());
        customer.setTimeEnter_pServer(simulationTime);
    }
    else
    {
        // Server
        availableServer.setCurrentCustomerID(customer.getCustomerID());
        availableServer.setTotalServiceTime(customer.getTimeSpent_sServer());
        availableServer.setIsAvailable(false);

        // Customer
        customer.set_sServerID(availableServer.getServerID());
        customer.setTimeEnter_sServer(simulationTime);

        // Main
        --noOfPendingCustomersIn_sServers;
    }

    // Main
    ++noOfCustomerInServers;
    --noOfAvailableServers;

    if (isCustomerInQueue)
    {
        removeCustomerFromQueue(queue, noOfCustomerInQueue);
    }
}

void addCustomerToQueue(Customer &customer, Queue &queue, int &noOfCustomerInQueue, const bool is_p)
{
    // Queue
    queue.enQueue(customer.getCustomerID());

    // Customer
    (is_p) ? customer.set_pQueueWaitTime(1) : customer.set_sQueueWaitTime(1);

    // Main
    ++noOfCustomerInQueue;
}

/* --------------------------------------------------------- Others --------------------------------------------------------- */
bool isAllCustomerCompleted()
{
    return (noOfCustomerExited == totalNoOfCustomers) ? true : false;
}

void calculateQueueDetails(int &noOfCustomerInQueue, int &minutesInQueue, int &maxQueueSize, vector<int> &queueSizes)
{
    minutesInQueue += noOfCustomerInQueue;
    queueSizes.push_back(noOfCustomerInQueue);
    maxQueueSize = (noOfCustomerInQueue > maxQueueSize) ? noOfCustomerInQueue : maxQueueSize;
}

/* --------------------------------------------------------- Print Report --------------------------------------------------------- */

void printReport(vector<Server> &pServers, vector<Server> &sServers)
{

    printInformation("Number of people served", totalNoOfCustomers);
    printInformation("Time last service request is completed", simulationTime);
    printInformation("Average total service time", getAverageServiceTime());
    cout << endl;
    printInformation("Average total time in PRIMARY queue(s)", (minutesIn_pQueue / (double)simulationTime));
    printInformation("Average total time in SECONDARY queue(s)", (minutesIn_sQueue / (double)simulationTime));
    printInformation("Average total time in OVERALL queue(s)", ((minutesIn_pQueue / (double)simulationTime) + (minutesIn_sQueue / (double)simulationTime)) / 2.0);
    cout << endl;
    printInformation("Average length in PRIMARY queue(s)", getAverageQueueLength(pQueueSizes));
    printInformation("Average length in SECONDARY queue(s)", getAverageQueueLength(sQueueSizes));
    printInformation("Average length in OVERALL queue(s)", (getAverageQueueLength(pQueueSizes) + getAverageQueueLength(sQueueSizes)) / 2.0);
    cout << endl;
    printInformation("Maximum Length in PRIMARY queue(s)", max_pQueueSize);
    printInformation("Maximum Length in SECONDARY queue(s)", max_sQueueSize);
    printInformation("Maximum Length in OVERALL queue(s)", max_pQueueSize + max_sQueueSize);
    cout << endl;
    printServer(pServers);
    printServer(sServers);
}

double getAverageServiceTime()
{
    double totalServiceTime = 0.0;
    for (int i = 0; i < customers.size(); i++)
    {
        totalServiceTime += customers.at(i).getTotalServiceTime();
    }
    return totalServiceTime / (double)customers.size();
}

double getAverageQueueLength(vector<int> queue)
{
    double aveLength = 0.0;
    for (int i = 0; i < queue.size(); i++)
    {
        aveLength += queue.at(i);
    }
    return aveLength / simulationTime;
}

void printServer(vector<Server> &servers)
{
    for (int i = 0; i < servers.size(); i++)
    {
        string str = "Total idle time for " + servers.at(i).getServerID();
        printInformation(str, servers.at(i).getTotalIdleTime(simulationTime));
    }
}

void printInformation(string dataDetail, int data)
{
    cout << left << setw(40) << dataDetail << " : " << data << endl;
}
