/**
 * \file Watcher.hpp
 * \brief This file defines the class that creates a tool and use it
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 18/04/11
 */

#ifndef __WATCHER__HH__
#define __WATCHER__HH__


#include "LogTool.hpp"


class Watcher{
public:
  /**
   * \brief Constructor
   * \param toolType: The type of the tool used by the watcher
   * \param argc: Argc from main
   * \param argv: Argv from main
   */
  Watcher(int toolType, int argc, char** argv);
  /**
   * \brief Destructor
   */
  ~Watcher();
  /**
   * \brief To run the function
   */
  void 
  run();
protected:
private:
  /**
   * \brief The log tool to use
   */
  LogTool* mtool;
};


#endif


