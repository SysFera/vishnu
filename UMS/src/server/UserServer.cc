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
//#include "sed/ServerUMS.hh"

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

std::string pwd;

 try {
        if (exist()) {  
	    if (isAdmin()) {
	     
	      //Insertion dans la base User ... Insert Into
	      //User_ptr user = parser.load(std::string(userSerialized))->as< User >();
	      //TODO :  voir stockage password avec Ibrahima
	      pwd = generatePassword(user->getLastname(), user->getFirstname());
	      //std::string pwd = generatePassword("","");
	      user->setPassword(pwd);
	      //TODO voir génération Id avec Kévine
	      user->setUserId(user->getFirstname()+"_"+user->getLastname());
	      
	      //extern std::string ServerUMS::mvishnuid;
	      
	      //std::cout << "VISHNUIOD dans USer" << ServerUMS::mvishnuid << std::endl;
	      //ServerUMS::mvishnuid;
	      //ServerUMS::mvishnuid;
	      std::cout << "VISHNUIOD dans USer" << Vishnuid::mvishnuid <<std::endl;
	      //TODO Faire appel ici à la variable globale mvishnuid ServerUMS::mvishnuid;
	      //std::cout << "VISHNUIOD dans USer" << ServerUMS::mvishnuid << std::endl;
	      if (getAttribut("where userid='"+user->getUserId()+"'").size() == 0) {
		mdatabaseVishnu->process("insert into users (vishnu_vishnuid, userid, pwd, firstname, lastname,\
		privilege, email, passwordstate, status)\
		values (1, '"+user->getUserId()+"','"+user->getPassword()+"','\
		"+ user->getFirstname()+"','"+user->getLastname()+"',"+ 
		  convertToString(user->getPrivilege()) +",'"+user->getEmail() +"', 0, "+convertToString(user->getStatus())+")");
		}
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
}
 
int 
UserServer::update(UMS_Data::User* user) {
  try {
    if (exist()) {  
      if (isAdmin()) {
	//if the user whose information will be updated exists
	if (getAttribut("where userid='"+user->getUserId()+"'").size() != 0) {
	  
	  if (user->getFirstname().size() != 0) {
	    mdatabaseVishnu->process("UPDATE users SET firstname='"+user->getFirstname()+"'"+"\
	    where userid='"+user->getUserId()+"'");
	  }
	  
	  if (user->getLastname().size() != 0) {
	    mdatabaseVishnu->process("UPDATE users SET lastname='"+user->getLastname()+"'"+"\
	    where userid='"+user->getUserId()+"'");
	  }
	  
	  if (user->getEmail().size() != 0) {
	    mdatabaseVishnu->process("UPDATE users SET email='"+user->getEmail()+"'"+"\
	    where userid='"+user->getUserId()+"'");
	  }
	  
	  //If the user will be locked
	  if (user->getStatus() == 1) {
	    //If the user is not already locked
	    if (convertToInt(getAttribut("where userid='"+user->getUserId()+"'", "status")) != 1) {   
	      mdatabaseVishnu->process("UPDATE users SET status="+convertToString(user->getStatus())+""+"\
	      where userid='"+user->getUserId()+"'");
	    } //End If the user is not already locked
	    else {
	      UMSVishnuException e (4, "The user is already locked");
	      throw e;
	    }
	  } //End If the user will be locked
	  else {
	    mdatabaseVishnu->process("UPDATE users SET status="+convertToString(user->getStatus())+""+"\
	    where userid='"+user->getUserId()+"'");
	  }
	  
	  // if the user whose privilege will be updated is not an admin 
	  if (convertToInt(getAttribut("where userid='"+user->getUserId()+"'", "privilege")) != 1) {   
	    mdatabaseVishnu->process("UPDATE users SET privilege="+convertToString(user->getPrivilege())+""+"\
	    where userid='"+user->getUserId()+"'");
	  }
	  else {
	    UMSVishnuException e (4, "It is not possible to change the privilege of an admin");
	    throw e;
	  }
	  
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
}
 
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
	  
	      mdatabaseVishnu->process("DELETE FROM users where userid='"+user.getUserId()+"'");
	  
	  } // End if the user who will be deleted exist
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
}
 
int 
UserServer::changePassword(std::string newPassword) {
  std::string sqlChangePwd;
  std::string sqlUpdatePwdState;
  
  try {
    //If the user exist with the flagForChangePwd to true ti avoid the passwordstate checking
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
}
 
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
	  
	    user.setPassword(generatePassword(user.getUserId(), user.getUserId()));
	    
	    sqlResetPwd = "UPDATE users SET pwd='"+user.getPassword()+"'where \
	    userid='"+user.getUserId()+"';";
	    
	    
	    //sql code to update the passwordstate
	    sqlUpdatePwdState = "UPDATE users SET passwordstate=0 \
	    where userid='"+user.getUserId()+"' and pwd='"+user.getPassword()+"';";
	    
	    sqlResetPwd.append(sqlUpdatePwdState);
	    
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
  
  //If userId and password have not been defined
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


bool UserServer::exist(bool flagForChangePwd) {
  
  bool existUser = true;
  
  try {    
      //If the user is on the database
     if (getAttribut("where \
			userid='"+muser.getUserId()+"'and pwd='"+muser.getPassword()+"'").size() != 0)	{	  
             //If the user is not locked  
	      if (!isAttributOk("status", 1)) {
		
		//if the flag that means that the user wants to change the password is not set
		//This flag avoid to check the passworState
		if (!flagForChangePwd) {
		  //If the passwordstate is active  
		  if (isAttributOk("passwordstate", 1)) {
			  return existUser;
			  /*(getAttribut("where \
			  userid='"+muser.getUserId()+"'and pwd='"+muser.getPassword()+"'").size() != 0	
			  );*/
		  } //END If the passwordstate is active  
		  else {
			UMSVishnuException e (7, "The user have to change her/his password");
			throw e;
		  }
		}//End if the flag that means that the user wants to change the password is not set
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
 
 
/* 
bool UserServer::checkLogin()
{
  return false;
}*/
 
bool UserServer::checkPassword() {
  return false;
}

//md5 encrypted of two strings
std::string 
UserServer::generatePassword(std::string lastname, std::string firstname) {
  srand(time(NULL));
  std::string salt = "$5$"+lastname + convertToString(rand())+firstname+"$";
  std::string clef = firstname+convertToString(rand());
  //remove of the string used to encrypted the string which is on the crypt result
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

bool 
UserServer::existuserId(std::string userId) {
  bool existUser = true;
  
  try {    
	//If the user is on the database
  if (getAttribut("where userid='"+userId+"'").size() != 0)	{	  
    //If the user is not locked  
    if ( convertToInt(getAttribut("where userid='"+userId+"'", "status")) != 1) {
      return existUser;     
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
}
  

	


/* 
UMS_Data::ListUsers  UserServer::list(SessionServer session, string userIdOptions)
{
	return 0;
}*/
