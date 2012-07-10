/**
 * \file SOCITemporaryTypes.cpp
 * \brief This file implements temporary types used by SOCI session.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 05/07/12
 */

#include "SOCITemporaryTypes.hpp"
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

/*
 * the request was executed on the destructor of the last reference to the temporary type
 * \return raises an VishnuException in case of error
 */
temporary_type::~temporary_type()
{
	TRYCATCH((once.~once_temp_type()),"Failed to execute query")
}

temporary_type&  temporary_type::exchange(details::use_type_ptr const & in)
{
	TRYCATCH((once,in), "")
	return *this;
}

temporary_type&  temporary_type::exchange(details::into_type_ptr const & out)
{
	TRYCATCH((once,out), "")
	return *this;
}

temporary_type&  temporary_type::operator ,(details::use_type_ptr const & in)
{
	return exchange(in);
}

temporary_type&  temporary_type::operator ,(details::into_type_ptr const & out)
{
	return exchange(out);
}
