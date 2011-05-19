#include "ThresholdServer.hpp"
#include "DbFactory.hpp"


ThresholdServer::ThresholdServer(const UserServer session):msession(session) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  // TODO FIX
  mvishnuId = 1;
}

ThresholdServer::ThresholdServer(const UserServer session, IMS_Data::ThresholdOp op):msession(session) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mop = op;
  // TODO FIX
  mvishnuId = 1;
}

ThresholdServer::~ThresholdServer() {
}

void
ThresholdServer::setThreshold(IMS_Data::Threshold_ptr tree) {
  bool insert = true;
  string request;
  string nuid;
  string nmid;

  // Check if threshold already exist (update or insert)
  try {
    insert = checkExist(tree);
  } catch (SystemException& e) {
    throw (e);
  }
  
  // Get the user and machine numerical id
  try {
    getUserAndMachine(tree, nuid, nmid);
  } catch (SystemException& e) {
    throw (e);
  } catch (UserException& e) {
    throw (e);
  }

  //  Create the insert or update sequence
  if (insert) {
    request = "INSERT INTO threshold (users_numuserid, machine_nummachineid, typet, value) VALUES ('"+nuid+"', '"+nmid+"', '"+convertToString(tree->getType())+"', '"+convertToString(tree->getValue())+"'";
  } else {
    request = "UPDATE threshold set users_numuserid='"+nuid+"', value='"+convertToString(tree->getValue())+"' WHERE typet='"+convertToString(tree->getType())+"' AND machine_nummachineid='"+nmid+"'";
  }
  // Call the database
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e) {
    e.appendMsgComp("Failed to set threshold");
    throw(e);
  }
}

IMS_Data::ListThreshold_ptr
ThresholdServer::getThreshold() {
  // Basic request
  string req = "SELECT * from threshold, machine, users WHERE threshold.machine_nummachineid = machine.nummachineid AND users.numuserid=threshold.users_numuserid ";
  vector<string> results = vector<string>();
  vector<string>::iterator iter;
  
  // Adding option to request
  if(mop.getMachineId().compare("")) {
    req += " AND \"machineid\"='"+mop.getMachineId()+"'";
  }
  if(mop.getMetricType()==1 || // cpuuse
     mop.getMetricType()==3 || // free disk
     mop.getMetricType()==5) { // free memory
    req += " AND \"typet\"="+mop.getMetricType();
  }
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListThreshold_ptr mlistObject = ecoreFactory->createListThreshold();
  
  try {
    // Executing the request and getting the results
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
    for (size_t i = 0; i < result->getNbTuples(); i++){
      results.clear();
      results = result->get(i);
      iter = results.begin();
      IMS_Data::Threshold_ptr tree = ecoreFactory->createThreshold();
      tree->setType(convertToInt(*(iter+3)));
      tree->setValue(convertToInt(*(iter+4)));
      tree->setMachineId(*(iter+12));
      tree->setHandler(*(iter+17));
      mlistObject->getThreshold().push_back(tree);
    }
  } catch (SystemException& e) {
    throw (e);
  }
  return mlistObject;
}

bool
ThresholdServer::checkExist(IMS_Data::Threshold_ptr tree) {
  string req = "SELECT * from threshold, machine, users WHERE threshold.machine_nummachineid = machine.nummachineid AND users.numuserid=threshold.users_numuserid ";
  req += "AND machine.machineid ='"+tree->getMachineId()+"' AND threshold.typet='"+convertToString(tree->getType())+",";
  try {
    // Executing the request and getting the results
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
    return (result->getNbTuples() == 0);
  } catch (SystemException& e) {
    throw (e);
  }
}


void
ThresholdServer::getUserAndMachine(IMS_Data::Threshold_ptr tree, string &nuid, string &nmid) {
  string req = "SELECT * from machine where machineid='"+tree->getMachineId()+"'";
  vector<string>::iterator iter;
  int privil;
  try {
    // Executing the request and getting the results
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
    if (result->getNbTuples()==0) {
      throw UserException(ERRCODE_INVALID_PARAM, "Invalid machine id for the threshold");
    }
    iter = result->get(0).begin();
    nmid = (*(iter+7));
  } catch (SystemException& e) {
    throw (e);
  }
  req = "SELECT * from users where userid='"+tree->getHandler()+"'";
  try {
    // Executing the request and getting the results
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
    if (result->getNbTuples()==0) {
      throw UserException(ERRCODE_INVALID_PARAM, "Unknown handler for the threshold");
    }
    iter = result->get(0).begin();
    nuid = (*(iter+2));
    privil = convertToInt((*(iter+6)));
    // If not an admin
    if (privil == 0) {
      throw UserException(ERRCODE_INVALID_PARAM, "Invalid handler for the threshold, it must be an admin");
    }
  } catch (SystemException& e) {
    throw (e);
  } catch (UserException& e) {
    throw (e);
  }
}
