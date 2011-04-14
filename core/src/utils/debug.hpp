/**
 * \file debug.hpp
 * \brief macros for debugging
 */

#ifndef _DEBUG_HPP
#define _DEBUG_HPP

#include <iostream>

#ifdef DEBUG

#define DEBUG_MSG( X, Y ) \
        std::X <<__FILE__ << ":" <<__LINE__ << ":" << __FUNCTION__ << ": " << Y << std::endl

#else

#define DEBUG_MSG(X, Y)

#endif

#define ERROR_MSG(Y) \
        std::wcerr <<__FILE__ << ":" <<__LINE__ << ":" << __FUNCTION__ << ": " << Y << std::endl

#endif // _DEBUG_HPP