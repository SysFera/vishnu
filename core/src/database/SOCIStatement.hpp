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

class SOCIStatement{
public:
	SOCIStatement(SOCISession & sess);
	SOCIStatement(SOCIStatement const & other);
	void operator=(SOCIStatement const & other);

	soci::statement & advanced(){return mstatement;}

	void alloc();
	void bind(values & v);

	template<typename OUTPUT>
	void exchange_into(OUTPUT out)
	{
		try
		{
			mstatement.exchange(into(out));
		}
		catch (std::exception const & e)
		{

		}
	}
	template<typename INPUT>
	void exchange_use(INPUT in)
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

	void clean_up();
    void prepare(std::string const & query,
        details::statement_type eType = details::st_repeatable_query);
    void define_and_bind();
    void undefine_and_bind();
    bool execute(bool withDataExchange = false);
    long long get_affected_rows();
    bool fetch();
    bool got_data() const;

    void describe();
    void set_row(row * r);

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
