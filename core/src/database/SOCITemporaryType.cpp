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
 * \brief constructor from an existing session
 */
SOCITemporaryType::SOCITemporaryType(soci::session & sess)
	:once(sess),nbIntos(0),nbUses(0)
{}
/**
 * \brief copy constructor
 */
SOCITemporaryType::SOCITemporaryType(const SOCITemporaryType & other)
		:once(other.once),nbIntos(other.nbIntos),nbUses(other.nbUses)
{}
/**
 * the request was executed on the destructor of the last reference to the temporary type
 * \return raises an VishnuException in case of error
 */
SOCITemporaryType::~SOCITemporaryType()
{
	string tmp;
	if(nbIntos==0) {
		once,soci::into(tmp);
	}
	TRYCATCH((once.~once_temp_type()),"Failed to execute query \n")
}

SOCITemporaryType &
SOCITemporaryType::exchange(details::use_type_ptr const & in)
{
	TRYCATCH((once,in), "")
	nbUses++;
	return *this;
}

SOCITemporaryType &
SOCITemporaryType::exchange(details::into_type_ptr const & out)
{
	TRYCATCH((once,out), "")
	nbIntos++;
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
