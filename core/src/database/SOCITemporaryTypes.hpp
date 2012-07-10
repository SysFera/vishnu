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

/*
 * \class temporary_type
 * \brief temporary type used when executing SQL command by SOCISession.
 * This class allows exchanging data in the SQL query :
 * - input to set value of parameters in the SQL string command
 * - output to get the result of the SQL command
 */
class temporary_type
{
public:
	/*
	 * \brief the soci implementation of the temporary type
	 * once_temp_type in one of the two soci temporary type
	 */
	details::once_temp_type once;

	/*
	 * \brief constructor from an existing session
	 */
	temporary_type(soci::session & sess)
		:once(sess)
	{}
	/*
	 * \brief copy constructor
	 */
	temporary_type(const temporary_type & other)
			:once(other.once)
	{}

	/*
	 * \brief destructor
	 * the SQL query is finally executed at the destruction of the temporary type
	 * if it is the last reference to.
	 */
	~temporary_type();

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
	temporary_type & exchange(details::into_type_ptr const & out);
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
	temporary_type & exchange(details::use_type_ptr const & in);
	/*
	 * \brief operator ,
	 * another syntax to call exchange(details::into_type_ptr)
	 */
	temporary_type & operator ,(details::into_type_ptr const & out);
	/*
	 * \brief operator ,
	 * another syntax to call exchange(details::use_type_ptr)
	 */
	temporary_type & operator ,(details::use_type_ptr const & in);



	/*
	 * \brief template function use
	 * use the value of parameter 'in' to set the first unbinded paramater in the SQL query
	 * \return Raises an exception in case of bad conversion
	 * \param in can be of types :
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
	temporary_type & use(INPUT in)
	{
		return this->exchange(soci::use(in));
	}

	/* \fn template function into(...)
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
	temporary_type & into(OUTPUT & out)
	{
		indicator ind; //TODO : meilleur solution pour la lecture de champ null ?
		return this->exchange(soci::into(out,ind));
	}

	template<typename OUTPUT>
	temporary_type & into(OUTPUT & out, soci::indicator & ind)
	{
		return this->exchange(soci::into(out,ind));
	}

	template<typename OUTPUT>
	temporary_type & into(std::vector<OUTPUT> & out)
	{
		std::vector<indicator> inds; //TODO : meilleur solution pour la lecture de champ null ?
		return this->exchange(soci::into(out,inds));
	}

	template<typename OUTPUT>
	temporary_type & into(std::vector<OUTPUT> & out, std::vector<soci::indicator> & inds)
	{
		return this->exchange(soci::into(out,inds));
	}

};


#endif //_SOCITEMPORARYTYPES_H_
