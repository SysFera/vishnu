/**
 * \file SOCITemporaryTypes.cpp
 * \brief This file implements temporary types used by SOCI session.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 05/07/12
 */

#include "SOCITemporaryType.hpp"
#include "SystemException.hpp"

using namespace std;
using namespace soci;

#ifndef TRYCATCH
#define TRYCATCH(a,b) \
	try { (a); } \
	catch (std::exception const & e) { \
	throw SystemException(ERRCODE_DBERR,std::string(b)+e.what());}
#endif

/*
 * Class temporary_type
 */

/**
 * the request was executed on the destructor of the last reference to the temporary type
 * \return raises an VishnuException in case of error
 */
SOCITemporaryType::~SOCITemporaryType()
{
	TRYCATCH((once.~once_temp_type()),"Failed to execute query \n")
}

SOCITemporaryType &
SOCITemporaryType::exchange(details::use_type_ptr const & in)
{
	TRYCATCH((once,in), "")
	return *this;
}

SOCITemporaryType &
SOCITemporaryType::exchange(details::into_type_ptr const & out)
{
	TRYCATCH((once,out), "")
	return *this;
}

SOCITemporaryType &
SOCITemporaryType::operator ,(details::use_type_ptr const & in)
{
	return exchange(in);
}

SOCITemporaryType &
SOCITemporaryType::operator ,(details::into_type_ptr const & out)
{
	return exchange(out);
}
