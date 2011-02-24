#include "sessionUtils.hpp"
#include <boost/archive/archive_exception.hpp>
#include <stdexcept>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace bfs = boost::filesystem;

typedef std::list<SessionEntry> SessionContainer;

SessionEntry::SessionEntry(const std::string sessionKey_, int closePolicy_):sessionKey(sessionKey_),closePolicy(closePolicy_){}

std::string SessionEntry::getSessionKey()const{return sessionKey;}
  int  SessionEntry::getClosePolicy()const{return closePolicy;}


template<class Archive>
  void SessionEntry::serialize(Archive& ar, const unsigned int version){
		    ar & sessionKey & closePolicy ;
				  }


 std::ostream& operator <<(std::ostream& os, const SessionEntry& session){
	    std::cout << "sessionKey: " << session.sessionKey << "  closePolicy: "<< session.closePolicy << std::endl;
			  }

template <class T>
void saveIntoFile(SessionContainer& allSessions, const char* file){

	  std::ofstream ofile (file);

		  T ar(ofile);

			  ar << allSessions;

				  ofile.close();
}
template <class T>
void getFromFile(SessionContainer& allSessions, const char* file){

	  std::ifstream ifile(file);

		  T ar(ifile);

			  ar >> allSessions;

				  ifile.close();
}
void storeLastSession(const SessionEntry& session,const std::string& terminal){

	SessionContainer allSessions;


	try{

		allSessions=getAllSessions(terminal);


		allSessions.push_back(session);

		saveIntoFile<text_oarchive>(allSessions, terminal.c_str());

	}
  catch(std::exception& e){

		    //if (e.code== archive_exception:: input_stream_error){// first session for this terminal

					allSessions.push_back(session);

					saveIntoFile<text_oarchive>(allSessions, terminal.c_str());

          exit (EXIT_SUCCESS);
			//	}
				//else{

		//	throw;
			//	}
	}
}



SessionContainer getAllSessions(const std::string& terminal){

	SessionContainer allSessions;

	getFromFile<text_iarchive>(allSessions, terminal.c_str());

	return allSessions;

}

SessionEntry getLastSession(const std::string& terminal){

	SessionContainer allSessions;

	try{

		allSessions=getAllSessions(terminal);

				return allSessions.back();

	}

	catch(archive_exception& e){

		 if ((e.code== archive_exception:: input_stream_error) || (e.code== archive_exception:: invalid_signature)  ){// there  is  session stored

			 throw std::runtime_error("there is no open session ");
		 }
		 else{
       std::cerr<< "there is no open session \n";
			 throw;
		 }
	}
  catch(...){


    std::cerr<< "there is no open session \n";

    exit (EXIT_FAILURE);

  }

}




void removeLastSession(const std::string & terminal){

	SessionContainer allSessions;

	  try{

			    allSessions=getAllSessions(terminal);

					         allSessions.pop_back();

									 if (allSessions.size()){


									  saveIntoFile<text_oarchive>(allSessions, terminal.c_str());

									 }
									 else{// all session have been closed

										 std::ofstream file(terminal.c_str(), std::ios::out | std::ios::trunc);

											if (file){

												file.close();
											}
									 }
									  }

  catch(archive_exception& e){

		     if (e.code== archive_exception:: input_stream_error){// there  is  session stored

					        throw std::runtime_error("there is no open session ");
									     }

	 if(e.code== archive_exception:: output_stream_error){

								throw std::runtime_error("all sessions are closed now");
								                }


	}

}





std::string getSessionLocation(int tpid){

  // change current directory
  bfs::path home_dir = getenv("HOME");
  bfs::path  session_dir = home_dir;
  bfs::path  session_file ;
  session_dir /= ".vishnu";
  
  session_dir /= "sessions";

	std::ostringstream ss;
  ss<<tpid;
  session_file=session_dir;
  session_file/=ss.str();

  if(!bfs::exists(session_dir)){
    bfs::create_directories(session_dir);
  }
    
	
	return session_file.string();


}


