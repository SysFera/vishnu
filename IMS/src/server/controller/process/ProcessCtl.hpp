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
  ProcessCtl(string mid);
  /**
   * \brief Destructor
   */
  ~ProcessCtl();
  /**
   * \brief To relaunch a process
   */
  void
  restart();
  /**
   * \brief To stop a process
   */
  void
  stop();
  /**
   * To perform load schedding operations
   */
  void
  loadShed();
protected:
private:
  ProcessServer mp;
  //  RestartOp mop;
  string mmid;
};

#endif
