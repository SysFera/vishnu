/**
 * \file IMSVishnuTool.hpp
 * \brief This file defines the IMS log tool implementation
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 18/04/11
 */

#ifndef __IMSVISHNUTOOL__HH__
#define __IMSVISHNUTOOL__HH__

#include "LogTool.hpp"

class IMSVishnuTool: public LogTool{
public:
  /**
   * \brief Constructor
   */
  IMSVishnuTool(int argc, char** argv);
  /**
   * \brief Destructor
   */
  ~IMSVishnuTool();
  /**
   * \brief To run the program
   */
  void run();
  /**
   * \brief To disconnect the tool
   * \return The disconnexion value
   */
  int disconnect();
  /**
   * \brief Function called when a message is received
   * \param msg: The message to send
   */
  void sendMsg(const log_msg_buf_t& msg);
  /**
   * \brief To set the filter to use
   * \param The path to the description file
   */
  void setFilter(string description_file);
  /**
   * \brief To set the name of the output file
   * \param name: The name of the file filter
   */
  void setFilename(string name);
protected:
private:
};


#endif
