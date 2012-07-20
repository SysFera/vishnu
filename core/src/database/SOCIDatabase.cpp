/**
 * \file SOCIDatabase.cpp
 * \brief This file implements the SOCI database.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 16/06/12
 */
#include "SOCIDatabase.hpp"
#include <boost/scoped_ptr.hpp>
#include "SystemException.hpp"
#include "utilVishnu.hpp"


#ifdef USE_MYSQL
#include <soci/mysql/soci-mysql.h>
#endif
#ifdef USE_POSTGRES
#include <soci/postgresql/soci-postgresql.h>
#endif

using namespace std;
using namespace vishnu;
using namespace soci;

/**
 * \brief timeout for gettig connection in getConnection
 */
const int CONN_TIMEOUT = 3000; // in millisecond


/**
 * \brief Function to process the request in the database
 * \remark If transacID equals -1, a free connexion is allocated
 * from the pool, the processed request is auto-commited and then
 * the connexion is released.
 */
int SOCIDatabase::process(string request, int transacId)
{
	if (!is_connected)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Cannot process request, not connected to DB");
	}
	int reqPos = -1;
	soci::session * pconn;
	size_t pos;

	try //try to get Connection
	{
		if (transacId == -1)
		{
			pconn = getConnection(reqPos);
			pos = reqPos;
		}
		else
		{
			reqPos = -1;
			pos = transacId;
			pconn = &mpool->at(pos);
		}
	} catch (exception const & e)
	{
		throw SystemException(ERRCODE_DBERR, "Cannot get transaction");
	}

	int res;
	if (request.empty())
	{
		releaseConnection(reqPos);
		throw SystemException(ERRCODE_DBERR, "Empty SQL query");
	}

	// To separate que request in case of multiple statements
	vector<string> requests=split(request,';');
	try
	{
		for(vector<string>::const_iterator it=requests.begin();it!=requests.end();++it)
		{
			if(! it->empty()) {
			(pconn->once)<<(*it);
			}
		}
	} catch (exception const &e)
	{
		throw SystemException(ERRCODE_DBERR,
				string("Cannot process request \n") + e.what());
	}

	if (reqPos != -1)
	{
		releaseConnection(reqPos);
	}

	return SUCCESS;
}
/**
 * \brief To make a connection to the database,
 * \biref Select the DBSM backend, between MYSQL, POSTGRESQL ...
 * \fn int connect()
 * \return raises an exception on error
 */
int SOCIDatabase::connect()
{
	if (is_connected)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Cannot connect to DB, already connected to DB");
	}

	switch (mconfig.getDbType())
	{
#ifdef USE_MYSQL
	case DbConfiguration::MYSQL:
	mbackend = &mysql;
	break;
#endif //USE_MYSQL
#ifdef USE_POSTGRES
	case DbConfiguration::POSTGRESQL:
	mbackend = &postgresql;
	break;
#endif //POSTGRES
#ifdef USE_ORACLE
	case DbConfiguration::ORACLE:
		throw SystemException(ERRCODE_DBERR," ORACLE is not supported yet");
	break;
#endif
	default:
		throw SystemException(ERRCODE_DBERR,
				"Database instance type unknown or not managed");
		break;
	}

	string connectString = "";
	connectString += "dbname=" + mconfig.getDbName();
	connectString += " user=" + mconfig.getDbUserName();
	connectString += " password=" + mconfig.getDbUserPassword();
	connectString += " host=" + mconfig.getDbHost();
	if (mconfig.getDbPort() != 0)
	{
		ostringstream oss;
		oss << mconfig.getDbPort();
		connectString += " port=" + oss.str();
	}



	for (unsigned int i = 0; i < mconfig.getDbPoolSize(); i++)
	{
		soci::session & msession = mpool->at(i);
		try
		{
			msession.open(*mbackend, connectString);
		} catch (exception const &e)
		{
			throw SystemException(ERRCODE_DBERR,
					string("Cannot connect to the DB : ") + e.what());
		}
	} //for

	is_connected = true;
	return SUCCESS;
}

/**
 * \fn SOCIDatabase(DbConfiguration dbConfig)
 * \brief Constructor, raises an exception on error
 */
