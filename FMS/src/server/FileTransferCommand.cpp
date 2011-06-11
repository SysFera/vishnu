/**
 * \brief Implementation of Helper class to build a transfer command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "FileTransferCommand.hpp"
#include "ScpCommand.hpp"
#include "RsyncCommand.hpp"

FileTransferCommand::FileTransferCommand ():mname("UNKNOWN"), 
    mlocation("UNKNOWN"),
    mrecursive(false),
    mcompression(false),
    mcommand("UNKNOWN"){}

FileTransferCommand::FileTransferCommand (const std::string& name,
                                  const std::string& location,
                                  bool recursive,
                                  bool compression,
                                  const std::string& command
                                   ):mname(name), 
                                     mlocation(location),
                                     mrecursive(recursive),
                                     mcompression(compression),
                                     mcommand(command){}


std::string FileTransferCommand::getName()const{

  return mname; 
}


std::string FileTransferCommand::getLocation()const{
  return mlocation;
}

bool FileTransferCommand::isRecursive() const{
  return mrecursive;
}

bool FileTransferCommand::useCompression() const{
  return mcompression;
}

std::string FileTransferCommand::getCommand()const{
  return mcommand;
}

void FileTransferCommand::setLocation (const std::string& location){

  mlocation=location;
}

void FileTransferCommand::setName(const std::string& name){

mname=name;
}


void FileTransferCommand::setCommand(const std::string& command){
  mcommand=command;
}



void FileTransferCommand::addOptions (const std::string& options){

  mcommand.append(options);
}


FileTransferCommand* FileTransferCommand::getCopyCommand(const SessionServer& sessionServer,const FMS_Data::CpFileOptions& options){

  if (options.getTrCommand()==1) {// Rsync

    return new RsyncCommand(options.isIsRecursive());
  }
  else {

    return new ScpCommand(options.isIsRecursive());

  }

}
