/**
 * \file tmssed.cpp
 * \brief This file contains the VISHNU TMS sed main function.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <sys/types.h>
#include <signal.h>
#include <boost/scoped_ptr.hpp>

#include "MachineServer.hpp"
#include "ServerTMS.hpp"
#include "ExecConfiguration.hpp"
#include "DbConfiguration.hpp"
#include "MonitorTMS.hpp"

//For ZMQ
#include "zmq.hpp"
#include "DIET_client.h"
#include "Handler.hpp"
#include "BasicHandler.hpp"
#include "Server.hpp"
#include "Message.hpp"

int ZMQServerStart(boost::scoped_ptr<ServerTMS>* tmsserver, string addr, int port, string braddr, int brport)
{
  // Prepare our context and socket for server
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REP);

// Connexion with broker
//  zmq::context_t brcontext (1);
//  zmq::socket_t brsocket (brcontext, ZMQ_REP);

  string add = addr + ":" + convertToString<int>(port);
  cout << "Binded to address: " << add << endl;
  socket.bind(add.c_str());

//  string bradd = braddr + ":" + convertToString<int>(brport);
//  brsocket.connect(bradd.c_str());
//  boost::shared_ptr<diet_profile_t> prof;
//  boost::shared_ptr<Server> s = boost::shared_ptr<Server>(new Server ("TMS", getTMSService(), addr, port));
//  boost::shared_ptr<Message> m = boost::shared_ptr<Message>(new Message("", ADSE, s, prof));
// Handler for messages
//  BasicHandler hd (m);
//  boost::shared_ptr<Message> brrep = hd.send(brsocket);
//  TreatmentData brdata;
//  BasicHandler rep(brrep);
//  rep.treat();



//  socket.bind ("tcp://*:5555");

  while (true) {
//    std::cout << "Received a message" << std::endl;

    //Receive message from ZMQ
    zmq::message_t message(0);
    try {
      if (!socket.recv(&message, 0)) {
	return false;
      }
    } catch (zmq::error_t error) {
      std::cout << "E: " << error.what() << std::endl;
      return false;
    }

    std::string data = static_cast<const char *>(message.data());
    std::cerr << "recv: \"" << data << "\", size " << data.length() << "\n";


    // Deserialize and call UMS Method
    boost::shared_ptr<diet_profile_t> profile(my_deserialize(data));
    tmsserver->get()->call(profile.get());

    // Send reply back to client
    std::string resultSerialized = my_serialize(profile.get());
//    std::cout << " Serialized to send : " << resultSerialized << std::endl;

    zmq::message_t reply(resultSerialized.length()+1);
    memcpy(reply.data(), resultSerialized.c_str(), resultSerialized.length()+1);
    socket.send(reply);
  }
  return 0;
}

/**
 * \brief To show how to use the sed
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
int
usage(char* cmd) {
  std::cerr << std::endl;
  std::cout << "Usage: " << cmd << " vishnu_config.cfg" << std::endl;
  std::cerr << std::endl;
  return 1;
}

/**
 * \brief The main function
 * \param argc Number of parameter
 * \param argv List of argument
 * \param envp Array of environment variables
 * \return The result of the diet sed call
 */