SOCIDatabase::SOCIDatabase(DbConfiguration dbConfig) :
	mconfig(dbConfig), mdbtype(dbConfig.getDbType())
{
	mpool = new connection_pool(mconfig.getDbPoolSize());
	is_connected = false;
}

/**
 * \fn ~Database()
 * \brief Destructor, raises an exception on error
 */
SOCIDatabase::~SOCIDatabase()
{
	if (is_connected)
	{
		disconnect();
	}
	delete mpool;
}
/**
 * \brief To disconnect from the database
 * \fn disconnect()
 * \return 0 on success, an error code otherwise
 */
int SOCIDatabase::disconnect()
{
	if (!is_connected)
	{
		throw SystemException(ERRCODE_DBERR,
				"Cannot disconnect : DB is not connected");
	}
	soci::session * pconn;
	for (unsigned int i = 0; i < mconfig.getDbPoolSize(); i++)
	{
		pconn = &(mpool->at(i));
		pconn->close();
		try
		{
			mpool->give_back(i);
		} catch (exception & e)
		{
			/* \remark the session was already free
			 * no need to throw the exception
			 * How to check if a soci::session is already free
			 * before give it back ?
			 */
		}
	}
	is_connected = false;
	return SUCCESS;
}

/**
 * \brief To get the result of the latest request (if any result)
 * \param transacId the id of the transaction if one is used
 * \return The result of the latest request
 */
DatabaseResult*
SOCIDatabase::getResult(string request, int transacId)
{
	if (!is_connected)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Cannot get request result, not connected to DB");
	}

	int reqPos;
	size_t pos;
	soci::session * pconn;

	try {
		if (transacId == -1)
		{
			getConnection(reqPos);
			pos = reqPos;
		}
		else
		{
			reqPos = -1;
			pos = transacId;
		}
		pconn = &mpool->at(pos);
	} catch (exception const & e)
	{
		throw SystemException(ERRCODE_DBERR,
				string("Cannot get connection : \n") + e.what());
	}


	if (request.empty())
	{
		releaseConnection(reqPos);
		throw SystemException(ERRCODE_DBERR, "Empty SQL query");
	}

	vector<vector<string> > resultsStr;
	vector<string> attributesNames;
	try
	{
		rowset<row> results = pconn->prepare << request;
		resultsStr = rowsetToString(results, attributesNames);
	} catch (exception const &e)
	{

		throw SystemException(ERRCODE_DBERR,
				string("Cannot get query results : \n") + e.what());
	}

	if(reqPos != -1)
	{
		releaseConnection(reqPos);
	}
	return new DatabaseResult(resultsStr, attributesNames);
}

soci::session *
SOCIDatabase::getConnection(int& id)
{

	bool available_connection;
	size_t pos;
	available_connection = mpool->try_lease(pos, CONN_TIMEOUT);

	if (available_connection)
	{
		id = (int) pos; //Transaction ID
		return &(mpool->at(pos));
	}
	else
	{
		//TIMEOUT PASSED
		id = -1;
		throw SystemException(ERRCODE_DBCONN,
				"Cannot get available DB connection : TIMEOUT passed");
	}

}

void SOCIDatabase::releaseConnection(int pos)
{
	if (pos < 0)
	{
		return;
	}


	try
	{
		mpool->give_back((size_t) pos);
	} catch (exception const &e)
	{
		throw SystemException(ERRCODE_DBCONN,
				string("Cannot release connection lock : ")+e.what());
	}

}

int SOCIDatabase::startTransaction()
{
	if (!is_connected)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Cannot start transaction, not connected to DB");
	}
	int reqPos;
	soci::session * conn = getConnection(reqPos);

	try
	{
		conn->begin();
	} catch (exception const &e)
	{
		throw SystemException(ERRCODE_DBCONN, "Failed to start transaction");
	}

	// DO NOT RELEASE THE CONNECTION, KEEPING TRANSACTION
	return reqPos;
}

