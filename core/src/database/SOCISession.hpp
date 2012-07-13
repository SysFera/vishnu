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




/*
 * \class SOCISession
 * \brief This class represent a connection session to the database.
 * It allows different synthaxes to execute a query and exchanging data
 *
 * \sample
 * SOCISession session= //from a SOCIDatabase//;
 * string name;
 * int myId;
 * string query = "select name from tabe where id=:id"
 *
 * * those syntaxes are equivalent :
 * - sythax 1
 * session.execute(query).use(myId).into(name);
 *
 * - syntax 2
 * session<<query,soci::use(myId, soci::indicator() ),soci::into(name);
 *
 * The use of soci::indicator is to prevent exception by reading data with null value.
 * In those synthaxes above, the user cannot acces to the indicator.
 * To acces the value of the indicator, you must use those synthax below :
 *
 * - synthax 1 bis
 * soci::indicator ind;
 * session.execute(query).use(myId,ind).into(name);
 *
 * - synthax 2 bis
 * soci::indicator ind;
 * session<<query,soci::use(myId,ind),soci::into(name)
 */
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
	 * \brief To execute a SQL query
	 * \param query : the SQL query string to execute
	 * \return temporary_type : allows output and input exchanging data
	 * see temporary_type methods into(..) and use(..)
	 */
	temporary_type execute(std::string const & query);
	/*
	 * \brief another syntax to execute(std::string const & query)
	 */
	temporary_type operator<<(std::string const & query);


	/*
	 * \brief To get a Statement initialized with the current SOCISession
	 */
	SOCIStatement
	getStatement();

	/*
	 * \brief To know if last SQL query returned some result
	 */
	bool
	got_data();


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



private:
	soci::session * msession;
	size_t pool_position;
};

#endif // _SOCISESSION_H_
