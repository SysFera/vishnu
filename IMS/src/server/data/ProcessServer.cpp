#include "ProcessServer.hpp"
#include "DbFactory.hpp"
#include "DatabaseResult.hpp"
#include "utilServer.hpp"
#include "utilIMS.hpp"
#include "IMSVishnuException.hpp"

using namespace vishnu;

ProcessServer::ProcessServer(const UserServer session):msession(session){
  DbFactory factory;
  mcommandName = "vishnu_list_processes";
  mdatabase = factory.getDatabaseInstance();
  mop = IMS_Data::ProcessOp_ptr();
}

ProcessServer::ProcessServer(IMS_Data::ProcessOp_ptr op, const UserServer session):mop(op), msession(session){
  DbFactory factory;
  mcommandName = "vishnu_list_processes";
  mdatabase = factory.getDatabaseInstance();
}

ProcessServer::~ProcessServer(){
}

IMS_Data::ListProcesses*
ProcessServer::list(){
  if (!msession.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "get processes is an admin function. A user cannot call it");
  }

  string request = "SELECT DISTINCT dietname, launchscript, machineid, pstatus, uptime, vishnuname from process WHERE  pstatus ='"+convertToString(PRUNNING)+"' ";
  vector<string> results;
  vector<string>::iterator iter;

  if (mop->getMachineId().compare("") != 0){
    string machine = "SELECT machineid from machine where machineid='"+mop->getMachineId()+"'";
    DatabaseResult *res = mdatabase->getResult(machine.c_str());
    if(res->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE,"Unknown machine id to list the processes over");
    }
    request += "AND  machineid ='"+mop->getMachineId()+"'";
  }

  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListProcesses_ptr mlistObject = ecoreFactory->createListProcesses();

  try{
    boost::scoped_ptr<DatabaseResult> listOfProcess (mdatabase->getResult(request.c_str()));
    for (size_t i = 0; i < listOfProcess->getNbTuples(); i++){
      results.clear();
      results = listOfProcess->get(i);
      iter  = results.begin();
      IMS_Data::Process_ptr proc = ecoreFactory->createProcess();
      proc->setState(convertToInt(*(iter+3)));
      proc->setProcessName(*(iter+5));
      proc->setDietId(*(iter));
      proc->setMachineId(*(iter+2));
      proc->setTimestamp(convertToInt(*(iter+4)));
      proc->setScript(*(iter+1));
      mlistObject->getProcess().push_back(proc);
    }
  }catch (SystemException& e){
    throw (e);
  }

  return mlistObject;
}

string
ProcessServer::getCommandName(){
  return mcommandName;
}

int
ProcessServer::connectProcess(IMS_Data::Process_ptr proc){
  string request = "UPDATE  process  SET  pstatus ='"+convertToString(PRUNNING)+"',  uptime =CURRENT_TIMESTAMP WHERE  vishnuname ='"+proc->getProcessName()+"' AND  machineid ='"+proc->getMachineId()+"'";
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp(" Error connecting the process "+proc->getProcessName()+" on the machine "+proc->getMachineId());
    throw(e);
  }
  return IMS_SUCCESS;
}

int
ProcessServer::disconnectProcess(IMS_Data::Process_ptr proc){
  string request = "UPDATE  process  SET  pstatus ='"+convertToString(PDOWN)+"',  uptime =CURRENT_TIMESTAMP WHERE  dietname ='"+proc->getDietId()+"' and pstatus='"+convertToString(PRUNNING)+"'";
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp(" Error disconnecting the process "+proc->getDietId());
    throw(e);
  }
  return IMS_SUCCESS;
}
int
ProcessServer::authentifyProcess(IMS_Data::Process_ptr proc){
  string request = "UPDATE  process  SET  pstatus ='"+convertToString(PRUNNING)+"',  dietname ='"+proc->getDietId()+"',  uptime =CURRENT_TIMESTAMP WHERE  vishnuname ='"+proc->getProcessName()+"' AND  machineid ='"+proc->getMachineId()+"' AND  pstatus ='"+convertToString(PUNDEF)+"'";
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp(" Error authentificating the process"+proc->getProcessName()+" on the machine "+proc->getMachineId());
    throw(e);
  }
  return IMS_SUCCESS;
}

int
ProcessServer::stopProcess(IMS_Data::Process_ptr proc){
  string request = "UPDATE process SET pstatus='"+convertToString(PDELETED)+"', uptime=CURRENT_TIMESTAMP WHERE vishnuname='"+proc->getProcessName()+"' and machineid='"+proc->getMachineId()+"' and pstatus="+convertToString(PRUNNING);
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp(" Error stopping the process "+proc->getDietId());
    throw(e);
  }
  return IMS_SUCCESS;
}

