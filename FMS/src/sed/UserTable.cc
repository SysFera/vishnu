#include <string>
#include <map>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

#include "UserTable.hh"

using namespace std;

/* Default constructor.
 * Used by the STL containers.
 */
User::User() {
}

/* Standard constructor.
 * - globalID is the user global identifier.
 * - localID is the user local identifier.
 * - home is the local home directory of the user.
 * - key is the path to the private key of the user.
 */
User::User(const string& globalID, const string& localID,
           const string& home, const string& key) :
globalID(globalID), localID(localID), home(home), key(key)
{
  struct passwd* pwd = getpwnam(localID.c_str());
  if (pwd==NULL)
    throw runtime_error("Unknown user: "+localID);
  uid = pwd->pw_uid;
}

/* Copy constructor. */
User::User(const User& user) :
globalID(user.globalID), localID(user.getLocalID()), home(user.getHome()),
key(user.getKey()), uid(user.uid)
{
}

/* Copy operator. */
User& User::operator=(const User& user) {
  globalID = user.globalID;
  localID = user.getLocalID();
  home = user.getHome();
  key = user.getKey();
  uid = user.getUid();
  return *this;
}

/* Comparison operator. */
bool User::operator<(const User& user) {
  return (getUid()<user.getUid());
}

/* Equality operator. */
bool User::operator==(const User& user) {
  return uid == user.getUid();
}

/* Accessors. */
const string& User::getLocalID() const {
  return localID;
}

const string& User::getHome() const {
  return home;
}

const string& User::getKey() const {
  return key;
}

const uid_t& User::getUid() const {
  return uid;
}

/* Default constructor. */
UserTable::UserTable() {
}

/* Standard constructor.
 * The parameter is a path to a file containing the user table.
 * The file must contain 4 columns:
 *   - The first col. is the users global identifier.
 *   - The second col. is the user local identifier.
 *   - The third col. is the user home directory
 *     (it is not a mandatory to use the same directory as
 *      the one used by the system)
 *   - The last col. is a path to the user ssh private key
 *     used to access the filesystem.
 */
UserTable::UserTable(const string& userFile) {
  ifstream file(userFile.c_str());
  char buffer[1024];
  string line;
  
  if (!file.is_open())
    throw runtime_error("Cannot open "+userFile);
  
  while (!file.eof()) {
    string globalID, localID, home, key;
    
    file.getline(buffer, 1024);
    line = buffer;
    
    istringstream is(line);
    is >> globalID >> localID >> home >> key;
    if (globalID[0]=='#') continue;
    try {
      table[globalID] = User(globalID, localID, home, key);
    } catch (runtime_error& err) {
      cerr << "Warning: " << err.what() << endl;
    }
  }
}

/* Copy constructor. */
UserTable::UserTable(const UserTable& table) {
  operator=(table);
}

/* Accessors. The class encapsulates the map.
 * All the User class fields are accessible through the
 * UserTable class. */
const string& UserTable::getLocalID(const string& globalID) const {
  map<string,User>::const_iterator it;
  
  it = table.find(globalID);
  if (it==table.end())
    throw runtime_error("Unknown user: "+globalID);
  return it->second.getLocalID();
}

const string& UserTable::getHome(const string& globalID) const {
  map<string,User>::const_iterator it;
  
  it = table.find(globalID);
  if (it==table.end())
    throw runtime_error("Unknown user: "+globalID);
  return it->second.getHome();
}

const string& UserTable::getKey(const string& globalID) const {
  map<string,User>::const_iterator it;
  
  it = table.find(globalID);
  if (it==table.end())
    throw runtime_error("Unknown user: "+globalID);
  return it->second.getKey();
}


const uid_t& UserTable::getUid(const std::string& globalID) const {
  map<string,User>::const_iterator it;
  
  it = table.find(globalID);
  if (it==table.end())
    throw runtime_error("Unknown user: "+globalID);
  return it->second.getUid();
}

/* Copy operator. */
UserTable& UserTable::operator=(const UserTable& table) {
  this->table = table.table;
  return *this;
}
