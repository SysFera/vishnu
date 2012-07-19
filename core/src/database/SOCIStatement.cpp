#include "SOCIStatement.hpp"
#include "SOCISession.hpp"
using namespace std;
using namespace soci;



SOCIStatement::SOCIStatement(SOCISession & s)
	:mstatement(s.advanced())
{

}
SOCIStatement::SOCIStatement(SOCIStatement const & other)
	:mstatement(other.mstatement)
{

}

SOCIStatement& SOCIStatement::operator=(SOCIStatement const & other)
{
	mstatement=other.mstatement;
	return *this;
}

void SOCIStatement::alloc()
{
	if(mstatement.get_backend() == NULL) {
		throw SystemException(ERRCODE_DBERR,"There is no backend for statement");
	}
	TRYCATCH(mstatement.alloc(),"Cannot alloc ressources for statement:\n")
}

void SOCIStatement::bind(values & v)
{
	TRYCATCH(mstatement.bind(v),"Failed to bind values:\n")
}

void SOCIStatement::clean_up()
{
	TRYCATCH(mstatement.clean_up(),"Failed to clean up statement:\n")
}

void SOCIStatement::prepare(std::string const & query,
        details::statement_type eType)
{
	if(mstatement.get_backend() == NULL) {
		throw SystemException(ERRCODE_DBERR,"There is no backend for statement");
	}
	TRYCATCH(mstatement.prepare(query,eType),"Failed to prepare statement:\n")
}
void SOCIStatement::define_and_bind()
{
	TRYCATCH(mstatement.define_and_bind(),"Failed to define and bind :\n")
}


bool SOCIStatement::execute(bool withDataExchange)
{
	if(mstatement.get_backend() == NULL) {
		throw SystemException(ERRCODE_DBERR,"There is no backend for statement");
	}
	try{
		return mstatement.execute(withDataExchange);
	}
	catch(exception const & e)
	{
		throw SystemException(ERRCODE_DBERR,
				string("Cannot execute statement :\n")+e.what());
	}
	return false;
}


bool SOCIStatement::fetch()
{
	if(mstatement.get_backend() == NULL) {
		throw SystemException(ERRCODE_DBERR,"There is no backend for statement");
	}
	bool ret=false;
	TRYCATCH(ret=mstatement.fetch(),"")
	return ret;
}

bool SOCIStatement::got_data() const
{
	bool ret=false;
	TRYCATCH(ret=mstatement.got_data(),"")
	return ret;
}
void SOCIStatement::describe()
{
	if(mstatement.get_backend() == NULL) {
		throw SystemException(ERRCODE_DBERR,"There is no backend for statement");
	}
	TRYCATCH(mstatement.describe(),"")
}
void SOCIStatement::set_row(row * r)
{
	TRYCATCH(mstatement.set_row(r),"Failed to set row :\n")
}


