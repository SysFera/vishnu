/**
 * \file SOCITemporaryTypes.hpp
 * \brief This file presents tempory types used by SOCI session.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 05/07/12
 */

#ifndef _SOCITEMPORARYTYPES_H_
#define _SOCITEMPORARYTYPES_H_

#include <soci/soci.h>

using namespace soci;

class temporary_type
{
public:
	details::once_temp_type once;

	temporary_type(soci::session & sess)
		:once(sess)
	{}
	temporary_type(temporary_type & other)
			:once(other.once)
	{}

	~temporary_type();

	temporary_type &operator ,(details::use_type_ptr const & in);
	temporary_type &operator ,(details::into_type_ptr const & out);

};


#endif //_SOCITEMPORARYTYPES_H_
