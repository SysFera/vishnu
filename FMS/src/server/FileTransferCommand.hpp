/**
 * \brief Declaration of Helper class to build a transfer command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef TRANSFERCOMMAND_HPP
#define TRANSFERCOMMAND_HPP

#include <string>
#include "FMS_Data.hpp"
#include "SessionServer.hpp"

/**
 * \brief The generic file transfer command class
 */
class FileTransferCommand {
public:
  /**
   * \brief The default constructor
   */
  FileTransferCommand();
  /**
   * \brief A constructor by value
   * \param name the name of the command
   * \param location the path of the command
   * \param recursive a flag for command recursivity
   * \param compression a flag for use compression
   * \param command the command
   */
  FileTransferCommand(const std::string& name,
                      const std::string& location,
                      bool recursive,
                      bool compression,
                      const std::string& command="",
                      int timeout=0);

  /**
   * \brief Check if the command is recursive
   * \return true if the command is recursive false otherwise
   */
  bool
  isRecursive() const;
  /**
   * \brief Check if the command uses the compression
   * \return true if the command uses compression false otherwise
   */
  bool
  useCompression() const;


private:
  /**
   * \brief The name
   */
  std::string mname;
  /**
   * \brief The location
   */
  std::string mlocation;
  /**
   * \brief The flag for recursivity
   */
  bool mrecursive;
  /**
   * \brief The flag for compression
   */
  bool mcompression;
  /**
   * \brief The command
   */
  std::string mcommand;
  /**
   * \brief timeout (seconds)
   */
  int mtimeout;
};


#endif
