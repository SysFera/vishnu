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
#include "SOCIStatement.hpp"
#include "SOCITemporaryTypes.hpp"

#ifndef TRYCATCH
#define TRYCATCH(a,b) \
	try { (a); } \
	catch (std::exception const & e) { \
	throw SystemException(ERRCODE_DBERR,std::string(b)+e.what());}
#endif





class SOCISession
{

public:
	/*
	 * \brief default constructor
	 * \brief no soci session is pointed
	 */
	SOCISession();
	/*
	 * \brief constructor from a pool connection
	 * \param assession : the session to use
	 * \param pos : the pool position where assesion is from
	 */
	SOCISession(soci::session * asession, size_t pos);
	/*
	 * \brief copy constructor
	 * \param s : the SOCISession to copy
	 */
	SOCISession(const SOCISession & s);
	/*
	 * \brief default destructor
	 */
	~SOCISession();

	/*
	 * \brief to acces the soci session implements
	 * it allows to use all the public function of soci::session
	 * be aware of soci exceptions
	 */
	soci::session & advanced()
	{
		return *msession;
	}
	/*
	 * \brief get the pool position where the session is from
	 */
	size_t getPoolPosition()
	{
		return pool_position;
	}

	/*
	 * \brief begin a transaction
	 * \return throw a SystemException in case of error
	 */
	void begin();
	/*
	 * \brief commit a transaction
	 * the transaction is ended after that function
	 * \return thow a SystemException in case of error
	 */
	void commit();
	/*
	 * \brief rollback a transaction
	 * the transaction is ended after that function
	 * \return throw a SystemException in case of error
	 */
	void rollback();

	/*
	 * TODO : comment functions
	 */

	template<typename T>
	temporary_type operator<<(T const & t)
	{
		temporary_type ret(*msession);
		TRYCATCH( ret.once<<t, "")
		return ret;
	}

	/*
	 * \brief template function getResult
	 * \param request : the SQL request, the input variables must begin by ':'
	 * sample : "select name from table where id=:inputid"
	 * \param typename I : type of the using input in the SQL request
	 * \param typename O : type of the output result of SQL request
	 * \brief I and O can be a standard C++ type, or a vector
	 * if soci was install with boost compatibilities, it could be a boost::tuple
	 */
	template<typename I, typename O>
	void getResult(std::string request, const I input, O & output)
	{
		std::vector<std::vector<std::string> > results;
		std::vector<std::string> attributeNames;

		try
		{
			((msession->once) << request, soci::use(input), soci::into(output));
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
	 * \param typename O : type of the output result of SQL request
	 * \brief O can be a standard C++ type, or a vector
	 * if soci was install with boost compatibilities, it could be a boost::tuple
	 */
	template<typename O>
	void getResult(std::string request, O & output)
	{
		std::vector<std::vector<std::string> > results;
		std::vector<std::string> attributeNames;

		try
		{
			((msession->once) << request, soci::into(output));
		} catch (std::exception const & e)
		{
			throw SystemException(ERRCODE_DBERR,
					std::string("Cannot get query result : \n") + e.what());
		}
		return;
	}
	/*
	 * \brief template function process
	 * \param request : the SQL request
	 */
	void process(std::string request);
	/*
	 * \brief template function process
	 * \param request : the SQL request, the input variables must begin by ':'
	 * sample : "select name from table where id=:inputid"
	 * \param typename I : type of the using input in the SQL request
	 * \brief I can be a standard C++ type, or a vector
	 * if soci was install with boost compatibilities, it could be a boost::tuple
	 */
	template<typename I>
	void process(std::string request, I & input)
	{
		std::vector<std::vector<std::string> > results;
		std::vector<std::string> attributeNames;

		try
		{
			((msession->once) << request, soci::use(input));
		} catch (std::exception const & e)
		{
			throw SystemException(ERRCODE_DBERR,
					std::string("Cannot get query result : \n") + e.what());
		}
		return;
	}

	SOCIStatement
	getStatement();

private:
	soci::session * msession;
	size_t pool_position;
};

#endif // _SOCISESSION_H_
