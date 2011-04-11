
#ifndef _FILE_EXCEPTION_HH_
#define _FILE_EXCEPTION_HH_

#include <exception>
#include <string>

/**
 * @author hguemar <hguemar@sysfera.com>
 *
 * @class FileParserError
 * @brief Base class for all FileParser exceptions
 */
class FileParserError : public std::exception {};

/**
 * @author hguemar <hguemar@sysfera.com>
 *
 * @class FileOpenError
 * @brief Configuration file cannot be opened
 */
class FileOpenError : public FileParserError
{
private:
    std::string diagnostic;
public:
    FileOpenError(const std::string& filename)
	: diagnostic("Can't open configuration file :")
	{
	    diagnostic.append(filename);
	}

    virtual ~FileOpenError() throw() {}

    virtual const char *what() const throw()
    {
	return diagnostic.c_str();
    }
};


#endif /* _FILE_EXCEPTION_HH_ */
