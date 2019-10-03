#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer
{
private:
  static int customerCount;
  int customerID;
  int arrivalTime;

  std::string pServerID, sServerID;

  int pQueueWaitTime, timeEnter_pServer, timeSpent_pServer, departTime_pServer;
  bool isCompletedAt_pServer;

  int sQueueWaitTime, timeEnter_sServer, timeSpent_sServer, departTime_sServer;
  bool isCompletedAt_sServer;

  int exitTime, totalTimeSpentAtSystem;

public:
  Customer(int arrivalTime, int timeSpent_pServer, int timeEnter_sServer);

  // Getters
  static int getCustomerCount();
  int getCustomerID() const;

  int getArrivalTime() const;

  std::string get_pServerID() const;
  std::string get_sServerID() const;

  int get_pQueueWaitTime() const;
  int getTimeEnter_pServer() const;
  int getTimeSpent_pServer() const;
  int getDepartTime_pServer() const;
  bool getIsCompletedAt_pServer(const int currentTime);

  int get_sQueueWaitTime() const;
  int getTimeEnter_sServer() const;
  int getTimeSpent_sServer() const;
  int getDepartTime_sServer() const;
  bool getIsCompletedAt_sServer(const int currentTime);

  int getTotalServiceTime() const;

  // Setters;
  void setArrivalTime(int arrivalTime);

  void set_pServerID(std::string pServerID);
  void set_sServerID(std::string sServerID);

  void set_pQueueWaitTime(int time);
  void setTimeEnter_pServer(int timeEnter_pServer);
  void setTimeSpent_pServer(int timeSpent_pServer);
  void setDepartTime_pServer(int departTime_pServer);

  void set_sQueueWaitTime(int time);
  void setTimeEnter_sServer(int timeEnter_sServer);
  void setTimeSpent_sServer(int timeSpent_sServer);
  void setDepartTime_sServer(int departTime_sServer);

  // Methods
  void printCustomerInfo() const;
  void printInformation(std::string dataDetail, int data, bool isAddNewLine = false) const;
};
#endif