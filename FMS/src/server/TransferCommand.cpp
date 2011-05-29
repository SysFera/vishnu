/**
 * \brief Implementation of Helper class to build a transfer command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "TransferCommand.hpp"

std::string TransferCommand::getName()const{

  return mname; 
}


std::string TransferCommand::getLocation()const{
  return mlocation;
}

bool TransferCommand::isRecursive() const{
  return mrecursive;
}

bool TransferCommand::useCompression() const{
  return mcompression;
}

std::string TransferCommand::getCommand(const std::string& command){
  return mcommand;
}

void TransferCommand::setLocation (const std::string& location){

  mlocation=location;
}

void TransferCommand::setName(const std::string& name){

mname=name;
}


void TransferCommand::setCommand(const std::string& command){
  mcommand=command;
}



void TransferCommand::addOptions (const std::string& options){

  mcommand.append(options);
}

