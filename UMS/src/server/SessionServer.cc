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


template <class T>
std::string convertToString(const T& val) {
  std::ostringstream out;
  out << val;
  return out.str();
}

int checkId(int id) {
  if (id < 0) {
    UMSVishnuException e (4, "The id of the object is incorrect");
    throw e;
  }
  return id;
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
    
  if (user.exist()) {
  
     generateSessionKey(user.getData().getUserId());
     generateSessionId(user.getData().getUserId());
     std::cout <<" After generation SessionKey:" << msession.getSessionKey()<< std::endl;
     std::cout <<" After generation SessionId:" << msession.getSessionId()<< std::endl;
     
     UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
     ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

     
     ConnectOptions_ptr connectOpt = parser.load(std::string(opt))->as< ConnectOptions >();
   
     std::cout << "Opt:" << opt << std::endl ;
     
      switch (connectOpt->getClosePolicy()) {
       
       case 0: //TODO: closePolicy pas défini donc aller voir dans options users sinon défault options
       break;
       
       case 1: msession.setClosePolicy(1);
               
		if (connectOpt->getSessionInactivityDelay() != 0) {
		  msession.setTimeout(connectOpt->getSessionInactivityDelay());
		} 
		else {
		//TODO:  donc aller voir dans options users sinon défault options  
		}
       
	       break;
       case 2: msession.setClosePolicy(2);
	       break;
       
       //default: break; //Faire comme case 0
    }
    
    //TODO if (opt.getSubstituteUserId() != "")  vérifier qu'il est admin sinon renvoyer erreur;
    
    //TODO Récupérer le numMachine puis mettre ça dans la base de données en enregistrant la machineServer dans sa table à l'aide de record
    
    
	try {
	  host.recordMachineClient();
	  recordSessionServer(convertToString(checkId(host.getId())), convertToString(checkId(user.getId())));
	} catch (SystemException& e) {
	throw e;
	}    
	//TODO: machineServer et commanderServer à enregistrer dans la base de données donc créer un commanderServer
    
  } else {
    UMSVishnuException e(4, "The user is unknwon");
    throw e;
  }
  
  
  
	return 0;
}
 
int SessionServer::reconnect (UserServer user, MachineClientServer host, std::string sessionId) {
 
 std::string key;
  
 msession.setSessionId(sessionId);
 
 if (user.exist()) {
    
   //if (exist()) {
    
    try {   
    int res = getSessionkey(convertToString(checkId(host.getId())));
    
    if (res == -1) {
	UMSVishnuException e(4, "The corresponding session has not been opened with this machine");
	throw e;
     }
      
    
    
    } catch (SystemException& e) {
	throw e;
    }
    
      
   /*}//ENF if res == -1
   else {
     UMSVishnuException e(4, "The sessionId is unrecognized");
     throw e;
  }*/
 } //END IF user.exist
 else {
  UMSVishnuException e(4, "The user is unknwon");
  throw e;
 }
 return 0;
}
 
int SessionServer::close() {
  
  int state = getState();
    
  if (state != -1) {
     if (state != 0) {
	//TODO: if no running commands
	std::string sqlCommand("UPDATE vsession SET state=0 WHERE sessionkey='"+msession.getSessionKey()+"'");
	try {
	    mdatabaseVishnu->process(sqlCommand.c_str());
	    
	} catch (SystemException& e) {
	    throw e;
	}
     } //END IF STATE != 0 
     else {
       UMSVishnuException e (4, "The sessionKey is expired. The session is already closed");
       throw e;
    }
    
  } //END IF STATE != -1
  else {
    UMSVishnuException e (4, "The sessionKey is unrecognized");
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
  //char *salt = (char *) malloc(20 * sizeof(char));
  char *tmp = (char *) malloc(200 * sizeof(char)); 
  
  
  gettimeofday(&tv, &tz);
  tm = localtime(&tv.tv_sec);
	
  timeMilliseconde = tm->tm_hour * 3600 * 1000 + 
  tm->tm_min * 60 * 1000 + tm->tm_sec * 1000 + tv.tv_usec/1000;
  
  sprintf(clef,"-%d:%02d:%02d:%d (~%0.1f ms)\n", tm->tm_hour, 
	  tm->tm_min, tm->tm_sec, (int)tv.tv_usec, timeMilliseconde*rand());
  
  //sprintf(salt,"$6$%x$",id);
  salt.append("$");
  std::string tmpSalt("$1$");
  tmpSalt.append(salt);

  std::string saltCrypted(std::string(crypt(salt.c_str(),+tmpSalt.c_str())));
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
  float timeMilliseconde;
  char *clef = (char *) malloc(100 * sizeof(char));
  //char *salt = (char *) malloc(20 * sizeof(char));
  //char *tmp = (char *) malloc(200 * sizeof(char)); 
  
  
  gettimeofday(&tv, &tz);
  tm = localtime(&tv.tv_sec);
	
  timeMilliseconde = tm->tm_hour * 3600 * 1000 + 
  tm->tm_min * 60 * 1000 + tm->tm_sec * 1000 + tv.tv_usec/1000;
  
  sprintf(clef,"-%d-%d-%d-%d:%02d:%02d:%d \n", tm->tm_mday, tm->tm_mon, tm->tm_yday, tm->tm_hour, 
	  tm->tm_min, tm->tm_sec, (int)tv.tv_usec);
   
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
 std::string("insert into vsession (vsessionid, clmachine_numclmachineid, \
 users_numuserid, sessionKey, state, closepolicy, timeout) values "); 
 //std::string("insert into vsession (vsessionid, clmachine_numclmachineid, users_numuserid, lastconnect, creation, closure, sessionKey, state, closepolicy, timeout) values ");
 
 std::string values = std::string("('" +msession.getSessionId()+"\
 ',"+idmachine+","+iduser+",'"+msession.getSessionKey()+"',");
   
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
 
int SessionServer::exist() {
  
 DatabaseResult* result;	
 std::string sqlCommand("SELECT * FROM vsession where sessionkey=\
 '"+msession.getSessionKey()+ "'");
 std::cout <<"SQL COMMAND:"<<sqlCommand;
 
 try {
  result = mdatabaseVishnu->getResult(sqlCommand.c_str());
 } 
 catch (SystemException& e) {
  throw e;
 }
 std::cout << "Nb résulats:" << result->getNbTuples() << std::endl;
  
 return (result->getNbTuples() != 0); 
} 
 
int SessionServer::getState() {
 DatabaseResult* result;
 std::vector<std::string>::iterator ii;

 std::string sqlCommand("SELECT state FROM vsession where sessionkey=\
 '"+msession.getSessionKey()+ "'");
 
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
 
int SessionServer::getSessionkey(std::string idmachine) {
 
  DatabaseResult* result;
 std::vector<std::string>::iterator ii;

 std::cout <<"SESSION:"<< msession.getSessionId() <<std::endl;
 
 std::string sqlCommand("SELECT sessionkey FROM vsession where vsessionid='"+msession.getSessionId()+ "' and clmachine_numclmachineid="+idmachine);
 
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


/*UMS_Data::ListSessions  SessionServer::list(SessionServer session, UMS_Data::ListSessionOptions  options)
{
	return 0;
}*/
