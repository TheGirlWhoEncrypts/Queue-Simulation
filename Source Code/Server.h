#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server
{
private:
  std::string serverID;
  int currentCustomerID;

  int totalServiceTime, totalIdleTime;
  bool isAvailable;

public:
  Server();
  Server(char serverType, int serverNum);

  // Getters
  std::string getServerID() const;
  int getCurrentCustomerID() const;
  int getTotalServiceTime() const;
  int getTotalIdleTime(int totalSimulationTime);
  bool getIsAvailable() const;

  // Setters
  void setServerID(char serverType, int serverNum);
  void setCurrentCustomerID(int currentCustomerID);
  void setTotalServiceTime(int totalServiceTime);
  void setIsAvailable(bool isAvailable);

  // Method
  void printServerInformation() const;
  void printInformation(std::string dataDetail, std::string data) const;
};

#endif