#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UserServer.hh"
#include <exception>
#include "SystemException.hh"
#include "UMSVishnuException.hh"
#include "utilServer.hh"

/*
int convertToInt(std::string val) {
  int intValue;
  std::istringstream str(val); 
  str >> intValue;
  return static_cast<int> (intValue);
}
*/

UserServer::UserServer(std::string userId, std::string password) {
  //try {
    muser.setUserId(userId);
    muser.setPassword(password);
    mdatabaseVishnu = factory.getDatabaseInstance();
    /*}
  catch (SystemException& e) {
	throw e;
 }*/
}

UserServer::UserServer(const UMS_Data::User& user):muser(user) {
  
}
 
UserServer::UserServer(SessionServer sessionServer): msessionServer(&sessionServer) {
  //try {
	mdatabaseVishnu = factory.getDatabaseInstance();
  /*}
  catch (SystemException& e) {
	throw e;
  }*/
}
 
  
int 
UserServer::add(UMS_Data::User*& user) {

/*  
ecorecpp::parser::parser parser;
UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);
*/	      
 try {
        if (exist()) {  
	    if (isAdmin()) {
	     
	      //Insertion dans la base User ... Insert Into
	      //User_ptr user = parser.load(std::string(userSerialized))->as< User >();
	      //TODO :  voir stockage password avec Ibrahima
	      std::string pwd = generatePassword(user->getLastname(), user->getFirstname());
	      //std::string pwd = generatePassword("","");
	      user->setPassword(pwd);
	      //TODO voir génération Id avec Kévine
	      user->setUserId(user->getFirstname()+"_"+user->getLastname());
	      
	      mdatabaseVishnu->process("insert into users (vishnu_vishnuid, userid, pwd, firstname, lastname,\
	      privilege, email, passwordstate, deleted, blocked)\
              values (1, '"+user->getUserId()+"','"+user->getPassword()+"','\
              "+ user->getFirstname()+"','"+user->getLastname()+"',"+ 
		convertToString(user->getPrivilege()) +",'"+user->getEmail() +"', 1, 0, 0)");
	      
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
}
 
int 
UserServer::update(UMS_Data::User user) {
	return 0;
}
 
int 
UserServer::deleteUser(UMS_Data::User user)
{
	return 0;
}
 
int 
UserServer::changePassword(std::string newPassword)
{
	return 0;
}
 
int 
UserServer::resetPassword(UMS_Data::User user) {
	return 0;
}
 
UserServer::~UserServer()
{
}
 
UMS_Data::User 
UserServer::getData()
{
	return muser;
}
 
bool 
UserServer::isAdmin() { 
 try {        
      return 
      (
	 convertToInt (
	   getAttribut("where userid='"+muser.getUserId()+"'and \
           pwd='"+muser.getPassword()+"'", " privilege")
	   ) != 0
      );
 } 
 catch (SystemException& e) {
	throw e;
 }	
}


//The init function is used when the user has been created with the sessionKey
//To initialize userId and password
void 
UserServer::init(){
   std::string numUser;
  
  //userId and password have not been defined
  if ((muser.getUserId().size() == 0) && (muser.getUserId().size() == 0)) {
    
    try {
      //To get the user information by using the sessionServer
	    numUser = 
	    msessionServer->getAttribut("where\
	    sessionkey='"+msessionServer->getData().getSessionKey()+"'", "users_numuserid");
	    
	    
	    std::cout << "Num user" << numUser << std::endl;
	    /*If the session key is found*/ 
	    if (numUser.size() != 0) {
	      muser.setUserId(getAttribut("where numuserid='"+numUser+"'", "userid"));
	      muser.setPassword(getAttribut("where numuserid='"+numUser+"'", "pwd"));
	      
	      std::cout << "idUser" << muser.getUserId() << std::endl;
	      std::cout << "Pwd" << muser.getPassword() << std::endl;
	
	    } // END If the session key is found
	    else {
		  UMSVishnuException e (4, "The session key is unrecognized. The user is unknown");
		  throw e;
	    }
      
      }
      catch (SystemException& e) {
	    throw e;
    }
  
  }
}


bool UserServer::exist() {
  
  bool existUser = true;
  
  try {    
	//If the user is on the database
     if (getAttribut("where \
			userid='"+muser.getUserId()+"'and pwd='"+muser.getPassword()+"'").size() != 0)	{	  
       //If the user is not deleted
	if (!isAttributOk("deleted", 1)) { 
	      //If the user is not blocked  
	      if (!isAttributOk("blocked", 1)) {
		
		//If the passwordstate is active  
		if (isAttributOk("passwordstate", 1)) {
			return existUser;
			/*(getAttribut("where \
			userid='"+muser.getUserId()+"'and pwd='"+muser.getPassword()+"'").size() != 0	
			);*/
		} //END If the passwordstate is active
		else {
		      UMSVishnuException e (4, "The user have to change her/his password");
		      throw e;
		}
		    
	    } //END if the user is non blocked
	    else {
		  UMSVishnuException e (4, "The user is blocked");
		  throw e;
	    }
	    
	}//END if the user is not deleted
	else {
	      UMSVishnuException e (4, "The user is deleted");
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
}

bool 
UserServer::isAttributOk(std::string attributName, int valueOk) {
 try {
      return
      
      (
	convertToInt(getAttribut("where userid='"+muser.getUserId()+"'and \
	pwd='"+muser.getPassword()+"'", attributName)) == valueOk	
      );
 } 
 catch (SystemException& e) {
	throw e;
 } 	
}
 
 
 
bool UserServer::checkLogin()
{
  return false;
}
 
bool UserServer::checkPassword() {
  return false;
}

//md5 encrypted of the lastname et firstname
std::string 
UserServer::generatePassword(std::string lastname, std::string firstname) {
  srand(time(NULL));
  std::string salt = "$5$"+lastname + firstname+"$";
  std::string clef = firstname+convertToString(rand());    
  return (std::string(crypt(clef.c_str(), salt.c_str())+salt.length()));
}

/*
int UserServer::getId() {
  
  DatabaseResult* result;
  std::vector<std::string>::iterator ii;
  
  std::string sqlCommand("SELECT numuserid FROM users where userid='");
  sqlCommand.append(muser.getUserId()+ "' and pwd='");
  sqlCommand.append(muser.getPassword()+"'");
  std::cout <<"SQL COMMAND:"<<sqlCommand;
  
  try {
  result = mdatabaseVishnu->getResult(sqlCommand.c_str());
  } 
  catch (SystemException& e) {
  throw e;
  }
	
  if (result->getNbTuples() != 0) {
      result->print();
	     std::vector<std::string> tmp = result->get(0);
	  
	  ii=tmp.begin();
	  std::istringstream str(*ii);
	  int value;
	  str>>value;
	  std::cout << "Value: "<< value << std::endl;
	  return value;
	    //      ii=tmp.begin();
	     //return 0;
	  //   return static_cast<int> (*ii);
	} 
	else {
	  return -1;
	    //res =  false;
	}
}
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
  
  if (result->getNbTuples() != 0) {
      result->print();
      std::vector<std::string> tmp = result->get(0);  
      ii=tmp.begin();
      /*std::istringstream str(*ii);
      str>>value;
	  std::cout << "Value: "<< value << std::endl;
	  return static_cast<int> (value);*/
      return (*ii);
   } 
   else {
	  return "";
	}
}

/* 
UMS_Data::ListUsers  UserServer::list(SessionServer session, string userIdOptions)
{
	return 0;
}*/
