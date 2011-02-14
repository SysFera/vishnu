/**
* \file UserServer.cpp
* \brief This file implements the Class which manipulates VISHNU user data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "UserServer.hh"

/**
* \brief Constructor
* \fn UserServer(std::string userId, std::string password) 
* \param userId The userId of the user
* \param password The password of the user
*/
UserServer::UserServer(std::string userId, std::string password) {
  DbFactory factory;
  muser.setUserId(userId);
  muser.setPassword(password);
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Constructor
* \fn UserServer(const UMS_Data::User& user) 
* \param user The user data structure
*/
UserServer::UserServer(const UMS_Data::User& user):muser(user) { 
}

/**
* \brief Constructor
* \fn UserServer(SessionServer sessionServer) 
* \param sessionServer The object to manipulate session
*/ 
UserServer::UserServer(SessionServer sessionServer): msessionServer(&sessionServer) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU user
* \fn int add(UMS_Data::User*& user) 
* \param user The user data structure
* \return raises an exception on error
*/
int 
UserServer::add(UMS_Data::User*& user) {
  std::string pwd;
  std::string sqlInsert = "insert into users (vishnu_vishnuid, userid, pwd, firstname, lastname,\
  privilege, email, passwordstate, status) values "; 

  try {
    if (exist()) {  
      if (isAdmin()) {
	//TODO :  voir stockage password avec Ibrahima
	pwd = generatePassword(user->getLastname(), user->getFirstname());
	user->setPassword(pwd);
	
	//TODO voir génération Id avec Kévine
	user->setUserId(user->getFirstname()+"_"+user->getLastname());
	
	//If the user to add exists
	if (getAttribut("where userid='"+user->getUserId()+"'").size() == 0) {    
	  //Insertion of user information on the database
	  mdatabaseVishnu->process(sqlInsert + "(" + Vishnuid::mvishnuid+", \
	  '"+user->getUserId()+"','"+user->getPassword()+"','\
	  "+ user->getFirstname()+"','"+user->getLastname()+"',"+ 
	  convertToString(user->getPrivilege()) +",'"+user->getEmail() +"', \
	  0, "+convertToString(user->getStatus())+")");
	  
	}// END If the user to add exists
	else {
	  UMSVishnuException e (4, "The userId generated already exists");
	  throw e;
	}  
	  //TODO : voir envoi de mail avec kévine
	  
      } //END if the user is an admin 
      else {
	UMSVishnuException e (4, "The user is not an admin");
	throw e;
      }
    } //END if the user exists 
    else {
	  UMSVishnuException e (4, "The user is unknown");
	  throw e;
    }
  }
  catch (SystemException& e) {
	throw e;
 }  
  return 0;
}//END: add(UMS_Data::User*& user)

/**
* \brief Function to update user information
* \fn int update(UMS_Data::User*& user) 
* \param user The user data structure
* \return raises an exception on error
*/
int 
UserServer::update(UMS_Data::User *user) {
  std::string sqlCommand = "";
  try {
    if (exist()) {  
      if (isAdmin()) {
	//if the user whose information will be updated exists
	if (getAttribut("where userid='"+user->getUserId()+"'").size() != 0) {
	  
	  //TODO: concatener les UPDATE et faire un seul accès à la base
	  if (user->getFirstname().size() != 0) {
	    sqlCommand.append("UPDATE users SET firstname='"+user->getFirstname()+"'"+"\
	    where userid='"+user->getUserId()+"';");
	  }
	  
	  if (user->getLastname().size() != 0) {
	    sqlCommand.append("UPDATE users SET lastname='"+user->getLastname()+"'"+"\
	    where userid='"+user->getUserId()+"';");
	  }
	  
	  if (user->getEmail().size() != 0) {
	    sqlCommand.append("UPDATE users SET email='"+user->getEmail()+"'"+"\
	    where userid='"+user->getUserId()+"';");
	  }
	  
	  //If the user will be locked
	  if (user->getStatus() == 0) {
	    //If the user is not already locked
	    if (convertToInt(getAttribut("where userid='"+user->getUserId()+"'", "status")) != 0) {   
	      sqlCommand.append("UPDATE users SET status="+convertToString(user->getStatus())+""+"\
	      where userid='"+user->getUserId()+"';");
	    } //End If the user is not already locked
	    else {
	      UMSVishnuException e (4, "The user is already locked");
	      throw e;
	    }
	  } //End If the user will be locked
	  else {
	    sqlCommand.append("UPDATE users SET status="+convertToString(user->getStatus())+""+"\
	    where userid='"+user->getUserId()+"';");
	  }
	  
	  // if the user whose privilege will be updated is not an admin 
	  if (convertToInt(getAttribut("where userid='"+user->getUserId()+"'", "privilege")) != 1) {   
	    sqlCommand.append("UPDATE users SET privilege="+convertToString(user->getPrivilege())+""+"\
	    where userid='"+user->getUserId()+"';");
	  }
	  /*else { 
	    UMSVishnuException e (4, "It is not possible to change the privilege of an admin");
	    throw e;    
	  }*/
	  
	  std::cout <<"SQL COMMAND:"<<sqlCommand;
	  mdatabaseVishnu->process(sqlCommand.c_str());
	  
	} // End if the user whose information will be updated exists
	else {
	  UMSVishnuException e (4, "The user whose information will be updated does not exist");
	  throw e;
	} 
      } //END if the user is an admin 
      else {
	UMSVishnuException e (4, "The user is not an admin");
	throw e;
      }
    } //END if the user exists 
    else {
      UMSVishnuException e (4, "The user is unknown");
      throw e;
    }
  }
  catch (SystemException& e) {
    throw e;
  }  
  return 0;
} //END: update(UMS_Data::User *user)

/**
* \brief Function to delete VISHNU user
* \fn int deleteUser(UMS_Data::User user)
* \param user The user data structure
* \return raises an exception on error
*/
int 
UserServer::deleteUser(UMS_Data::User user) {
  
  try {
    //If the user to delete is not the super VISHNU admin
    if (user.getUserId().compare("vishnu_db_admin")!=0) {
      //If the user exists
      if (exist()) {  
	if (isAdmin()) {
	  //if the user who will be deleted exist
	  if (getAttribut("where userid='"+user.getUserId()+"'").size() != 0) {
	    //Execution of the sql code to delete the user  
	    mdatabaseVishnu->process("DELETE FROM users where userid='"+user.getUserId()+"'");
	  } // End if the user who will be deleted exist
	  else {
	    UMSVishnuException e (4, "The user who will be deleted does not exist");
	    throw e;
	  } 
	} //END if the user is an admin 
	else {
	  UMSVishnuException e (4, "The user is not an admin");
	  throw e;
	}
      } //END if the user exists 
      else {
	UMSVishnuException e (4, "The user is unknown");
	throw e;
      }
    }//END If the user to delete is not the super VISHNU admin
    else {
      UMSVishnuException e (4, "It is not possible to delete this user. It is the VISHNU root user");
      throw e;
    }
  }
  catch (SystemException& e) {
    throw e;
  }    
  return 0;
}//END: deleteUser(UMS_Data::User user)

/**
* \brief Function to change VISHNU user password
* \fn int changePassword(std::string newPassword)
* \param newPassword The new password of the user
* \return raises an exception on error
*/ 
int 
UserServer::changePassword(std::string newPassword) {
  std::string sqlChangePwd;
  std::string sqlUpdatePwdState;
  
  try {
    //If the user exist (the flagForChangePwd is set to true to avoid the password state checking)
    if (exist(true)) {  
      //sql code to change the user password
      sqlChangePwd = "UPDATE users SET pwd='"+newPassword+"'where \
      userid='"+muser.getUserId()+"' and pwd='"+muser.getPassword()+"';";
        
      //sql code to update the passwordstate
      sqlUpdatePwdState = "UPDATE users SET passwordstate=1 \
      where userid='"+muser.getUserId()+"' and pwd='"+newPassword+"';";
      
      sqlChangePwd.append(sqlUpdatePwdState);
      mdatabaseVishnu->process(sqlChangePwd.c_str());
      
      //Put the new user's password
      muser.setPassword(newPassword);
    } //End If the user exist with the flagForChangePwd to true ti avoid the passwordstate checking
    else {
      UMSVishnuException e (4, "It is not possible to change the password. The userId or/and the password are unknown");
      throw e;
    }
  }
  catch (SystemException& e) {
    throw e;
  }    
  return 0;
}//END: changePassword(std::string newPassword)

/**
* \brief Function to change VISHNU user password
* \fn int resetPassword(UMS_Data::User user)
* \param user The user data structure
* \return raises an exception on error
*/
int 
UserServer::resetPassword(UMS_Data::User user) {
  std::string sqlResetPwd;
  std::string sqlUpdatePwdState;
  
  try {
    //If the user exists
    if (exist()) {  
      //if the user is an admin
      if (isAdmin()) {
	//if the user whose password will be reset exists
	if (getAttribut("where userid='"+user.getUserId()+"'").size() != 0) {
	  //generation of a new password
	  user.setPassword(generatePassword(user.getUserId(), user.getUserId()));
	  //The sql code to reset the password
	  sqlResetPwd = "UPDATE users SET pwd='"+user.getPassword()+"'where \
	  userid='"+user.getUserId()+"';";
	  //sql code to update the passwordstate
	  sqlUpdatePwdState = "UPDATE users SET passwordstate=0 \
	  where userid='"+user.getUserId()+"' and pwd='"+user.getPassword()+"';";
	  //Append of the previous sql code
	  sqlResetPwd.append(sqlUpdatePwdState);
	  //Execution of the sql code on the database
	  mdatabaseVishnu->process( sqlResetPwd.c_str());
	} // End if the user whose password will be reset exists
	else {
	  UMSVishnuException e (4, "The user whose information will be updated do not exist");
	  throw e;
	} 
      } //END if the user is an admin 
      else {
	UMSVishnuException e (4, "The user is not an admin");
	throw e;
      }
    } //END if the user exists 
    else {
      UMSVishnuException e (4, "The user is unknown");
      throw e;
    }
  }
  catch (SystemException& e) {
    throw e;
  }  
  return 0;
}//END: resetPassword(UMS_Data::User user)

/**
* \fn ~UserServer()
* \brief Destructor
*/
UserServer::~UserServer() {
}
/**
* \brief Function to get user information
* \fn UMS_Data::User getData()
* \return  The user data structure
*/ 
UMS_Data::User 
UserServer::getData() {
  return muser;
}
 
/**
* \brief Function to initialize user data for constructor with sessionServer 
* \fn init()
*/
void 
UserServer::init(){
  std::string numUser;
  
  //If userId and password have not been defined
  if ((muser.getUserId().size() == 0) && (muser.getUserId().size() == 0)) {    
    try {
      //To get the users_numuserid by using the sessionServer
      numUser = 
      msessionServer->getAttribut("where\
      sessionkey='"+msessionServer->getData().getSessionKey()+"'", "users_numuserid");
     
      //If the session key is found 
      if (numUser.size() != 0) {
	muser.setUserId(getAttribut("where numuserid='"+numUser+"'", "userid"));
	muser.setPassword(getAttribut("where numuserid='"+numUser+"'", "pwd"));
      } //END If the session key is found
      else {
	UMSVishnuException e (4, "The session key is unrecognized. The user is unknown");
	throw e;
      } 
    }
    catch (SystemException& e) {
      throw e;
    }
  }//END If the userId and password have not been defined
} //END: void init()
/**
* \brief Function to check user on database
* \fn bool exist(bool flagForChangePwd)
* \param flagForChangePwd A flag to check the password state
* \return true if the password state has not to be checked else false
*/
bool UserServer::exist(bool flagForChangePwd) {
  
  bool existUser = true;
  try {    
      //If the user is on the database
    if (getAttribut("where userid='"+muser.getUserId()+"'and pwd='"+muser.getPassword()+"'").size() != 0) {	  
	//If the user is not locked  
	if (!isAttributOk("status", 0)) {
	  //if the flag to check the password state is set
	  if (!flagForChangePwd) {
	    //If the passwordstate is active  
	    if (isAttributOk("passwordstate", 1)) {
	      return existUser;
	    } //END If the passwordstate is active  
	    else {
	      UMSVishnuException e (7, "The user have to change her/his password");
	      throw e;
	    }
	  }//END the flag to check the password state is set
	  else {
	    return existUser;
	  }  
	} //END if the user is not locked
	else {
	  UMSVishnuException e (4, "The user is locked");
	  throw e;
	}    
    }//END if the user is on the database
    else {
      existUser = false;
    }
  }// END try 
  catch (SystemException& e) {
	  throw e;
  } 	
  return existUser;
} //END: exist(bool flagForChangePwd)

/**
* \brief Function to check the VISHNU user privilege
* \fn bool isAdmin()
* \return true if the user is an admin else false
*/
bool 
UserServer::isAdmin() { 
  try {
    return (convertToInt (getAttribut("where userid='"+muser.getUserId()+"'and \
    pwd='"+muser.getPassword()+"'", " privilege")) != 0);
  } 
  catch (SystemException& e) {
  throw e;
  }	
}

/**
* \brief Function to check the user attribut value
* \fn bool isAttributOk(std::string attributName, int valueOk)
* \param attributName The name of the attribut to check
* \param valueOk the value which will be compare to attribut name value
* \return true if the attributName value is valueOk
*/
bool 
UserServer::isAttributOk(std::string attributName, int valueOk) {
  try {
    return (convertToInt(getAttribut("where userid='"+muser.getUserId()+"'and \
    pwd='"+muser.getPassword()+"'", attributName)) == valueOk);
  } 
  catch (SystemException& e) {
    throw e;
  } 	
}

/**
* \brief Function to get user information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname);
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \return the value of the attribut or empty string if no results
*/
std::string UserServer::getAttribut(std::string condition, std::string attrname) {
  
  DatabaseResult* result;
  std::vector<std::string>::iterator ii;

  std::string sqlCommand("SELECT "+attrname+" FROM users "+condition);
  std::cout <<"SQL COMMAND:"<<sqlCommand;
    
  try {
    result = mdatabaseVishnu->getResult(sqlCommand.c_str());
  } 
  catch (SystemException& e) {
    throw e;
  }

  //TODO: A factoriser à mettre adans utils (return util::filter (databaseResult))
  if (result->getNbTuples() != 0) {
    result->print();
    std::vector<std::string> tmp = result->get(0);  
    ii=tmp.begin();
    return (*ii);
  } 
  else {
    return "";
  }
}

/**
* \brief Function to check a userId
* \fn bool existuserId(std::string userId)
* \param userId The userId to check
* \return true if the userId exists
*/
bool 
UserServer::existuserId(std::string userId) {
  bool existUser = true;
  
  try {    
    //If the userID exists on the database
    if (getAttribut("where userid='"+userId+"'").size() != 0)	{	  
      //If the user is not locked  
      if ( convertToInt(getAttribut("where userid='"+userId+"'", "status")) != 1) {
	return existUser;     
      } //END if the user is not locked
      else {
	UMSVishnuException e (4, "The user is locked");
	throw e;
      }    
    }//END If the userID exists on the database
    else {
      existUser = false;
    }
  }// END try 
  catch (SystemException& e) {
    throw e;
  } 	
  return existUser;
}

/**
* \brief Function to generate a password
* \fn generatePassword(std::string value1, std::string value2)
* \param value1 a string used to generate the password
* \param value2 a string used to generate the password
* \return an encrypted message
*/
std::string 
UserServer::generatePassword(std::string value1, std::string value2) {
  
  srand(time(NULL));
  std::string salt = "$5$"+value1 + convertToString(rand())+value2+"$";
  std::string clef = value2+convertToString(rand());
  
  //remove of the string used to encrypted the string which is on the crypt result
  return (std::string(crypt(clef.c_str(), salt.c_str())+salt.length()));
}


/* 
UMS_Data::ListUsers  UserServer::list(SessionServer session, string userIdOptions)
{
	return 0;
}*/
