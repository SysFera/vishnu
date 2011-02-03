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


SessionServer::SessionServer(std::string sessionKey) {
 
    msession.setSessionKey(sessionKey);
    mdatabaseVishnu = factory.getDatabaseInstance();
  
}
 
SessionServer::SessionServer(UMS_Data::Session session) {
}
 
int SessionServer::connectSession(UserServer user, MachineClientServer host, UMS_Data::ConnectOptions*& opt)
{
 
  
  std::ostringstream out;
    
  if (user.exist()) {
  
     generateSessionKey(user.getData().getUserId());
     generateSessionId(user.getData().getUserId());
     std::cout <<" After generation SessionKey:" << msession.getSessionKey()<< std::endl;
     std::cout <<" After generation SessionId:" << msession.getSessionId()<< std::endl;
        
    /* switch (opt.getClosePolicy()) {
       case 0: //TODO: closePolicy pas défini donc aller voir dans options users sinon défault options
       break;
       
       case 1: msession.setClosePolicy(1);
               //TODO: if opt.getSessionInactivityDelay() donc aller voir dans options users sinon défault options
	       break;
       case 2: msession.setClosePolicy(2);
	       break;
       
       default: break; //return une error mauvaise option pour close policy      
    }*/
    
    //TODO if (opt.getSubstituteUserId() != "")  vérifier qu'il est admin sinon renvoyer erreur;
    
    //TODO Récupérer le numMachine puis mettre ça dans la base de données en enregistrant la machineServer dans sa table à l'aide de record
    
    //DbFactory factory;
    //Database *
    //mdatabaseVishnu = factory.getDatabaseInstance(); //TODO:à déclarer en attribut de la classe pour y avoir accès partout
	
    //try {
        
        /*std::string sqlDeb = std::string("insert into vsession (vsessionid, clmachine_numclmachineid, users_numuserid, sessionKey) values ");
        time_t current_time;
	time(&current_time);
	std::string now (std::string(ctime(&current_time)));
	std::string sqlCmd("('");
	sqlCmd.append(msession.getSessionId()+"','");
	
	//sqlCmd.append("'"+now+"',");//TODO: convertir la date du jour en timestamp
	//sqlCmd.append("'"+now+"','");
	//sqlCmd.append("1,1,'");
	//sqlCmd.append("'"+now+"','");
	
	sqlCmd.append(msession.getSessionKey()+"')");
	sqlDeb.append(sqlCmd);
	*/
	
	try {
	  host.recordMachineClient();
	  int numidhost = host.getId();
	  int numiduser = user.getId();
	  std::cout << "ID machineClient " << numidhost <<std::endl;
	  std::cout << "ID users " << numiduser <<std::endl;
	  //TODO: faire le test quand c'est égal à -1 pour les deux ids précédents
	  
	std::string sqlDeb = std::string("insert into vsession (vsessionid, clmachine_numclmachineid, users_numuserid, sessionKey) values ");
        /*time_t current_time;
	time(&current_time);
	std::string now (std::string(ctime(&current_time)));*/
	std::string sqlCmd("('");
	sqlCmd.append(msession.getSessionId()+"',");  
	
	
	out << numidhost;
	sqlCmd.append(out.str()+",");
	out.str("");
	out << numiduser;
	sqlCmd.append(out.str());
	sqlCmd.append(",'");
	//sqlCmd.append("'"+now+"',");//TODO: convertir la date du jour en timestamp
	//sqlCmd.append("'"+now+"','");
	//sqlCmd.append("1,1,'");
	//sqlCmd.append("'"+now+"','");
	
	sqlCmd.append(msession.getSessionKey()+"')");
	sqlDeb.append(sqlCmd);
	
	std::cout << "SQL COMMAND:"<< sqlDeb << std::endl;
	  
	  
	  mdatabaseVishnu->process(sqlDeb.c_str());
	} catch (SystemException& e) {
	throw e;
	}    
	//TODO: machineServer et commanderServer à enregistrer dans la base de données donc créer un commanderServer
	
	/*} catch (SystemException& e) {
	std::cout << "Message generique <-> 1: " << e.getMsg()<<std::endl;
	std::cout << "Details supplementaires 2: " << e.what() <<std::endl;  
	}*/
    
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
 
int SessionServer::checkClientMachine(MachineClientServer MachineClient)
{
	return 0;
}
 
/*UMS_Data::ListSessions  SessionServer::list(SessionServer session, UMS_Data::ListSessionOptions  options)
{
	return 0;
}*/
