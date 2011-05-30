/**
 * \brief Declaration of Helper class to build a transfer command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef TRANSFERCOMMAND_HPP
#define TRANSFERCOMMAND_HPP

#include <string>
#include "FMS_Data.hpp"

class FileTransferCommand{

  public:

    FileTransferCommand ();
    
    
    FileTransferCommand (const std::string& name,
                 const std::string& location,
                  bool recursive,
                  bool compression,
                 const std::string& command=""
                 );
    std::string getCommand()const;

    std::string getName()const;

    std::string getLocation()const;
    bool isRecursive() const;
    bool useCompression() const;

    static FileTransferCommand* getCopyCommand(const FMS_Data::CpFileOptions& options);
  
        protected:
    void setLocation (const std::string& location);

    void setName(const std::string& name);
    void setCommand(const std::string& command);

    void addOptions (const std::string& options);

    virtual void build()=0;

  private:

    std::string mname;
    std::string mlocation;
    bool mrecursive;
    bool mcompression;
    std::string mcommand;

};


#endif
