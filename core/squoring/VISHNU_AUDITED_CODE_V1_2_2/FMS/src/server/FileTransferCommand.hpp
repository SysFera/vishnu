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

class FileTransferCommand{

  public:
    /**
     * \brief The default constructor
     */

    FileTransferCommand ();
    /**
     * \brief A constructor by value
     * \param name the name of the command
     * \param location the path of the command
     * \param recursive a flag for command recursivity
     * \param compression a flag for use compression
     * \param command the command 
     */

    FileTransferCommand (const std::string& name,
        const std::string& location,
        bool recursive,
        bool compression,
        const std::string& command=""
        );

    /**
     * \brief Get the command
     * \return the command
     */
    std::string getCommand()const;

    /**
     * \brief Get the command name
     * \return the command name
     */
    std::string getName()const;

    /**
     * \brief Get the command location
     * \return the command location
     */
    std::string getLocation()const;
    /**
     * \brief Check if the command is recursive
     * \return true if the command is recursive false otherwise
     */
    bool isRecursive() const;
    /**
     * \brief Check if the command uses the compression
     * \return true if the command uses compression false otherwise
     */
    bool useCompression() const;

    /**
     * \brief A factory to build a copy command
     * \param sessionServer the session object server
     * \param options the copy options
     * \return a new file transfer command
     */
    static FileTransferCommand* getCopyCommand(const SessionServer& sessionServer, const FMS_Data::CpFileOptions& options);

  protected:

    /**
     * \brief To update the command location
     * \param location the new command location
     */

    void setLocation (const std::string& location);

    /**
     * \brief To update the command name
     * \param name the new command name
     */
    void setName(const std::string& name);
    /**
     * \brief To update the command 
     * \param command the new command 
     */
    void setCommand(const std::string& command);

    /**
     * \brief To add options to the command 
     * \param options the new options to add
     */
    void addOptions (const std::string& options);
    /**
     * \brief To build the command using options
     */
    virtual void build()=0;

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

};


#endif
