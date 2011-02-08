/*#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>*/
#include <time.h>
#include <sys/time.h>

#include <exception>
#include "SystemException.hh"
#include "UMSVishnuException.hh"
#include "SessionServer.hh"
#include "utilServer.hh"
#include "OptionValueServer.hh"

SessionServer::SessionServer() {
  msession.setSessionKey("");
}

SessionServer::SessionServer(std::string sessionKey) {
 msession.setSessionKey(sessionKey);
 mdatabaseVishnu = factory.getDatabaseInstance(); 
}
 
SessionServer::SessionServer(UMS_Data::Session session) {
}
 
int SessionServer::connectSession(UserServer user, MachineClientServer host, std::string opt) {
 
  std::ostringstream out;
  ecorecpp::parser::parser parser;
  std::string substituteUserId;
  std::string userIdToconnect;
  
  OptionValueServer optionValueServer;
  std::string numoption;
  std::string userValuepolicy;
  std::string userValuetimeout;
  std::string defaultValuepolicy;
  std::string defaultValuetimeout;
  
 try {
   
  //if the user exist 
  if (user.exist()) {
  
     UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
     ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);
     ConnectOptions_ptr connectOpt = parser.load(std::string(opt))->as< ConnectOptions >();
   
     std::cout << "Opt:" << opt << std::endl ;
     
      switch (connectOpt->getClosePolicy()) {
       
       case 0: 
	 
	      
	      msession.setClosePolicy( 
				      optionValueServer.getClosureInfo(user.getAttribut("\
				      where userid='"+user.getData().getUserId()+"'"))
				      );
	      
	      //If the policy is not 2 (CLOSE_ON_DISCONNECT)
	      if (msession.getClosePolicy() != 2) {
		  
		  msession.setTimeout(
				      optionValueServer.getClosureInfo(user.getAttribut("\
				      where userid='"+user.getData().getUserId()+"'"), "VISHNU_TIMEOUT")
				     );
	      } 
	      
	      
	      
	      
	      /*
	      numoption = 
	      optionValueServer.getAttribut("where description='VISHNU_CLOSE_POLICY'", "numoptionid", true);
	      
	      
	      userValuepolicy = 
	      optionValueServer.getAttribut
	      (
		"where optionu_numoptionid="+numoption+
		" and users_numuserid='"
		 +user.getAttribut("where userid='"+user.getData().getUserId()+"'")
		 +"'"
	      );
	      
	      
	      if (userValuepolicy.size() == 0) {
		  
		  defaultValuepolicy = 
		  optionValueServer.getAttribut("where description='VISHNU_CLOSE_POLICY'", "defaultvalue", true);
		 
		  if (convertToInt(defaultValuepolicy) == 1) {  
		    msession.setClosePolicy(1);
		    defaultValuetimeout = 
		    optionValueServer.getAttribut("where description='VISHNU_TIMEOUT'", "defaultvalue", true);
		    msession.setTimeout(convertToInt(defaultValuetimeout));
		  } 
		  else {
		    msession.setClosePolicy(2);
		  }
	      }	//END if no option value policy is defined for the user
	      else {
		    msession.setClosePolicy(convertToInt(userValuepolicy));
		    
		    //if the value is 1 (CLOSE_ON_TIMEOUT) the timeout have to be defined
		    if (convertToInt(userValuepolicy) == 1) {
			numoption = 
			optionValueServer.getAttribut("where description='VISHNU_TIMEOUT'", "numoptionid", true);
			
			userValuetimeout = optionValueServer.getAttribut
			(
			  "where optionu_numoptionid="+numoption+
			  " and users_numuserid='"
			  +user.getAttribut("where userid='"+user.getData().getUserId()+"'")
			  +"'"
			);
			  //if no option timeout is defined for the user
			 if (userValuetimeout.size() == 0) {
			    defaultValuetimeout = 
			    optionValueServer.getAttribut("where description='VISHNU_TIMEOUT'", "defaultvalue", true);
			    msession.setTimeout(convertToInt(defaultValuetimeout));
			 } //END if no option timeout is defined for the user
			 else {
				msession.setTimeout(convertToInt(userValuetimeout));
			  }
		      
		    } // End if the the option value is 1 (CLOSE_ON_TIMEOUT)
		
	      } //End Else option value is defined for the user 
	      */
	      //std::string numoptionpolicy = optionValueServer.getAttribut("where description='VISHNU_CLOSE_POLICY'", "numoptionid", true);
       break;
       
       case 1: msession.setClosePolicy(1);
              
		if (connectOpt->getSessionInactivityDelay() != 0) {
		    msession.setTimeout(connectOpt->getSessionInactivityDelay());
		} //END the timeout is defined
		else {
		    msession.setTimeout(
				      optionValueServer.getClosureInfo(user.getAttribut("\
				      where userid='"+user.getData().getUserId()+"'"), "VISHNU_TIMEOUT")
				     );
			
			/*numoption = 
			optionValueServer.getAttribut("where description='VISHNU_TIMEOUT'", "numoptionid");
			
			userValuetimeout = optionValueServer.getAttribut
			(
			  "where optionu_numoptionid="+numoption+
			  " and users_numuserid='"
			  +user.getAttribut("where userid='"+user.getData().getUserId()+"'")
			  +"'"
			);
			  //if no option timeout is defined for the user
			 if (userValuetimeout.size() == 0) {
			    defaultValuetimeout = 
			    optionValueServer.getAttribut("where description='VISHNU_TIMEOUT'", "defaultvalue", true);
			    msession.setTimeout(convertToInt(defaultValuetimeout));
			 } //END if no option timeout is defined for the user
			 else {
				msession.setTimeout(convertToInt(userValuetimeout));
			  }*/
		}// END ELSE the timeout is defined
       
	       break;
       case 2: msession.setClosePolicy(2); 
	       break;
       
       //default: break; //Faire comme case 0
    }
     
     //if a user to substitute is defined
     if (connectOpt->getSubstituteUserId().size() != 0) {
      std::cout << "USer to substitute:" << connectOpt->getSubstituteUserId() << std::endl;
      
      // if the user is admin
      if (user.isAdmin()) {
	std::cout << "Je suis Admin" << std::endl;
	
	substituteUserId = user.getAttribut("where \
	    userid='"+connectOpt->getSubstituteUserId()+"'");
	
	//If the user to substitute exist
	if (substituteUserId.size() != 0) {
	    userIdToconnect = substituteUserId;
	 } //End If the user to substitute exist
	 else {
	    UMSVishnuException e(4, "The user to substitute is unknown!");
            throw e;
	}
	
      } // END if the user is admin
      else {
	UMSVishnuException e(4, "The substitution is an admin option!");
        throw e;
      }
      
     }  
    
     //if There is not a substituteUserId
     if (userIdToconnect.size() == 0) {
         userIdToconnect = user.getAttribut("where userid='"+user.getData().getUserId()+"'\
	 and pwd='"+user.getData().getPassword()+"'"); 
	 generateSessionKey(user.getData().getUserId());
         generateSessionId(user.getData().getUserId());
     } //END if There is not a substituteUserId 
     else {
	 generateSessionKey(connectOpt->getSubstituteUserId());
	 generateSessionId(connectOpt->getSubstituteUserId());
    }
     
     std::cout <<" After generation SessionKey:" << msession.getSessionKey()<< std::endl;
     std::cout <<" After generation SessionId:" << msession.getSessionId()<< std::endl;
     
     
     host.recordMachineClient();
     recordSessionServer(convertToString(checkId(host.getId())), userIdToconnect);
	     
    //TODO: machineServer et commanderServer à enregistrer dans la base de données donc créer un commanderServer
    
  } // END if the user exist 
  else {
    UMSVishnuException e(4, "The user is unknwon");
    throw e;
  }
  
 } catch (SystemException& e) {
	throw e;
 }
  
	return 0;
}
 
