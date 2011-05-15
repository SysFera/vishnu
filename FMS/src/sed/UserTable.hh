#ifndef USERTABLE_HH
#define USERTABLE_HH

#include <string>
#include <map>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>

class User {
private:
  std::string globalID;
  std::string localID;
  std::string home;
  std::string key;
  uid_t uid;
public:
  User();
  User(const std::string& globalID, const std::string& localID,
       const std::string& home, const std::string& key);
  User(const User& user);
  
  User& operator=(const User& user);
  bool operator<(const User& user);
  bool operator==(const User& user);
  
  const std::string& getLocalID() const;
  const std::string& getHome() const;
  const std::string& getKey() const;
  const uid_t& getUid() const;
};

class UserTable {
private:
  std::map<std::string, User> table;
public:
  UserTable();
  UserTable(const std::string& userFile);
  UserTable(const UserTable& table);
  
  const std::string& getLocalID(const std::string& globalID) const;
  const std::string& getHome(const std::string& globalID) const;
  const std::string& getKey(const std::string& globalID) const;
  const uid_t& getUid(const std::string& globalID) const;
  
  UserTable& operator=(const UserTable& table);
};

#endif
