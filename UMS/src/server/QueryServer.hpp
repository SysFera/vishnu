#ifndef _QUERY_SERVER_H_
#define _QUERY_SERVER_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"

//template<class QueryParameters, class ListObject> class QueryServer;

template <class QueryParameters, class ListObject>
class QueryServer
{
public:

  explicit
  QueryServer(const SessionServer session):msessionServer(session)
  {
    mlistObject = NULL;
    DbFactory factory;
    mdatabaseVishnu = factory.getDatabaseInstance();
  }

  QueryServer(QueryParameters *params, const SessionServer& session):
    mparameters(params), msessionServer(session)
  {

    mlistObject = NULL;
    DbFactory factory;
    mdatabaseVishnu = factory.getDatabaseInstance();
  };

  virtual ListObject* list() = 0;

  void addOptionRequest(const std::string& name, const std::string& value, std::string& request) {
    request.append(" and "+name+"=");
    request.append("'"+value+"'");
  }

  template <class T>
  void addIntegerOptionRequest(const std::string& name, T& value, std::string& request) {
    std::ostringstream osValue;
    osValue << value;
    request.append(" and "+name+"=");
    request.append("'"+osValue.str()+"'");
  }

  void addCondition(const std::string& name, const std::string& value, std::string& request) {
    request.append(" where "+name+"=");
    request.append("'"+value+"'");
  }

  template <class T>
  void addIntegerCondition(const std::string& name, T& value, std::string& request) {
    std::ostringstream osValue;
    osValue << value;
    request.append(" where "+name+"=");
    request.append("'"+osValue.str()+"'");
  }

  long convertToTimeType(std::string date) {
    
    if(date.size()==0) return 0;
         
      boost::posix_time::ptime pt(time_from_string(date));
      boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
      time_duration::sec_type time = (pt - epoch).total_seconds();
     
      std::cout << "**********TimeValue=" << time_t(time) << "********************" << std::endl;
    return time_t(time);

  }
  
  virtual ~QueryServer()
  {
  }

protected:

  QueryParameters *mparameters;
  SessionServer msessionServer;
  ListObject* mlistObject;

  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;

};

#endif
