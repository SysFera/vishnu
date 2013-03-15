/**
 * \file NetrcReader.hpp
 * \brief This file contains the netrc file reader class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 * Eugene PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date December 2011
 */
#ifndef _NETRC_READER_H_
#define _NETRC_READER_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>                      // for pair

/**
 * \class NetrcReader
 * \brief NetrcReader class implementation
 */
class NetrcReader {

  public:
    /**
     * \brief Constructor
     */
    NetrcReader();

    /**
     * \param path  The path until the netrc file
     * \brief Constructor
     */
    NetrcReader(const std::string& path);

    /**
     * \brief Function to get the login and the password associated to a specific machine
     * defined on the netrc file
     * \param auth all the login and password returned which is associated to the machine
     * \param machineName the name of the machine defined on the netrc file
     * \return the corresponding couple (login, password)
     */
    void
    read(std::map<size_t, std::pair<std::string,std::string> >& auth, const std::string& machineName = "vishnu");

    /**
     * \brief Destructor
     */
    ~NetrcReader();

  private:

    /////////////////////////////////
    // Functions
    /////////////////////////////////
    /**
      * \brief Function to get value associated to a key
      * \param key the key associated to a value on the file
      * \param value  the value corresponding to the key on the file
      * \param tokens the parsed tokens
      * \param tab a table which allows to register the couple key value
      * \param machine_pos
      * \return true if the value has found else found
      */
    bool
    searchValueFromKey( const std::string& key,
                        std::string& value,
                        std::vector<std::string>& tokens,
                        std::map<size_t, std::pair<std::string,std::string> >& tab,
                        const size_t machine_pos);


    /**
     * \brief Function to get the login and the password associated to a specific machine
     * by parsing the netrc file
     * \param machineName the name of the machine
     * \return all the corresponding couple (login, password)
     */
  std::map<size_t, std::pair<std::string,std::string> >&
    getNetrcInfo(const std::string& machineName);

    /**
     * \brief Function to check the validity of the netrc file
     * \return raises an exception on error
     */
    void
    check();

    /**
     * \brief Function to analyze the couple key and value table
     * \param tab a table in which the couple key and value are registered
     * \param machineName the machine name used for searching the couple
     * \return The analyzed tab
     */
  std::map<size_t, std::pair<std::string,std::string> >&
  analyze(std::map<size_t, std::pair<std::string,std::string> >& tab, const std::string& machineName);

    /////////////////////////////////
    // Attributes
    /////////////////////////////////

    /**
     * \brief The path until the netrc file
     */
    std::string mpath;

};
#endif