int SessionServer::reconnect (UserServer user, MachineClientServer host, std::string sessionId) {
 
 std::string key;
 int existSessionKey = 0; 
 int state;
 
 msession.setSessionId(sessionId);
 
 try {
    if (user.exist()) {
        state = getState(true);
	
	  // -1 is an error code of getState when nohting has found
	  if (state != -1) {
	    
	    if (state == 1) {
	        
	        if (user.isAdmin()) {
		  existSessionKey = getSessionkey("", "", true);  
		} //END if user is admin
		else {
		existSessionKey = getSessionkey(convertToString(checkId(host.getId())), 
						user.getAttribut("where userid='"+user.getData().getUserId()+"'\
						and pwd='"+user.getData().getPassword()+"'")
	                                        );
		}
		
		if (existSessionKey == -1) {
		    UMSVishnuException e(4, "This session has not been opened with this machine for the corresponding user");
		    throw e;
		}
	    }//END IF state = 1;
	    else {
		//if (state == 0) {
		  UMSVishnuException e(4, "The reconnection is impossible because the session is closed");
		  throw e;
		/*} //END If state = 0
		else {
		  UMSVishnuException e(4, "The state associated to the session is incorrect");
		  throw e;
		}*/
	    }
		
	  }//END if state != -1
	  else {
	    UMSVishnuException e(4, "The sessionId is unrecognized");
	    throw e;
	  }
    } //END IF user.exist
    else {
      UMSVishnuException e(4, "The user is unknwon");
      throw e;
    }
 
 } catch (SystemException& e) {
	    throw e;
	}
 
 return 0;
}
 
