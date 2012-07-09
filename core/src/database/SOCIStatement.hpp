/**
 * \file SOCISession.hpp
 * \brief This file presents a sql statement.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 04/07/12
 */

#ifndef _SOCISTATEMENT_H_
#define _SOCISTATEMENT_H_

#include <soci/soci.h>
#include "SystemException.hpp"


using namespace soci;

class SOCISession;

/*
 * \class SOCIStatement
 * \brief SQL statement for prepared queries
 * allows to prepare a query with exchanging data
 * \sample
 * SOCIStatement st(session);
 * st.exchange_into(name);
 * st.exchange_use(id);
 * st.alloc();
 * st.prepare("select name from persons where id = :id");
 * st.define_and_bind();
 * st.execute(true);
 */
class SOCIStatement{
public:
	/*
	 * \brief Constructor from an exisiting session
	 */
	SOCIStatement(SOCISession & sess);
	/*
	 * \brief copy constructor
	 */
	SOCIStatement(SOCIStatement const & other);
	/*
	 * \brief operator =
	 */
	SOCIStatement& operator=(SOCIStatement const & other);

	/*
	 * \brief To access the soci implementation of statement
	 */
	soci::statement & advanced(){return mstatement;}

	/*
	 * \brief function which allocates necessary internal resources.
	 */
	void alloc();

	/*
	 * \brief function, which is used to bind the values object
	 * this is used in the object-relational mapping and normally called automatically
	 */
	void bind(soci::values & v);


	/*
	 * \brief template function exhange_use
	 * To bind the first non-bound field of SQL result with the parameter 'out'
	 * \param 'out' the parameter set as output, associted with a SQL result field
	 * out can be of types :
	 * char (for character values)
	 * short, int, unsigned long, long long, double (for numeric values)
	 * char*, char[], std::string (for string values)
	 * std::tm (for datetime values)
	 * or an std::vector of those types
	 *
	 * soci::statement (for nested statements and PL/SQL cursors)
	 * soci::blob (for Binary Large OBjects)
	 * soci::row_id (for row identifiers)
	 *
	 * or a soci::row for dynamic resultset binding
	 */
	template<typename OUTPUT>
	void exchange_into(OUTPUT & out)
	{
		try
		{
			mstatement.exchange(into(out));
		}
		catch (std::exception const & e)
		{
			throw SystemException(ERRCODE_DBERR,
								std::string("Failed to exchange output data :\n"));
		}
	}

	/*
	 * \brief template function exhange_use
	 * use the value of parameter 'in' to set the first unbound paramater in the SQL query
	 * \return Raises an exception in case of bad conversion
	 * \param 'in' can be of types :
	 *
	 * char (for character values)
	 * short, int, unsigned long, long long, double (for numeric values)
	 * char*, char[], std::string (for string values)
	 * std::tm (for datetime values)
	 * or an std::vector of those types in case of bulk operations
	 *
	 * soci::statement (for nested statements and PL/SQL cursors)
	 * soci::blob (for Binary Large OBjects)
	 * soci::row_id (for row identifiers)
	 *
	 */
	template<typename INPUT>
	void exchange_use(INPUT & in)
	{
		try
		{
			mstatement.exchange(use(in));
		}
		catch (std::exception const & e)
		{
			throw SystemException(ERRCODE_DBERR,
					std::string("Failed to exchange input data :\n"));
		}
	}

	/*
	 * \brief function for cleaning up resources, normally called automatically.
	 */
	void clean_up();

	/*
	 * \brief function for preparing the statement for repeated execution
	 * \param std::string query the SQL query
	 * parameters must begin by ':' and cannot be between quotes '
	 * \sample "Select id from table where name=:param"
	 */
    void prepare(std::string const & query,
        details::statement_type eType = details::st_repeatable_query);

    /*
     * \brief function for actually executing the registered bindings
     *  normally called automatically
     *  must be called :
     *  after : exchange_into, exchange_use, alloc, prepare
     *  before : execute
     */
    void define_and_bind();

    /*
     * \brief function for executing the statement.
     * \param withDataExchange
     * if false then there is no data exchange with locally bound variables
     * (this form should be used if later fetch of multiple rows is foreseen).
     * \return true if there was at least one row of data returned.
     *
     */
    bool execute(bool withDataExchange = false);

    /*
     * \brief function for retrieving the next portion of the result.
     * \return true if there was new data.
     */
    bool fetch();

    /*
     * \return true if the most recent execution returned any rows
     */
    bool got_data() const;


    /*
     * \brief function for extracting the type information for the result
     * (note: no data is exchanged).
     * This is normally called automatically and only when dynamic resultset binding is used
     */
    void describe();

    /*
     * \brief function for associating the statement and row objects,
     * normally called automatically.
     */
    void set_row(soci::row * r);

    /*
     * \brief special case for binding rowset objects
     */
    template<typename OUTPUT>
    void exchange_for_rowset(OUTPUT out)
    {
    	try
		{
			mstatement.exchange_for_rowset(soci::into(out));
		}
		catch (std::exception const & e)
		{
			throw SystemException(ERRCODE_DBERR,
					std::string(""));
		}
    }

private:
	soci::statement mstatement;

};


#endif // _SOCISTATEMENT_H_
