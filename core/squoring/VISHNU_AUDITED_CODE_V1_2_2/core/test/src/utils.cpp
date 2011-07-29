#include "utils.hpp"

#include <map>
#include <ostream>
#include <iostream>

#include <boost/assign/list_inserter.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/scoped_array.hpp>
#include <boost/test/unit_test.hpp>

#include "diet_config_tests.h"

namespace utils {
    namespace ba = boost::assign;
    namespace bf = boost::filesystem;

    bp::child *copy_child(const bp::child& c)
    {
	std::map<bp::stream_id, bp::handle> handles;
	ba::insert(handles)
	    (bp::stdin_id, c.get_handle(bp::stdin_id))
	    (bp::stdout_id, c.get_handle(bp::stdout_id))
	    (bp::stderr_id, c.get_handle(bp::stderr_id));

	return new bp::child(c.get_id(), handles);
    }


    ClientArgs::ClientArgs(std::string progName,
			   std::string config)
    {
	size_t len = progName.length();
	exec.reset(new char[len + 1]);
	std::strncpy(exec.get(), progName.c_str(), len + 1);
	args[0] = exec.get();

	/* config file path */
	bf::path config_path(CONFIG_DIR);
	config_path /= config;

	const char *tmp = config_path.c_str();
	len = std::strlen(tmp);
	configFile.reset(new char[len + 1]);
	std::strncpy(configFile.get(), tmp, len+1);
	args[1] = configFile.get();
	args[2] = 0;

        std::cout << std::endl; // hack for having clean xml outputs
	BOOST_TEST_MESSAGE( "config file path : " << args[1] );
        std::cout << std::endl; // hack for having clean xml outputs
    }

    // for debugging purpose only
    std::ostream& operator<<(std::ostream& s, const ClientArgs& c)
    {
	s << boost::format("Configuration file: %1%\n"
	    		   "argc: %2%\n"
	    		   "argv[0]: %3%\n"
	    		   "argv[1]: %4%\n"
			   "argv[2]: %5%\n")
	    % c.configFile.get()
	    % c.argc()
	    % c.args[0]
	    % c.args[1]
	    % (c.args[2] == 0);
    }


}
