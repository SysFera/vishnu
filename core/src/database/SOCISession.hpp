/**
 * \file SOCISession.hpp
 * \brief This file presents a database session.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 03/07/12
 */

#ifndef _SOCISESSION_H_
#define _SOCISESSION_H_

#include <soci/soci.h>
#include <boost/scoped_ptr.hpp>
#include "SystemException.hpp"
#include "SOCIStatement.hpp"
#include "SOCITemporaryType.hpp"

#ifndef TRYCATCH
#define TRYCATCH(a,b) \
	try { (a); } \
	catch (std::exception const & e) { \
	throw SystemException(ERRCODE_DBERR,std::string(b)+e.what());}
#endif




/**
 * \class SOCISession
 * \brief This class represent a connection session to the database.
 * It allows different syntaxes to execute a query and exchanging data
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
 * In those syntaxes above, the user cannot acces to the indicator.
 * To acces the value of the indicator, you must use those syntaxes below :
 *
 * - syntax 1 bis
 * soci::indicator ind;
 * session.execute(query).use(myId,ind).into(name);
 *
 * - syntax 2 bis
 * soci::indicator ind;
 * session<<query,soci::use(myId,ind),soci::into(name)
 */
class SOCISession
{

public:
	/**
	 * \brief default constructor
	 * \brief no soci session is pointed
	 */
	SOCISession();
	/**
	 * \brief constructor from a pool connection
	 * \param assession : the session to use
	 * \param pos : the pool position where assesion is from
	 */
	SOCISession(soci::session * asession, size_t pos);
	/**
	 * \brief copy constructor
	 * \param s : the SOCISession to copy
	 */
	SOCISession(const SOCISession & s);
	/**
	 * \brief default destructor
	 */
	~SOCISession();

	/**
	 * \brief to acces the soci session implements
	 * it allows to use all the public function of soci::session
	 * be aware of soci exceptions
	 */
	soci::session &
	getAdvanced();
	/**
	 * \brief get the pool position where the session is from
	 */
	size_t
	getPoolPosition();

	/**
	 * \brief begin a transaction
	 * \return throw a SystemException in case of error
	 */
	void
	begin();
	/**
	 * \brief commit a transaction
	 * the transaction is ended after that function
	 * \return thow a SystemException in case of error
	 */
	void
	commit();
	/**
	 * \brief rollback a transaction
	 * the transaction is ended after that function
	 * \return throw a SystemException in case of error
	 */
	void
	rollback();


	/**
	 * \brief To execute a SQL query
	 * \param query : the SQL query string to execute
	 * \return temporary_type : allows output and input exchanging data
	 * see temporary_type methods into(..) and use(..)
	 */
	SOCITemporaryType
	execute(std::string const & query);
	/**
	 * \brief another syntax to execute(std::string const & query)
	 */
	SOCITemporaryType
	operator<<(std::string const & query);


	/**
	 * \brief To get a Statement initialized with the current SOCISession
	 */
	SOCIStatement
	getStatement();

	/**
	 * \brief To know if last SQL query returned some result
	 */
	bool
	got_data();

private:
	soci::session * msession;
	size_t pool_position;
};

#endif // _SOCISESSION_H_
