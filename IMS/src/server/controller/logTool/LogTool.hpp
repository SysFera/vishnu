/**
 * \file LogTool.hpp
 * \brief This file defines the generic log tool class
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 18/04/11
 */

#ifndef __LOGTOOL__HH__
#define __LOGTOOL__HH__

#include "LogORBMgr.hh"

using namespace std;

class LogTool:  public POA_ToolMsgReceiver,
		public PortableServer::RefCountServantBase{
public:
  static const int HOSTSIZE = 100;
  /**
   * \brief Constructor
   */
  LogTool();
  /**
   * \brief Destructor
   */
  ~LogTool();
  /**
   * \brief To run the program
   */
  virtual void run() = 0;
  /**
   * \brief To disconnect the tool
   * \return The disconnexion value
   */
  virtual int disconnect() = 0;
  /**
   * \brief Function called when a message is received
   * \param msg: The message to send
   */
  virtual void sendMsg(const log_msg_buf_t& msg) = 0;
  /**
   * \brief To set the filter to use
   * \param The path to the description file
   */
  virtual void setFilter(string description_file) = 0;
  /**
   * \brief To set the name of the output file
   * \param name: The name of the output file
   */
  virtual void setFilename(string name) = 0;
protected:
  /**
   * \brief The log central tool
   */
  LogCentralTool_ptr mLCT;
  /**
   * The name of the output log file
   */
  string mfilename;
  /**
   * \brief The filter
   */
  filter_t mfilter;
  /**
   * \brief The name of the component
   */
  string mname;
private:
  
};

#endif

