/**
 * \brief Declaration of Helper class to build a transfer command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef TRANSFERCOMMAND_HPP
#define TRANSFERCOMMAND_HPP

#include <string>

class TransferCommand{

  public:

    TransferCommand ():mname("UNKNOWN"), 
    mlocation("UNKNOWN"),
    mrecursive(false),
    mcompression(false),
    mcommand("UNKNOWN"){}

     std::string getCommand()const=0;

    std::string getName()const;

    std::string getLocation()const;
    bool isRecursive() const;
    bool useCompression() const;

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
