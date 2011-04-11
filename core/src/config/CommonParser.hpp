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


extern std::ostream debug;
extern std::string nullString;

/**
 *  @typedef std::map<std::string, std::string> ConfigMap
 *
 *  Configuration Map (key, value)
 */
typedef std::map<std::string, std::string> ConfigMap;
/**
 *  @typedef std::map<std::string, std::string> OptionMap
 *
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
    std::string s1;
public:
    StartsWith(const std::string& str) : s1(str) {}

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
    char delim_;
    bool hasNext_;
    std::string::size_type previous_;
    std::string::size_type current_;
    std::string token_;
    std::string str_;
public:
    Splitter();
    Splitter(const char c);
    Splitter(const std::string& str, const char c);

    bool hasNext() const { return hasNext_; }

    void reset(const std::string& str);
    void reset(const std::string& str, const char c);

    std::string& operator() ();
};

#endif /* _COMMON_PARSER_HPP_ */
