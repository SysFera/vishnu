#ifndef GRPTABLE_HH
#define GRPTABLE_HH

#include <string>
#include <map>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>

/* Simple Group class. Associates a local ID to a global one. */
/* Also store the group id (gid) for comparisons.             */
class Group {
private:
  std::string globalID;
  std::string localID;
  gid_t gid;
public:
  Group();
  Group(const std::string& globalID, const std::string& localID);
  Group(const Group& group);
  
  Group& operator=(const Group& group);
  bool operator<(const Group& group);
  bool operator==(const Group& group);
  
  const std::string& getLocalID() const;
  const uid_t& getGid() const;
};

/* Groups table. To store and manage the local groups and map
 * them to global groups ID.
 */
class GroupTable {
private:
  std::map<std::string, Group> table;
public:
  GroupTable();
  GroupTable(const std::string& groupFile);
  GroupTable(const GroupTable& table);
  
  const std::string& getLocalID(const std::string& globalID) const;
  const uid_t& getGid(const std::string& globalID) const;
  
  GroupTable& operator=(const GroupTable& table);
};

#endif