void SOCIDatabase::endTransaction(int transactionID)
{
	if (!is_connected)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Cannot end transaction, not connected to DB");
	}
	size_t pos = transactionID;
	soci::session * pconn;
	try
	{
		pconn = &(mpool->at(pos));
	} catch (exception const &e)
	{
		throw SystemException(ERRCODE_DBERR,
				string("Cannot get transaction : \n") + e.what());
	}

	try
	{
		pconn->commit();
	} catch (exception const &e)
	{
		try
		{
			pconn->rollback();

		} catch (exception const &e2)
		{
			try
			{
				mpool->give_back(pos);
			} catch (exception const &e3)
			{
				throw SystemException(ERRCODE_DBCONN,
						string(
								"Failed to commit, rollback then end the transaction :\n")
								+ e.what() + "\n" + e2.what() + "\n"
								+ e3.what());
			}

			throw SystemException(ERRCODE_DBCONN,
					string("Failed to commit then rollback the transaction :\n")
							+ e.what() + "\n" + e2.what());

		}

		try
		{
			mpool->give_back(pos);
		} catch (exception const &e2)
		{
			throw SystemException(ERRCODE_DBERR,
					string("Cannot commit then end transaction :\n") + e.what()
							+ "\n" + e2.what());
		}

		throw SystemException(ERRCODE_DBCONN,
				string("Failed to commit the transaction\n") + e.what());

	}

	try
	{
		mpool->give_back(pos);
	} catch (exception const &e)
	{
		throw SystemException(ERRCODE_DBERR,
				string("Cannot end transaction : \n") + e.what());
	}

}

void SOCIDatabase::cancelTransaction(int transactionID)
{

	if (!is_connected)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Cannot cancel transaction, not connected to DB");
	}
	size_t pos = transactionID;

	soci::session * pconn;
	try
	{
		pconn = &(mpool->at(pos));
	} catch (exception const & e)
	{
		throw SystemException(ERRCODE_DBCONN,
				string("Failed to cancel transaction : ") + e.what());
	}

	try
	{
		pconn->rollback();
		mpool->give_back(pos);
	} catch (exception const &e)
	{
		throw SystemException(ERRCODE_DBCONN,
				string("Failed to cancel transaction : ") + e.what());
	}
}

void SOCIDatabase::flush(int transactionID)
{

	if (!is_connected)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Cannot flush transaction, not connected to DB");
	}
	size_t pos = transactionID;
	soci::session * conn;

	try
	{
		conn = &(mpool->at(pos));
		conn->commit();
		conn->begin(); // after a commit, needs to restart the transcation
	} catch (exception const &e)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Failed to commit the transaction");
	}

}

int SOCIDatabase::generateId(string table, string fields, string val, int tid)
{
	if (!is_connected)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Cannot generate ID, not connected to DB");
	}

	vector<string> results = vector<string>();
	vector<string>::iterator iter;
	std::string sqlCommand;
	std::string sqlCommand2;
	std::string getcpt;
	switch (mdbtype)
	{
	case DbConfiguration::MYSQL:
		sqlCommand = string("INSERT INTO " + table + fields + " values " + val);
		getcpt = string("SELECT LAST_INSERT_ID() FROM vishnu");
		try
		{
			process(sqlCommand.c_str(), tid);
			boost::scoped_ptr<DatabaseResult> result(
					getResult(getcpt.c_str(), tid));
			if (result->getNbTuples() == 0)
			{
				throw SystemException(ERRCODE_DBERR,
						"Failure generating the id");
			}
			results.clear();
			results = result->get(0);
			iter = results.begin();
		} catch (SystemException& e)
		{
			throw(e);
		}
		break;
	case DbConfiguration::POSTGRESQL:
		sqlCommand = string("INSERT INTO " + table + fields + " values " + val);
		sqlCommand2 =string("SELECT currval(pg_get_serial_sequence('vishnu', 'vishnuid'))");
		try
		{
			process(sqlCommand,tid);
			boost::scoped_ptr<DatabaseResult> result(
					getResult(sqlCommand2, tid));
			if (result->getNbTuples() == 0)
			{
				throw SystemException(ERRCODE_DBERR,
						"Failure generating the id");
			}
			results.clear();
			results = result->get(0);
			iter = results.begin();
		} catch (SystemException& e)
		{
			throw(e);
		}
		break;
	default:
		throw SystemException(ERRCODE_DBERR,
				"Database instance type unknown or not managed");
		break;
	}

	return vishnu::convertToInt(*iter);

}


