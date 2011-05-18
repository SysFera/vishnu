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

// TODO
void
ThresholdServer::setThreshold(IMS_Data::Threshold_ptr tree) {
  // Check if threshold already exist (update or insert)

  // Get the user and machine numerical id

  //  Create the insert or update sequence

  // Call the database
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

