/* 
 * utils.hpp
 * 
 * Created on: 10 january 2010
 * Author: hguemar
 *
 * some utilities
 *
 */


#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <iosfwd>

#include <boost/array.hpp>
#include <boost/process/all.hpp>

namespace utils {
    namespace bp = boost::process;

    /* free function allowing us to build a boost::process::child 
       from an existing boost::process::child */
    bp::child *copy_child(const bp::child& c);

    /* encapsulate client arguments required for diet_initialize() */
    class ClientArgs
    {
	static const int nbArgs = 2;
	char *args[3];
	boost::scoped_array<char> configFile;
	boost::scoped_array<char> exec;

	public:
	ClientArgs(std::string progName,
		   std::string config=std::string(""));
	
	int argc() const { return nbArgs; }
	// fugly 
	char **argv() const { return const_cast< char **>(args); }
	char *config() const { return configFile.get(); }
	
	friend std::ostream& operator<<(std::ostream&, const ClientArgs&);
    };

    std::ostream& operator<<(std::ostream&, const ClientArgs&);
    
}


#endif /* UTILS_HPP_ */
