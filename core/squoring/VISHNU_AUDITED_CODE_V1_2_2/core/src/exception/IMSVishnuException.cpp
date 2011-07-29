#include "IMSVishnuException.hpp"

using namespace std;

IMSVishnuException::IMSVishnuException(const IMSVishnuException& e):UserException(e){
  mtype = VishnuException::IMS;
  initMsg();
}

IMSVishnuException::IMSVishnuException(int msg, string msgComp):UserException(msg, msgComp){
  mtype = VishnuException::IMS;
  initMsg();
}

IMSVishnuException::IMSVishnuException():UserException(){
  mtype = VishnuException::IMS;
  initMsg();
}

void
IMSVishnuException::initMsg(){
  mp.insert(pair<int, string>(ERRCODE_INVPROCESS, string("The process is not recognized as a vishnu process")));
  mp.insert(pair<int, string>(ERRCODE_INVVISHNU, string("The vishnu identificator is invalid")));
  mp.insert(pair<int, string>(ERRCODE_INVEXPORT, string("The export cannot be done")));
}
