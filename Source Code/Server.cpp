#include "Server.h"

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;
Server::Server() {}

Server::Server(char serverType, int serverNum)
{
    serverID.push_back(serverType);
    serverID += to_string(serverNum);

    currentCustomerID = -1;
    totalServiceTime = 0;
    totalIdleTime = 0;
    isAvailable = true;
}

// Getters
string Server::getServerID() const { return serverID; }
int Server::getCurrentCustomerID() const { return currentCustomerID; }
int Server::getTotalServiceTime() const { return totalServiceTime; }
int Server::getTotalIdleTime(int totalSimulationTime) { return totalIdleTime = totalSimulationTime - totalServiceTime; }
bool Server::getIsAvailable() const { return isAvailable; }

// Setters
void Server::setServerID(char serverType, int serverNum)
{
    serverID.push_back(serverType);
    serverID += to_string(serverNum);
}
void Server::setCurrentCustomerID(int currentCustomerID) { this->currentCustomerID = currentCustomerID; }
void Server::setTotalServiceTime(int serviceTime) { this->totalServiceTime += serviceTime; }
void Server::setIsAvailable(bool isAvailable) { this->isAvailable = isAvailable; }

// Method
void Server::printServerInformation() const
{
    cout << "------------- Server Information -------------" << endl;
    printInformation("Server ID", serverID);
    printInformation("Current serving CustomerID", to_string(currentCustomerID));
    printInformation("Total Service Time", to_string(totalServiceTime));
    printInformation("Total Idle Time", to_string(totalIdleTime));
    printInformation("Is Server Available", to_string(isAvailable));
    cout << "------------------------------------------------\n"
         << endl;
}

void Server::printInformation(string dataDetail, string data) const
{
    cout << left << setw(35) << dataDetail << " : " << data << endl;
}
