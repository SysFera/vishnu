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
	 * FIXME : Is this method usefull ?? Maybe do not encapsule it
	 * --> it is usefull for Oracle backend
	 * Shall be called automatically
	 */
	void alloc();

	/*
	 * \brief function, which is used to bind the values object
	 * FIXME : Is this method usefull ?? Maybe do not encapsule it.
	 * this is used in the object-relational mapping and normally called automatically
	 */
	void bind(soci::values & v);




	/*
	 * \fn exchange
	 * \param out pointer to get the result of the SQL query
	 * into_type_ptr is a type returned by the template function soci::into(T)
	 * \brief To bind the the first unassociated field of the SQL result
	 * with the pointer out.
	 * This method can be repeated to associate each field of the SQL result
	 *
	 * BE CAREFUL : if the into_type_ptr out was created without soci::indicator,
	 * this methode would raise exception in case of reading a NULL value in the database
	 *
	 * \sample by executing "select id,name from table"
	 * exchange(out) --> integer id wrote where out points to.
	 * exchange(out_next) --> string name wrote where out_next points to.
	 */
	void exchange(details::into_type_ptr const & out);
	/*
	 * \fn exchange
	 * \param in pointer to value used to set a parameter in the SQL query
	 * use_type_ptr is a type returned by the template function soci::use(T)
	 * \brief To bind the the first unassociated parameter in SQL query
	 * with the pointer in.
	 * This method can be repeated to bind each parameter in the SQL query
	 * \sample by executing "select * from table where id=:param1 or name=:param2"
	 * exchange(in) --> :param1 is set with the value pointed by in
	 * exchange(in_next) --> :param2 is set with the value pointed by in_next
	 */
	void exchange(details::use_type_ptr const & in);








	/* \fn template function exchange_into(...)
	 * \brief
	 * write the first unassociated field of the SQL result in parameter 'out'
	 *
	 * Important : if the read data is a NULL value, out does not change value.
	 *
	 * \return Raises an exception in case of bad conversion
	 * \param first parameter 'out' can be of types :
	 *
	 * char (for character values)
	 * short, int, unsigned long, long long, double (for numeric values)
	 * char*, char[], std::string (for string values)
	 * std::tm (for datetime values)
	 * or an std::vector of those types
	 * IMPORTANT : if output is a std::vector, it must have been declared with a maximum size
	 * which represents the maximum number of result tuples.
	 * sample : std::vector<int> values(100)
	 *
	 * soci::statement (for nested statements and PL/SQL cursors)
	 * soci::blob (for Binary Large OBjects)
	 * soci::row_id (for row identifiers)
	 *
	 * or a soci::row for dynamic resultset binding
	 *
	 * \param the second parameter is optionnal, it is a soci type : indicator
	 * soci::indicator is an enum ok : i_ok, i_null, i_truncated
	 * which is set after exchanging data
	 * i_ok : exchanging data is successfull
	 * i_null : the data to exchange is a null value
	 * i_truncated : the to exchange data is troncated
	 *
	 * If the parameter 'out' is a std::vector, the second parameter shall be too.
	 * If there is no second user parameter when calling function, a default one is set
	 * because exchanging null or truncated data raises exception
	 */
	template<typename OUTPUT>
	void exchange_into(OUTPUT & out)
	{
		indicator ind; //useful for prevent exception by reading null value
		return this->exchange(soci::into(out,ind));
	}
	template<typename OUTPUT>
	void exchange_into(OUTPUT & out, soci::indicator & ind)
	{
		return this->exchange(soci::into(out,ind));
	}

	template<typename OUTPUT>
	void exhchange_into(std::vector<OUTPUT> & out)
	{
		std::vector<indicator> inds; //useful for prevent exception by reading null value
		return this->exchange(soci::into(out,inds));
	}

	template<typename OUTPUT>
	void exchange_into(std::vector<OUTPUT> & out, std::vector<soci::indicator> & inds)
	{
		return this->exchange(soci::into(out,inds));
	}

	/*
	 * \brief template function exchange_use
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
	 *
	 * \param the second parameter is optionnal, it is a soci type : indicator
	 * soci::indicator is an enum ok : i_ok, i_null, i_truncated
	 * which is set after exchanging data
	 * i_ok : exchanging data is successfull
	 * i_null : the data to exchange is a null value
	 * i_truncated : the data to exchange data is troncated
	 */
	template<typename INPUT>
	void exchange_use(INPUT & in)
	{
		return this->exchange(soci::use(in));
	}

	template<typename INPUT>
	void exchange_use(INPUT & in, soci::indicator & indic)
	{
		return this->exchange(soci::use(in,indic));
	}


	/*
	 * \brief function for preparing the statement for repeated execution.
	 * Successive calls of prepare(query) append the SQL request string,
	 * but you cannot insert multiple commands into a prepared statement
	 * \param std::string query the SQL query
	 * parameters must begin by ':' and cannot be between quotes '
	 * \sample "Select id from table where name=:param"
	 * \return raises an exception in case of multiples command,
	 * raises an exception in case of bad synthax
	 */
    void prepare(std::string const & query,
        details::statement_type eType = details::st_repeatable_query);

    /*
     * \brief function for actually executing the registered bindings
     *  normally called automatically
     *  must be called :
     *  after : exchange_into, exchange_use, (alloc?), prepare
     *  before : execute
     */
    void define_and_bind();

	/*
	 * \brief function for cleaning up resources, normally called automatically.
	 * Deallocate all bind and define objects
	 * After clean up, the SOCI statement cannot me used, because it has no more backend.
	 * It must be re-affected from a session.
	 */
	void clean_up();

    /*
     * \brief function for executing the statement.
     * \param withDataExchange
     * if false then there is no data exchange with locally bound variables
     * (this form should be used if later fetch of multiple rows is foreseen).
     * This means that the statement should be executed,
     *  but the actual data exchange will be performed later.
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



private:
	soci::statement mstatement;

};


#endif // _SOCISTATEMENT_H_
