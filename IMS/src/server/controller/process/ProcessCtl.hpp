/**
 * \file ProcessCtl.hpp
 * \brief This file defines the class to control the processes
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/05/11
 */

#ifndef __PROCESSCTL__HH__
#define __PROCESSCTL__HH__

#include "data/ProcessServer.hpp"


class ProcessCtl{
public:
  /**
   * \brief Constructor
   */
  ProcessCtl(string mid, UserServer user);
  /**
   * \brief Destructor
   */
  ~ProcessCtl();
  /**
   * \brief To relaunch a process
   */
  void
  restart(IMS_Data::RestartOp_ptr op);
  /**
   * \brief To stop a process
   */
  void
  stop(IMS_Data::Process_ptr p);
  /**
   * To perform load schedding operations
   */
  void
  loadShed();
protected:
private:
  /**
   * \brief To create the file for the restart function
   * \param dest: The destination file (inout)
   * \param p: The process containing the script
   */
  void 
  createFile(string& dest, IMS_Data::Process_ptr p);
  /**
   * \brief The process server
   */
  ProcessServer mp;
  /**
   * \brief The option to restart
   */
  IMS_Data::RestartOp mop;
  /**
   * \brief The machine id
   */
  string mmid;
};

#endif

