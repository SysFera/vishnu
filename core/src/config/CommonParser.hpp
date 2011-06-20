/**
 * @file   CommonParser.hpp
 * @author hguemar <hguemar@sysfera.com>
 * @date   Mon Jan 31 11:48:12 2011
 *
 * @brief
 * @internal
 *
 */

#ifndef _COMMON_PARSER_HH_
#define _COMMON_PARSER_HH_

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

/**
 * @brief A flux to store a debug message
 */

extern std::ostream debug;

/**
 *  @typedef std::map<std::string, std::string> ConfigMap
 *  @brief A map to store the configuration
 *  Configuration Map (key, value)
 */
typedef std::map<std::string, std::string> ConfigMap;

/**
 *  @typedef std::map<std::string, std::string> OptionMap
 *  @brief A map to store the the option
 *  Configuration/Option Map (key, value)
 */
typedef std::map<std::string, std::string> OptionMap;

/**
 * @author hguemar <hguemar@sysfera.com>
 *
 * @class StartsWith
 * @brief functor used to find short/long options
 * @internal
 */
class StartsWith : public std::unary_function<std::string, bool>
{
  private:
    /**
     * \brief the to compare with
     */
    std::string s1;
public:
    /**
     * \brief A constructor with parameters
     * \param str the string to compare with
     */
    StartsWith(const std::string& str) : s1(str) {}

    /**
     * \brief An overloaded () operator to check if a given string sart with s1
     * \param s2  a second string to perform the comparison
     * \return true if s2 starts with s1, false otherwise
     */
    bool operator() (const std::string& s2) const;
};

/**
 * @class Splitter
 * @brief functor working as token generator
 * @internal
 */
class Splitter
{
private:
  /**
   * \brief The delimiter
   */
    char delim_;
    /**
     * \brief Is equal to yes if there is a nex token
     */
    bool hasNext_;

    /**
     * \brief the size of the previous token
     */
    std::string::size_type previous_;

    /**
     * \brief the size of the current token
     */
    std::string::size_type current_;

    /**
     * \brief the current token
     */
    std::string token_;

    /**
     * \brief the string to split
     */
    std::string str_;
public:

    /**
     * \brief Default constructor
     */
    Splitter();

    /**
     * \brief constructor with parameters
     * \param c A delimiter
     */
    Splitter(const char c);

    /**
     * \brief Another  constructor with parameters
     * \param str a string to split
     * \param c A delimiter
     */

    Splitter(const std::string& str, const char c);

    /**
     * \brief To check if there is a next token
     * \return true if there is a next token in the string or false otherwise
     */
    bool hasNext() const { return hasNext_; }

    /**
     * \brief To take a new string to split
     * \param str  a string to split
     */
    void reset(const std::string& str);
    /**
     * \brief To take a new string to split
     * \param str  a string to split
     * \param c   a delimiter
     */
    void reset(const std::string& str, const char c);

    /**
     * \brief An overloaded operator () to get the token
     * \return the token
     */
    std::string& operator() ();
};

#endif /* _COMMON_PARSER_HPP_ */