SOCISession SOCIDatabase::getSingleSession(int transactionId)
{
	if (!is_connected)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Cannot get single session, not connected to DB");
	}

	session * conn=NULL;
	SOCISession ret;
	int reqPos=-1;
	try {
		if(transactionId==-1)
		{
			conn=getConnection(reqPos);
			ret  = SOCISession(conn,reqPos);
		}
		else
		{
			conn=&(mpool->at(transactionId));
			ret = SOCISession(conn,transactionId);
		}
	} catch( exception const & e)
	{
		throw SystemException(ERRCODE_DBCONN,
				string("Cannot get single session : ")+e.what());
	}

	return ret;
}

int SOCIDatabase::releaseSingleSession(SOCISession & ss)
{
	if (!is_connected)
	{
		throw SystemException(ERRCODE_DBCONN,
				"Cannot release single session, not connected to DB");
	}

	try {
	releaseConnection(ss.getPoolPosition());
	} catch (exception const & e)
	{
		// the session was already free
		throw SystemException(ERRCODE_DBCONN,
						string("The session was already free : ")+e.what());
	}
	ss=SOCISession();

	return SUCCESS;
}




/*
 * private functions
 */


/*
 * \brief convert standard time to string YYYY-MM-DD HH-MM-SS
 * \note :maybe put it in Utils
 */
std::string convertTmToString(std::tm time)
{
	string timeStr="";
	timeStr.append(convertToString(1900+time.tm_year));
	timeStr.append("-");
	timeStr.append(convertToString(time.tm_mon));
	timeStr.append("-");
	timeStr.append(convertToString(time.tm_mday));
	timeStr.append(" ");
	timeStr.append(convertToString(time.tm_hour));
	timeStr.append(":");
	timeStr.append(convertToString(time.tm_min));
	timeStr.append(":");
	timeStr.append(convertToString(time.tm_sec));

	return timeStr;
}

std::string SOCIDatabase::dataToString(const row & r, size_t pos)
{
	std::string dataStr = "";
	data_type dt = r.get_properties(pos).get_data_type();
	switch (dt)
	{
	case dt_string:
		dataStr = r.get<std::string>(pos);
		break;
	case dt_double:
		dataStr = convertToString(r.get<double>(pos));
		break;
	case dt_integer:
		dataStr = convertToString(r.get<int>(pos));
		break;
	case dt_unsigned_long:
		dataStr = convertToString(r.get<unsigned long>(pos));
		break;
	case dt_long_long:
		dataStr = convertToString(r.get<long long>(pos));
		break;
	case dt_date:
		std::tm time = r.get<std::tm>(pos);
		dataStr=convertTmToString(time);
		break;
	}


	return dataStr;
}

vector<string> SOCIDatabase::rowToString(const row & r)
{

	vector<string> rowStr;
	for (size_t i = 0; i < r.size(); ++i)
	{
		string value = "";
		//data_type dt = r.get_properties(i).get_data_type();
		indicator ind = r.get_indicator(i);

		switch (ind)
		{
		case i_ok:
			// the data was returned without problems
			value = dataToString(r, i);

			break;
		case i_null:
			// null value
			value = "";
			break;
		case i_truncated:
			value = "error:truncated";
			break;
		}

		rowStr.push_back(value);

	}

	return rowStr;
}

vector<vector<string> > SOCIDatabase::rowsetToString(rowset<row> rs,
		vector<string> & namesStr)
{
	vector<vector<string> > rsStr;
	bool namesGetted = false;

	for (rowset<row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
	{
		rsStr.push_back(rowToString(*it));

		if (!namesGetted)
		{
			namesStr = getRowAttributeNames(*it);
			namesGetted = true;
		}

	}

	return rsStr;

}

vector<string> SOCIDatabase::getRowAttributeNames(const row & r)
{
	vector<string> attributeStr;

	for (size_t i = 0; i < r.size(); ++i)
	{
		attributeStr.push_back(r.get_properties(i).get_name());

	}

	return attributeStr;
}



/*
 * \brief split a string into a vector of string
 * \param s the string to split
 * \param delim the delimiter character delim
 */
vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> elems;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