int main(int argc, char* argv[], char* envp[]) {

  int res = 0;
  int vishnuId = 0;
  int interval = 1;
  ExecConfiguration config;
  DbConfiguration dbConfig(config);
  std::string dietConfigFile;
  BatchType batchType ;
  std::string batchTypeStr;
  std::string machineId;
  std::string remoteBinDirectory;
  string TMSTYPE = "TMS";
  string cfg;

  string address;
  int port;
  string brokerAddress;
  int brokerPort;

  if (argc != 2) {
    return usage(argv[0]);
  }

  // Read the configuration
  try {
    config.initFromFile(argv[1]);
    config.getRequiredConfigValue<std::string>(vishnu::DIETCONFIGFILE, dietConfigFile);
    config.getRequiredConfigValue<int>(vishnu::VISHNUID, vishnuId);
    config.getRequiredConfigValue<int>(vishnu::INTERVALMONITOR, interval);
    config.getRequiredConfigValue<std::string>(vishnu::ADDR, address);
    config.getRequiredConfigValue<int>(vishnu::PORT, port);
    config.getRequiredConfigValue<std::string>(vishnu::BRAD, brokerAddress);
    config.getRequiredConfigValue<int>(vishnu::BRPO, brokerPort);
    if (interval < 0) {
      throw UserException(ERRCODE_INVALID_PARAM, "The Monitor interval value is incorrect");
    }
    dbConfig.check();
    config.getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, batchTypeStr);
    if (batchTypeStr == "TORQUE") {
#ifndef HAVE_TORQUE_2_3
      std::cerr << std::endl;
      std::cerr << "Error: can't initialize TORQUE batch type because this server has not been compiled with TORQUE library" << std::endl;
      std::cerr << std::endl;
      exit(1);
#endif
      batchType = TORQUE;
    } else if (batchTypeStr == "LOADLEVELER") {
#ifndef HAVE_LOADLEVELER_3_5
      std::cerr << std::endl;
      std::cerr << "Error: can't initialize LOADLEVELER batch type because this server has not been compiled with LOADLEVELER library" << std::endl;
      std::cerr << std::endl;
      exit(1);
#endif
      batchType = LOADLEVELER;
    } else if (batchTypeStr == "SLURM") {
#if !( HAVE_SLURM_2_2 || HAVE_SLURM_2_3)
      std::cerr << std::endl;
      std::cerr << "Error: can't initialize SLURM batch type because this server has not been compiled with SLURM library" << std::endl;
      std::cerr << std::endl;
      exit(1);
#endif
      batchType = SLURM;

    } else if (batchTypeStr == "LSF") {
#ifndef HAVE_LSF_7_0
      std::cerr << std::endl;
      std::cerr << "Error: can't initialize LSF batch type because this server has not been compiled with LSF library" << std::endl;
      std::cerr << std::endl;
      exit(1);
#endif
      batchType = LSF;
    } else if (batchTypeStr == "SGE") {
#ifndef HAVE_SGE_11
      std::cerr << std::endl;
      std::cerr << "Error: can't initialize SGE batch type because this server has not been compiled with SGE library" << std::endl;
      std::cerr << std::endl;
      exit(1);
#endif
      batchType = SGE;
    } else {
      std::cerr << std::endl;
      std::cerr << "Error: invalid value for batch type parameter (must be 'TORQUE' or 'LOADLEVELER' or 'SLURM' or 'LSF' or 'SGE')" << std::endl;
      std::cerr << std::endl;
      exit(1);
    }
    config.getRequiredConfigValue<std::string>(vishnu::MACHINEID, machineId);
    if (!config.getConfigValue<std::string>(vishnu::REMOTEBINDIR, remoteBinDirectory)) {
      remoteBinDirectory = ExecConfiguration::getCurrentBinaryDir();
    }
  } catch (UserException& e) {
    std::cerr << std::endl;
    std::cerr << e.what() << std::endl;
    std::cerr << std::endl;
    exit(1);
  }catch (std::exception& e) {
    std::cerr << std::endl;
    std::cerr << argv[0] << " : "<< e.what() << std::endl;
    std::cerr << std::endl;
    exit(1);
  }

  // Fork a child for UMS monitoring
  pid_t pid;
  pid_t ppid;
  pid = fork();

  if (pid > 0) {

    try {
      //Check if machineId is authorized
      if(machineId.compare(AUTOMATIC_SUBMIT_JOB_KEYWORD)==0){
        std::cerr << std::endl;
        std::cerr << AUTOMATIC_SUBMIT_JOB_KEYWORD;
        std::cerr << " is not authorized as machine identifier. It is a TMS keyword." << std::endl;
        std::cerr << std::endl;
        exit(1);
      }
      if(machineId.compare(LIST_JOBS_ON_MACHINES_KEYWORD)==0) {
        std::cerr << std::endl;
        std::cerr << LIST_JOBS_ON_MACHINES_KEYWORD;
        std::cerr << " is not authorized as machine identifier. It is a TMS keyword." << std::endl;
        std::cerr << std::endl;
        exit(1);
      }

      //Initialize the TMS Server
      boost::scoped_ptr<ServerTMS> server (ServerTMS::getInstance());
      res = server->init(vishnuId, dbConfig, machineId, batchType, remoteBinDirectory);
      registerSeD(TMSTYPE, config, cfg);

      UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
      UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
      machine->setMachineId(machineId);

      MachineServer machineServer(machine);
      machineServer.checkMachine();
      delete machine;

      // Initialize the DIET SeD
      if (!res) {
        ZMQServerStart(&server, address, port, brokerAddress, brokerPort);
//        diet_print_service_table();
//        res = diet_SeD(cfg.c_str(), argc, argv);
        unregisterSeD(TMSTYPE, machineId);
      } else {
        std::cerr << std::endl;
        std::cerr << "There was a problem during services initialization" << std::endl;
        std::cerr << std::endl;
        exit(1);
      }
    } catch (VishnuException& e) {
      std::cerr << e.what() << std::endl;
      exit(1);
    }

  }  else if (pid == 0) {
    // Initialize the TMS Monitor (Opens a connection to the database)
    MonitorTMS monitor(interval);
    dbConfig.setDbPoolSize(1);
    monitor.init(vishnuId, dbConfig, machineId, batchType);
    ppid = getppid();

    while(kill(ppid,0) == 0) {
      monitor.run();
    }
  } else {
    std::cerr << std::endl;
    std::cerr << "There was a problem to initialize the server" << std::endl;
    std::cerr << std::endl;
    exit(1);
  }
  return res;
}
