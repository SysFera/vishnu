/**
 * \file SOCISession.hpp
 * \brief This file presents a database session.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 03/07/12
 */

#ifndef _SOCISESSION_H_
#define _SOCISESSION_H_

#include <soci/soci.h>
#include "SystemException.hpp"

class SOCISession
{

public:
	SOCISession();
	SOCISession(soci::session * asession, size_t pos);
	SOCISession(const SOCISession & s);
	~SOCISession();

	soci::session * getSOCIimplements()
	{
		return msession;
	}
	size_t getPoolPosition()
	{
		return pool_position;
	}

	/*
	 * \brief template function getResult
	 * \param request : the SQL request, the input variables must begin by ':'
	 * sample : "select name from table where id=:inputid"
	 * \param : input
	 */
	template<class I, class O>
	void getResult(std::string request, const I input, O & output)
	{
		std::vector<std::vector<std::string> > results;
		std::vector<std::string> attributeNames;

		try
		{
			((msession->once) << request, soci::use(input),
					soci::into(output));
		} catch (std::exception const & e)
		{
			throw SystemException(ERRCODE_DBERR,
					std::string("Cannot get query result : \n") + e.what());
		}
		return;
	}
	/*
	 * \brief template function getResult
	 * \param request : the SQL request, the input variables must begin by ':'
	 * sample : "select name from table where id=:inputid"
	 * \param : input
	 */
	template<class I>
	void process(std::string request, I & input)
	{
		std::vector<std::vector<std::string> > results;
		std::vector<std::string> attributeNames;

		try
		{
			((msession->once) << request, soci::into(input));
		} catch (std::exception const & e)
		{
			throw SystemException(ERRCODE_DBERR,
					std::string("Cannot get query result : \n") + e.what());
		}
		return;
	}
private:
	soci::session * msession;
	size_t pool_position;
};

#endif // _SOCISESSION_H_
