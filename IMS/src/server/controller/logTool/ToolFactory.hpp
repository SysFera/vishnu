/**
 * \file ToolFactory.hpp
 * \brief This file defines the factory to get log tool
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 18/04/11
 */

#ifndef __TOOLFACTORY__HH__
#define __TOOLFACTORY__HH__

#include "LogTool.hpp"

/**
 * \brief Type of tool to create
 */
typedef enum toolType{
  IMSVishnuTool_v1
}toolType;

/**
 * \class ToolFactory
 * \brief Factory to generate tools
 */
class ToolFactory{
public:
  /**
   * \brief To get the singleton instance
   * \return The only instance of the factory
   */
  static ToolFactory* getInstance();
  /**
   * \brief To get a tool of type 'type'
   * \param type: The type of the tool to create
   * \param argc: Argc from main
   * \param argv: Argv from main
   * \param mid: The machine id the log tool will run
   * \return The created tool
   */
  LogTool*
  getTool(int type, int argc,  char** argv, string mid);
  /**
   * \brief Destructor
   */
  ~ToolFactory();
protected:
private:
  /**
   * \brief Constructor
   */
  ToolFactory();
  /**
   * \brief The singleton instance
   */
  static ToolFactory* mfact;
};


#endif