int SessionServer::close() {
  
  int state = getState();
    
  if (state != -1) {
     if (state != 0) {
	//TODO: if no running commands
	std::string sqlCommand("UPDATE vsession SET state=0 WHERE sessionkey='"+msession.getSessionKey()+"';");
	sqlCommand.append("UPDATE vsession SET closure=CURRENT_TIMESTAMP WHERE sessionkey='"+msession.getSessionKey()+"';");
	
	
	try {
	  //TODO: réflechir sur une transaction ici seul pb 
	    mdatabaseVishnu->process(sqlCommand.c_str());
	    //mdatabaseVishnu->process(sqlCode.c_str());
	} catch (SystemException& e) {
	    throw e;
	}
     } //END IF STATE != 0 
     else {
       UMSVishnuException e (4, "The session key is expired. The session is already closed");
       throw e;
    }
    
  } //END IF STATE != -1
  else {
    UMSVishnuException e (4, "The session key is unrecognized");
    throw e;
  }
  
  return 0;
}
 
SessionServer::~SessionServer() {
}
 
UMS_Data::Session SessionServer::getData()
{
	return msession;
}
 
int SessionServer::generateSessionKey(std::string salt) {
	
  struct timeval  tv;
  struct timezone tz;
  struct tm *tm;
  float timeMilliseconde;
  char *clef = (char *) malloc(100 * sizeof(char));
  char *tmp = (char *) malloc(200 * sizeof(char)); 
  
  srand(time(NULL));
  gettimeofday(&tv, &tz);
  tm = localtime(&tv.tv_sec);
	
  timeMilliseconde = tm->tm_hour * 3600 * 1000 + 
  tm->tm_min * 60 * 1000 + tm->tm_sec * 1000 + tv.tv_usec/1000;
  
  sprintf(clef,"%d-%d-%d-%d:%d:%d:%d (~%0.1f ms)", tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, tm->tm_hour, 
	  tm->tm_min, tm->tm_sec, (int)tv.tv_usec, timeMilliseconde*rand());
  
  //sprintf(salt,"$6$%x$",id);
  salt.append("$");
  std::string tmpSalt("$1$");
  tmpSalt.append(salt);

  std::string saltCrypted(std::string(crypt(salt.c_str(),+tmpSalt.c_str())));
  
  std::cout << "saltCrypted: "<< saltCrypted << std::endl; 
  
  std::string globalSalt ("$6$");
  globalSalt.append(saltCrypted+"$");
  
  std::cout << "SessionKey generated:" << std::string(crypt(clef,globalSalt.c_str())+5) <<std::endl;
  
  msession.setSessionKey(std::string(crypt(clef,globalSalt.c_str())+5));
  
  return 0;
}
 
int SessionServer::generateSessionId(std::string userId) {
  
  struct timeval  tv;
  struct timezone tz;
  struct tm *tm;
  char *clef = (char *) malloc(100 * sizeof(char));
  
  srand(time(NULL));
  gettimeofday(&tv, &tz);
  tm = localtime(&tv.tv_sec);
	
  sprintf(clef,"-%d-%d-%d-%d:%d:%d:%d:%d", tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, tm->tm_hour, 
	  tm->tm_min, tm->tm_sec, (int)tv.tv_usec, rand()%1000 + 1);
   
  userId.append(std::string(clef));
  msession.setSessionId(userId);
  
  return 0;
}
 
int SessionServer::checkClientMachine(MachineClientServer MachineClient) {
	return 0;
}
 
