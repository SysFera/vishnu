/**
 * \file IMSVishnuTool.hpp
 * \brief This file defines the IMS log tool implementation
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 18/04/11
 */

#ifndef __IMSVISHNUTOOL__HH__
#define __IMSVISHNUTOOL__HH__

#include "LogTool.hpp"
#include "data/ProcessServer.hpp"

/**
 * \class IMSVishnuTool
 * \brief Class that implements the log tool for the IMS SeD in VISHNU
 */
class IMSVishnuTool: public LogTool{
public:
  /**
   * \brief Constructor
   * \param argc: The argc of the main function
   * \param argv: The argv of the main function
   * \param mid: The machine id
   */
  IMSVishnuTool(int argc, char** argv, string mid);
  /**
   * \brief Destructor
   */
  ~IMSVishnuTool();
  /**
   * \brief To run the program
   */
  void 
  run();
  /**
   * \brief To disconnect the tool
   * \return The disconnexion value
   */
  int 
  disconnect();
  /**
   * \brief Function called when a message is received
   * \param msg: The message to send
   */
  void 
  sendMsg(const log_msg_buf_t& msg);
  /**
   * \brief To set the filter to use
   * \param description_file: The path to the description file
   */
  void 
  setFilter(string description_file);
  /**
   * \brief To set the name of the output file
   * \param name: The name of the file filter
   */
  void 
  setFilename(string name);
  /**
   * \brief Return true if the imssed is the last actif imssed
   * \return True if the process is elected, false otherwise
   */
  bool
  elect();
protected:
private:
  /**
   * \brief Extract the machine if from log message (out message)
   * \param msg: The message received from the log
   * \return The hostname of the machine
   */
  string
  getMidFromOutLog(string msg);
  /**
   * \brief The process server
   */
  ProcessServer mproc;
};


#endif
