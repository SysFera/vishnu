/**
 * \file FileParser.hpp
 * \brief Parser for configuration file
 * \author Gael Le Mahec <gael.le.mahec@ens-lyon.fr>
 * \author Haikel Guemar <hguemar@sysfera.com>
 */
#ifndef _FILE_PARSER_HH_
#define _FILE_PARSER_HH_

#include <map>
#include <string>

#include "UserException.hpp"
#include "CommonParser.hpp"

/**
 * @class FileParser
 * @brief Simple Configuration File Parser
 *
 * support ini-like format, options are stored in a std::map
 */
class FileParser {
private:
    mutable OptionMap options_;
public:
    /**
     * @brief default constructor
     */
    FileParser();
    /**
     * @brief constructor
     * @param path path to config file
     */
    explicit FileParser(const std::string& path);
    /**
     * @brief parse configuration file
     * @param path path to config file
     * @exception FileOpenError
     */
    void parseFile(const std::string& path);

    /**
     * @brief subscript operator
     * @param key option name
     * @return option value
     */
    std::string& operator[](const std::string& key); // setter
    /**
     * @brief subscript operator
     * @param key option name
     * @return option value
     */
    const std::string& operator[](const std::string& key) const; //getter

    /**
     * @brief return options
     * @return configuration map (key, value)
     */
    const std::map<std::string, std::string>& getConfiguration() const;
};

#endif /* _FILE_PARSER_HH_ */