int SessionServer::recordSessionServer(std::string idmachine, std::string iduser) {
  
 std::string sqlInsert = 
 //std::string("insert into vsession (vsessionid, clmachine_numclmachineid, users_numuserid, sessionKey, state, closepolicy, timeout) values ");
 std::string("insert into vsession\
 (vsessionid, clmachine_numclmachineid, users_numuserid, lastconnect, creation, sessionKey, state, closepolicy, timeout) values "); 
 //std::string("insert into vsession (vsessionid, clmachine_numclmachineid, users_numuserid, lastconnect, creation, closure, sessionKey, state, closepolicy, timeout) values ");
 
 std::string values = std::string("('" +msession.getSessionId()+"',"+idmachine+","+iduser+",\
 CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, '"+msession.getSessionKey()+"',");
   
 values.append(convertToString(msession.getState())+",");
 values.append(convertToString(msession.getClosePolicy())+",");
 values.append(convertToString(msession.getTimeout())+")");
  
 sqlInsert.append(values);
 std::cout << "SQL COMMAND:"<< sqlInsert << std::endl;
  
 try {
      mdatabaseVishnu->process(sqlInsert.c_str());
 } 
 catch (SystemException& e) {
	throw e;
 }
} 
//int flagSessionId = 0 si la fonction utilise sessionKey et 1 si sessionIds 
bool SessionServer::exist(bool flagSessionId) {
 
  if (flagSessionId) {
  
      try {  
	  return (getState(true) != -1);  
      } catch (SystemException& e) {
	  throw e;
      } 
  }
  else {
      try {  
	  return (getState(true) != -1);  
      } catch (SystemException& e) {
	throw e;  
      }
  }
} 
 
int SessionServer::getState(bool flagSessionId) {
 DatabaseResult* result;
 std::vector<std::string>::iterator ii;
 std::string sqlCommand;
 
 if (flagSessionId) {
     sqlCommand = "SELECT state FROM vsession where vsessionid='"+msession.getSessionId()+"'";
 }
 else {
     sqlCommand = "SELECT state FROM vsession where sessionkey='"+msession.getSessionKey()+"'";
 }
 
 std::cout <<"SQL COMMAND:"<<sqlCommand;
 try {
 result = mdatabaseVishnu->getResult(sqlCommand.c_str());
 } catch (SystemException& e) {
 throw e;
 }

 if (result->getNbTuples() != 0) {
      // res = true;
      result->print();
      std::vector<std::string> tmp = result->get(0);
  
  ii=tmp.begin();
  std::istringstream str(*ii);
  int value;
  str>>value;
  std::cout << "Value: "<< value;
  return value;
    
 } // if  if (result->getNbTuples() != 0)
 else {
  return -1;
 }
}
 
int SessionServer::getSessionkey(std::string idmachine, std::string iduser, bool flagAdmin) {
 
 DatabaseResult* result;
 std::vector<std::string>::iterator ii;

 std::cout <<"SESSION:"<< msession.getSessionId() <<std::endl;
 std::string sqlCommand;
 
 if (!flagAdmin) {
 sqlCommand = "SELECT sessionkey FROM vsession \
 where vsessionid='"+msession.getSessionId()+"' and clmachine_numclmachineid="+idmachine+" and users_numuserid="+iduser;
 }
 else {
 sqlCommand = "SELECT sessionkey FROM vsession where vsessionid='"+msession.getSessionId()+"'";  
}
 
 std::cout <<"SQL COMMAND:"<<sqlCommand;
 
 try {
 result = mdatabaseVishnu->getResult(sqlCommand.c_str());
 } catch (SystemException& e) {
 throw e;
 }

 if (result->getNbTuples() != 0) {
   std::cout << "Je suis là"; 
   // res = true;
      result->print();
      std::vector<std::string> tmp = result->get(0);
  
  ii=tmp.begin();
  //std::istringstream str(*ii);
  /*int value;
  str>>value;
  std::cout << "Value: "<< value;*/
   msession.setSessionKey(*ii);
  return 0;  
 } // if  if (result->getNbTuples() != 0)
 else {
  result->printAttributesNames(); 
  return -1;
 }

}

//TODO utiliser cette fonction dans getState et getSessionKey ou simplement la généraliser pour
//éviter les répétitions de code
std::string SessionServer::getAttribut(std::string condition, std::string attrname) {
 DatabaseResult* result;
 std::vector<std::string>::iterator ii;
 
 std::string sqlCommand("SELECT "+attrname+" FROM vsession "+condition);
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
      return (*ii);
   } 
   else {
	  return "";
	}
}


/*UMS_Data::ListSessions  SessionServer::list(SessionServer session, UMS_Data::ListSessionOptions  options)
{
	return 0;
}*/
