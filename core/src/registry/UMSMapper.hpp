/**
 * \file UMSMapper.hpp
 * \brief Mapper for the UMS module
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#ifndef __UMSMAPPER__HH__
#define __UMSMAPPER__HH__

#include <map>
#include <vector>

#include "Mapper.hpp"
#include "MapperRegistry.hpp"
#include "utilVishnu.hpp"

using namespace std;

const int VISHNU_CONNECT                  = 1 ;
const int VISHNU_RECONNECT 	     	  = 2 ;
const int VISHNU_CLOSE 	     	          = 3 ;
const int VISHNU_ADD_VISHNU_USER   	  = 4 ;
const int VISHNU_UPDATE_VISHNU_USER 	  = 5 ;
const int VISHNU_DELETE_VISHNU_USER 	  = 6 ;
const int VISHNU_CHANGE_PASSWORD   	  = 7 ;
const int VISHNU_RESET_PASSWORD    	  = 8 ;
const int VISHNU_ADD_LOCAL_ACCOUNT        = 9 ;
const int VISHNU_UPDATE_LOCAL_ACCOUNT     = 10;
const int VISHNU_DELETE_LOCAL_ACCOUNT     = 11;
const int VISHNU_SAVE_CONFIGURATION       = 12;
const int VISHNU_RESTORE_CONFIGURATION    = 13;
const int VISHNU_ADD_MACHINE 	          = 14;
const int VISHNU_UPDATE_MACHINE           = 15;
const int VISHNU_DELETE_MACHINE           = 16;
const int VISHNU_LIST_LOCAL_ACCOUNT       = 17;
const int VISHNU_LIST_MACHINE 	          = 18;
const int VISHNU_LIST_HISTORY_CMD         = 19;
const int VISHNU_LIST_OPTIONS 	          = 20;
const int VISHNU_LIST_USERS 	          = 21;
const int VISHNU_LIST_SESSIONS 	          = 22;
const int VISHNU_CONFIGURE_DEFAULT_OPTION = 23;
const int VISHNU_CONFIGURE_OPTION	  = 24;


/**
 * \class UMSMapper
 * \brief Mapper implementation for the UMS module
 */
class UMSMapper : Mapper{
public :
  UMSMapper(MapperRegistry* reg, string name);
  UMSMapper();
  ~UMSMapper();
  /**
   * \brief To register a mapper in the list of active mapper
   * \fn virtual int registerMapper()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  registerMapper();

  /**
   * \brief To unregister a mapper in the list of active mapper
   * \fn virtual int unregisterMapper()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  unregisterMapper();

  /**
   * \brief To get the command corresponding to a key
   * \fn virtual int getCommand()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getCommand(const int& key,string& command);

  /**
   * \brief To get the key corresponding to a command
   * \fn virtual int getKey()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getKey(const string& command, int& key);

  /**
   * \brief Function to add an element to code
   * \fn  virtual int code(const string& cmd, unsigned int code = 0);
   * \param cmd The string to add
   * \param code If 0, the code is created, otherwize it is the key in the map to add the string
   * \return if param code = 0, the assigned code to add other element to the same item, otherwize return 0
   */
  virtual int
  code(const string& cmd, unsigned int code = 0);

  /**
   * \brief To end a command, delete it from the map and get its value
   * \fn virtual string finalize(int key);
   * \param The entry in the map to get an remove
   * \return The coded value of the command
   */
  virtual string
  finalize(int key);

  /**
   * \brief To get, from a coded string, the CPP command that made it
   * \fn virtual string decodeCPP (const string& msg);
   * \param msg The coded string
   * \return The CPP command
   */
  virtual string
  decodeCPP (const string& msg);


protected:
private:


string
decodeClose(vector<int> separator, const string& msg);

string
decodeAddUser(vector<int> separator, const string& msg);

string
decodeUpUser(vector<int> separator, const string& msg);

string
decodeDelUser(vector<int> separator, const string& msg);

string
decodeChangePwd(vector<int> separator, const string& msg);

string
decodeResetPwd(vector<int> separator, const string& msg);

string
decodeAddAcc(vector<int> separator, const string& msg);

string
decodeUpAcc(vector<int> separator, const string& msg);

string
decodeDelAcc(vector<int> separator, const string& msg);

string
decodeSaveConf(vector<int> separator, const string& msg);

string
decodeRestoreConf(vector<int> separator, const string& msg);

string
decodeAddM(vector<int> separator, const string& msg);

string
decodeUpM(vector<int> separator, const string& msg);

string
decodeDelM(vector<int> separator, const string& msg);

string
decodeListAcc(vector<int> separator, const string& msg);

string
decodeListM(vector<int> separator, const string& msg);

string
decodeListH(vector<int> separator, const string& msg);

string
decodeListOp(vector<int> separator, const string& msg);

string
decodeListUser(vector<int> separator, const string& msg);

string
decodeListSession(vector<int> separator, const string& msg);

string
decodeConfDefaultOp(vector<int> separator, const string& msg);

string
decodeConfOp(vector<int> separator, const string& msg);

void
findSeparator(const string& s, vector<int>& vec);

};


#endif
