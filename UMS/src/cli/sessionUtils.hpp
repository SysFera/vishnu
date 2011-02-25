#ifndef SESSIONUTILS_HPP
#define SESSIONUTILS_HPP

#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/foreach.hpp>


//  Boost archives
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

// To serialize a std::list
#include <boost/serialization/list.hpp>

using namespace boost::archive;

/**
 * \class SessionEntry
 * \brief A helper class to handle session authentication
 */
class SessionEntry{
public:
	SessionEntry(){}
	SessionEntry(const std::string sessionKey_, int closePolicy_=0);
	std::string getSessionKey()const;
	int  getClosePolicy()const;
friend	std::ostream& operator <<(std::ostream& os, const SessionEntry& session);

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version);
	int closePolicy;
	std::string sessionKey;
};

typedef std::list<SessionEntry> SessionContainer;// a container fo session
// record all sessions in a file
template <class T>
void saveIntoFile(SessionContainer &, const char*);


// load all sessions  from a file
template <class T>
void getFromFile(SessionContainer& , const char*);

//
SessionContainer getAllSessions(const std::string&);

void storeLastSession(const SessionEntry& ,const std::string&);
SessionEntry getLastSession(const std::string&);
void removeLastSession(const std::string &);
std::string getSessionLocation(int tpid);

void storeLastSessionKey(const std::string& ,int,int);
std::string getLastSessionKey(int);
#endif