int
ProcessServer::setRestarted(IMS_Data::Process_ptr proc){
  string request = "UPDATE process SET pstatus='"+convertToString(PRUNNING)+"', uptime=CURRENT_TIMESTAMP WHERE vishnuname='"+proc->getProcessName()+"' and machineid='"+proc->getMachineId()+"'";
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp(" Error setting the restarted process "+proc->getDietId());
    throw(e);
  }
  return IMS_SUCCESS;
}


int
ProcessServer::stopAllProcesses(IMS_Data::Process_ptr proc){
  string request = "UPDATE  process  SET  pstatus ='"+convertToString(PDELETED)+"',  uptime =CURRENT_TIMESTAMP WHERE  machineid ='"+proc->getMachineId()+"' AND  pstatus ='"+convertToString(PRUNNING)+"'";
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp(" Error stopping the process "+proc->getDietId());
    throw(e);
  }
  return IMS_SUCCESS;
}


bool
ProcessServer::isIMSSeD(string Pname){
  string req = "SELECT vishnuname from process where dietname='"+Pname+"'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVPROCESS, "Unknown process");
  }
  vector<string> res;
  res = result->get(0);
  return (string(res.at(NAMEPOS)).compare("IMS")==0);
}

void
ProcessServer::fillContent(IMS_Data::Process_ptr p) {
  string req = "SELECT pstatus, dietname, uptime, launchscript from process where vishnuname='"+p->getProcessName()+"'";
  req += " AND machineid='"+p->getMachineId()+"' order by uptime desc";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVPROCESS, "Unknown process");
  }
  vector<string> res;
  res = result->get(0);
  p->setState(convertToInt(res.at(0)));
  p->setDietId(res.at(1));
  p->setTimestamp(convertToInt(res.at(2)));
  p->setScript(res.at(3));
}

void
ProcessServer::getDataFromDietId(IMS_Data::Process_ptr p) {
  string req = "SELECT pstatus, vishnuname, machineid, uptime, launchscript from process where dietname='"+p->getDietId()+"'";
  req += " order by uptime desc";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVPROCESS, "Unknown process");
  }
  vector<string> res;
  res = result->get(0);
  p->setState(convertToInt(res.at(0)));
  p->setProcessName(res.at(1));
  p->setMachineId(res.at(2));
  p->setTimestamp(convertToInt(res.at(3)));
  p->setScript(res.at(4));
}


bool
ProcessServer::checkStopped(string machine, string type) {
  string req = "select vishnuname from process where machineid='"+machine+"' and vishnuname='"+type+"' and (pstatus='"+convertToString(PDOWN)+"' or pstatus='"+convertToString(PRUNNING)+"')";
  try {
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
    return(result->getNbTuples() == 0);
  } catch (SystemException& e) {
    throw (e);
  }
  return true;
}

void
ProcessServer::getHost(string mid, string& hostname, string& acclog) {
  string req = "select nummachineid, name from machine where machine.machineid='"+mid+"'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
  if(result->getNbTuples() == 0) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE, "Machine not found");
  }
  vector<string> res;
  res = result->get(0);
  hostname = res.at(1);

  req = "select aclogin from account, users where users.userid='root' and users.numuserid=account.users_numuserid and account.machine_nummachineid='"+res.at(0)+"'";
  boost::scoped_ptr<DatabaseResult> result2(mdatabase->getResult(req.c_str()));
  if(result2->getNbTuples() == 0) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_LOCAL_ACCOUNT, "No account found to restart the machine"+mid);
  }
  res = result2->get(0);
  acclog = res.at(0);
}


// Return the last actif ims server
string
ProcessServer::getElectedMid() {
  string req = "select process.machineid from machine, process where machine.machineid=process.machineid and process.vishnuname='IMS' and process.pstatus='"+convertToString(PRUNNING)+"' order by uptime desc";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
  if(result->getNbTuples() == 0) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_LOCAL_ACCOUNT, "No account found to restart on the machine");
  }
  vector<string> res;
  res = result->get(0);
  return string(res.at(0));
}


std::string
ProcessServer::getURI(){
  string req = "select dietname from process where machineid='"+mop->getMachineId()+"'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
  if(result->getNbTuples() == 0) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE, "Unrecognized machine id");
  }
  return result->get(0).at(0);
}
