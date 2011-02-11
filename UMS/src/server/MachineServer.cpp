#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <exception>
#include "SystemException.hh"
#include "UMSVishnuException.hh"

#include"utilServer.hh"
using namespace utilServer;


#include "MachineServer.hpp"


MachineServer::MachineServer(UMS_Data::Machine*& machine, SessionServer& session) {
  mmachine = machine;
  msessionServer = session;
  mdatabaseVishnu = factory.getDatabaseInstance();
}
 
int 
MachineServer::add() {
  
  std::string sqlInsert = "insert into machine (vishnu_vishnuid, name, site, machineid) values ";
  
  UserServer userServer = UserServer(msessionServer);
  userServer.init();
    //if the user exists 
    if (userServer.exist()) {
      //if the user is an admin
      if (userServer.isAdmin()) {  
	//TODO:  generation Machine Id avec Kévine
	mmachine->setMachineId(mmachine->getName()+"_"+mmachine->getName());
	
	//if the machineId does not exist
	if (getAttribut("where machineid='"+mmachine->getMachineId()+"'").size() == 0) {
	  
	  //TODO: récupérer le vishnunumId su serverUMS
	  //To insert the machine
	  mdatabaseVishnu->process(sqlInsert + "("+Vishnuid::mvishnuid+",'"+mmachine->getName()+"\
	  ','"+ mmachine->getSite()+"','"+mmachine->getMachineId()+"')");
	  
	  //To insert the description of the machine
	  mdatabaseVishnu->process("insert into description (machine_nummachineid, lang, \
	  description) values \
	  ("+getAttribut("where machineid='"+mmachine->getMachineId()+"'")+",\
	  '"+ mmachine->getLanguage()+"','"+mmachine->getMachineDescription()+"')");
	  
	} //if the machineId does not exist
	else {
	  UMSVishnuException e (4, "The machineId generated already exists");
	  throw e;
	}
      } //End if the user is an admin
      else {
	  UMSVishnuException e (4, "The machine is not added. The user is not an admin");
	  throw e;
      }
    }//End if the user exists
    else {
      UMSVishnuException e (4, "The user is unknown");
      throw e;
    }
    
  return 0;
}
 
int 
MachineServer::update()
{
	return 0;
}
 
int 
MachineServer::deleteMachine() {
  
 return 0;
}
 
MachineServer::~MachineServer() {
  delete mmachine;
}
 
UMS_Data::Machine* 
MachineServer::getData() {
	return mmachine;
}

std::string 
MachineServer::getAttribut(std::string condition, std::string attrname) {
 DatabaseResult* result;
 std::vector<std::string>::iterator ii;
 
 std::string sqlCommand("SELECT "+attrname+" FROM machine "+condition);
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

int 
MachineServer::generateMachineId()
{
	return 0;
}
 /*
UMS_Data::ListMachines MachineServer::list(SessionServer session, UMS_Data::LisMachineOptions options)
{
	return 0;
}
*/