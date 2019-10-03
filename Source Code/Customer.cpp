#include "Customer.h"

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

int Customer::customerCount = 0;

Customer::Customer(int arrivalTime, int timeSpent_pServer, int timeSpent_sServer)
{
    customerID = ++customerCount;

    this->arrivalTime = arrivalTime;

    pServerID = "";
    sServerID = "";

    pQueueWaitTime = 0;
    timeEnter_pServer = 0;
    this->timeSpent_pServer = timeSpent_pServer;
    departTime_pServer = 0;
    isCompletedAt_pServer = false;

    sQueueWaitTime = 0;
    timeEnter_sServer = 0;
    this->timeSpent_sServer = timeSpent_sServer;
    departTime_sServer = 0;
    isCompletedAt_sServer = false;
}

// Getters
int Customer::getCustomerCount() { return customerCount; }
int Customer::getCustomerID() const { return customerID; }

int Customer::getArrivalTime() const { return arrivalTime; }
string Customer::get_pServerID() const { return pServerID; }
string Customer::get_sServerID() const { return sServerID; }

int Customer::get_pQueueWaitTime() const { return pQueueWaitTime; }
int Customer::getTimeEnter_pServer() const { return timeEnter_pServer; }
int Customer::getTimeSpent_pServer() const { return timeSpent_pServer; }
int Customer::getDepartTime_pServer() const { return departTime_pServer; }
bool Customer::getIsCompletedAt_pServer(const int currentTime)
{
    bool isCompleted = (timeSpent_pServer == (currentTime - timeEnter_pServer));
    if (isCompleted)
    {
        departTime_pServer = currentTime;
    }
    return isCompleted;
}

int Customer::get_sQueueWaitTime() const { return sQueueWaitTime; };
int Customer::getTimeEnter_sServer() const { return timeEnter_sServer; };
int Customer::getTimeSpent_sServer() const { return timeSpent_sServer; };
int Customer::getDepartTime_sServer() const { return departTime_sServer; };
bool Customer::getIsCompletedAt_sServer(const int currentTime)
{
    bool isCompleted = (timeSpent_sServer == (currentTime - timeEnter_sServer));
    if (isCompleted)
    {
        departTime_sServer = currentTime;
    }
    return isCompleted;
}

int Customer::getTotalServiceTime() const
{
    return timeSpent_pServer + timeSpent_sServer;
}

// Setters;
void Customer::setArrivalTime(int arrivalTime) { this->arrivalTime = arrivalTime; };

void Customer::set_pServerID(string pServerID) { this->pServerID = pServerID; };
void Customer::set_sServerID(string sServerID) { this->sServerID = sServerID; };

void Customer::set_pQueueWaitTime(int time) { this->pQueueWaitTime += time; };
void Customer::setTimeEnter_pServer(int timeEnter_pServer) { this->timeEnter_pServer = timeEnter_pServer; };
void Customer::setTimeSpent_pServer(int timeSpent_pServer) { this->timeSpent_pServer = timeSpent_pServer; };
void Customer::setDepartTime_pServer(int departTime_pServer) { this->departTime_pServer = departTime_pServer; };

void Customer::set_sQueueWaitTime(int time) { this->sQueueWaitTime += time; };
void Customer::setTimeEnter_sServer(int timeEnter_sServer) { this->timeEnter_sServer = timeEnter_sServer; };
void Customer::setTimeSpent_sServer(int timeSpent_sServer) { this->timeSpent_sServer = timeSpent_sServer; };
void Customer::setDepartTime_sServer(int departTime_sServer) { this->departTime_sServer = departTime_sServer; };

// Methods
void Customer::printCustomerInfo() const
{
    cout << "------------- Customer Information -------------" << endl;
    printInformation("Customer ID", customerID);
    printInformation("Arrival Time", arrivalTime);
    printInformation("Primary Queue Wait Time", pQueueWaitTime, true);
    cout << "\n"
         << left << setw(40) << "Primary Server ID"
         << " : " << pServerID << endl;
    printInformation("Arrival Time @ Primary Server", timeEnter_pServer);
    printInformation("Time Spent At Primary Server", timeSpent_pServer);
    printInformation("Departure Time From Primary Server", departTime_pServer);
    printInformation("Secondary Queue Wait Time", sQueueWaitTime, true);
    cout << "\n"
         << left << setw(40) << "Secondary Server ID"
         << " : " << sServerID << endl;
    printInformation("Arrival Time @ Secondary Server", timeEnter_sServer);
    printInformation("Time Spent At Secondary Server", timeSpent_sServer);
    printInformation("Departure Time From Secondary Server", departTime_sServer);
    cout << "------------------------------------------------\n"
         << endl;
}

void Customer::printInformation(string dataDetail, int data, bool isAddNewLine) const
{
    if (isAddNewLine)
        cout << endl;
    cout << left << setw(40) << dataDetail << " : " << data << endl;
}
