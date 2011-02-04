#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include <exception>
#include "SystemException.hh"
#include "UMSVishnuException.hh"

#include "SessionServer.hh"

char* crypt(const char* clef, const char* salt);


std::string intToString(int tmp) {
  std::ostringstream out;
  out << tmp;
  return out.str();
}

int checkId(int id) {
  if (id < 0) {
    SystemException e (4, "The id of the object is incorrect");
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
     
     /*ConnectOptions_ptr connectOpt = parser.load(std::string(opt))->as< ConnectOptions >();
   
     
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
    */
    //TODO if (opt.getSubstituteUserId() != "")  vérifier qu'il est admin sinon renvoyer erreur;
    
    //TODO Récupérer le numMachine puis mettre ça dans la base de données en enregistrant la machineServer dans sa table à l'aide de record
    
    
	try {
	  host.recordMachineClient();
	  recordSessionServer(intToString(checkId(host.getId())), intToString(checkId(user.getId())));
	} catch (SystemException& e) {
	throw e;
	}    
	//TODO: machineServer et commanderServer à enregistrer dans la base de données donc créer un commanderServer
    
  } else {
    SystemException e(4, "The user is unknwon");
    throw e;
  }
  
  
  
	return 0;
}
 
int SessionServer::reconnect() {
	
	std::string sqlCommand("SELECT sessionKey FROM vsession where sessionId='");
	sqlCommand.append(msession.getSessionId()+ "'");
	
	std::cout <<"SQL COMMAND:"<<sqlCommand;
        try {
	   DatabaseResult* result = mdatabaseVishnu->getResult(sqlCommand.c_str());
	
	    std::cout << "Nb résulats:" << result->getNbTuples() << std::endl;
	    //std::cout<<result->getNbTuples();
	    if (result->getNbTuples() != 0) {
	    //TODO: mettre le résultat de dans sessionKey
	      result->print();
	    } 
	    else {
	    //TODO://Throw UMSexception il n'y pas de reponse
	    }
	} catch (SystemException& e) {
	std::cout << "Message generique <-> 1: " << e.getMsg()<<std::endl;
	std::cout << "Details supplementaires 2: " << e.what() <<std::endl;  
	}
	
	return 0;
}
 
int SessionServer::close() {
  
  if (exist()) {
    //TODO: if no running commands
    std::string sqlCommand("UPDATE vsession SET state=0 WHERE sessionkey='"+msession.getSessionKey()+"'");
    try {
	mdatabaseVishnu->process(sqlCommand.c_str());  
    } catch (SystemException& e) {
	throw e;
    }
    
    
    
  } 
  else {
    SystemException e (4, "Unknown sessionKey");
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
  
  //sprintf(tmp,"%x#%s", crypt(clef,salt.c_str())+5);
  //printf("tmp: %s\n",tmp);
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
  
  sprintf(clef,"-%d/%d/%d-%d:%02d:%02d:%d \n", tm->tm_mday, tm->tm_mon, tm->tm_yday, tm->tm_hour, 
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
  std::string("insert into vsession (vsessionid, clmachine_numclmachineid, users_numuserid, sessionKey, state, closepolicy, timeout) values ");
  //std::string("insert into vsession (vsessionid, clmachine_numclmachineid, users_numuserid, lastconnect, creation, closure, sessionKey, state, closepolicy, timeout) values ");
  std::string values = std::string("('" +msession.getSessionId()+"',"+idmachine+","+iduser+",'"+msession.getSessionKey()+"',");
  
  
  /*values.append(std::ostringstream << msession.getState()+",");
  values.append(std::ostringstream << msession.getClosePolicy()+",");
  values.append(std::ostringstream << msession.getTimeout()+")");
  */
  
  
  std::ostringstream out;
  
  out << msession.getState();
  values.append(out.str()+",");
  
  out.str("");
  out << msession.getClosePolicy();//TODO faire une fonction générique de convertion en string

  values.append(out.str()+",");
  out.str("");
  
  out << msession.getTimeout();
  values.append(out.str()+")");
  
  sqlInsert.append(values);
  std::cout << "SQL COMMAND:"<< sqlInsert << std::endl;
  
    try {
	  //host.recordMachineClient(); 
	  mdatabaseVishnu->process(sqlInsert.c_str());
    } 
    catch (SystemException& e) {
	throw e;
    }
} 
 
int SessionServer::exist() {
  
  //TODO: Pour éviter la duplication du code de la fonction exit (UserServer, MachineServer et SessionServer)
  //Faire une fonction générique statique  bool exist(sqlcommand) qui renvoie un booléen
  //Elle peut être défini dans sessionServer car tous les serveurs l'utilisent
  
  bool res = false;	
  DatabaseResult* result;
	
  std::string sqlCommand("SELECT * FROM vsession where sessionkey='");
  sqlCommand.append(msession.getSessionKey()+ "'");


  std::cout <<"SQL COMMAND:"<<sqlCommand;
  try {
  result = mdatabaseVishnu->getResult(sqlCommand.c_str());
  } catch (SystemException& e) {
  throw e;
  }
      std::cout << "Nb résulats:" << result->getNbTuples() << std::endl;
      if (result->getNbTuples() != 0) {
	res = true;
      } 
      else {
      res =  false;
      }

  return res;

 
  
} 
 
/*UMS_Data::ListSessions  SessionServer::list(SessionServer session, UMS_Data::ListSessionOptions  options)
{
	return 0;
}*/
