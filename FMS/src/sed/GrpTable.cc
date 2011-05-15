#include <string>
#include <map>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>

#include "GrpTable.hh"

using namespace std;

/* Default constructor.
 * Needed for the STL containers. */
Group::Group() {
}

/* Standard constructor.
 * To create a group using its local and global IDs. */
Group::Group(const string& globalID, const string& localID) :
globalID(globalID), localID(localID)
{
  struct group* grp = getgrnam(localID.c_str());
  if (grp==NULL)
    throw runtime_error("Unknown group: "+localID);
  gid = grp->gr_gid;
}

/* Copy constructor. */
Group::Group(const Group& group) :
globalID(group.globalID), localID(group.getLocalID()), gid(group.gid)
{
}

/* Copy operator. */
Group& Group::operator=(const Group& group) {
  globalID = group.globalID;
  localID = group.getLocalID();
  gid = group.getGid();
  return *this;
}

/* Comparison operator. */
bool Group::operator<(const Group& group) {
  return (getGid()<group.getGid());
}

/* Equality operator.
 * Use the gids. */
bool Group::operator==(const Group& group) {
  return gid == group.getGid();
}

/* Accessors. */
const string& Group::getLocalID() const {
  return localID;
}

const gid_t& Group::getGid() const {
  return gid;
}

/* Default constructor. */
GroupTable::GroupTable() {
}

/* Standard onstructor.
 * The string parameter is the path to a file describing the
 * groups correspondance.
 *
 * => The file has two columns:
 *    - The first one for global ID.
 *    - The second one for local ID.
 */
GroupTable::GroupTable(const string& groupFile) {
  ifstream file(groupFile.c_str());
  char buffer[1024];
  string line;
  
  if (!file.is_open())
    throw runtime_error("Cannot open "+groupFile);
  
  while (!file.eof()) {
    string globalID, localID;
    
    file.getline(buffer, 1024);
    line = buffer;
    
    istringstream is(line);
    is >> globalID >> localID;
    if (globalID[0]=='#') continue;
    try {
      table[globalID] = Group(globalID, localID);
    } catch (runtime_error& err) {
      cerr << "Warning: " << err.what() << endl;
    }
  }
}

/* Copy operator. */
GroupTable::GroupTable(const GroupTable& table) {
  operator=(table);
}

/* Simplified accessors.
 * Encapsulates the map:
 * Each field of the stored groups are accessed through this class.
 */
const string& GroupTable::getLocalID(const string& globalID) const {
  map<string,Group>::const_iterator it;
  
  it = table.find(globalID);
  if (it==table.end())
    throw runtime_error("Unknown group: "+globalID);
  return it->second.getLocalID();
}

const gid_t& GroupTable::getGid(const std::string& globalID) const {
  map<string,Group>::const_iterator it;
  
  it = table.find(globalID);
  if (it==table.end())
    throw runtime_error("Unknown group: "+globalID);
  return it->second.getGid();
}

GroupTable& GroupTable::operator=(const GroupTable& table) {
  this->table = table.table;
  return *this;
}
