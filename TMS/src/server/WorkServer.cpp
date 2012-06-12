/**
* \file WorkServer.cpp
* \brief This file implements the Class which manipulates VISHNU work data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "WorkServer.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"

/**
* \brief Constructor
* \param work The work data structure
*/
WorkServer::WorkServer(TMS_Data::Work*& work):
mwork(work)
{
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Constructor
* \param work The work data structure
* \param session The object which encapsulates session data
*/
WorkServer::WorkServer(TMS_Data::Work*& work, SessionServer& session):
mwork(work), msessionServer(session)
{
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU work
* \fn int add(int vishnuId)
* \param vishnuId The identifier of the vishnu instance
* \return raises an exception on error
*/
int
WorkServer::add(int vishnuId) {
  std::string sqlInsert = "insert into work (application_id, date_created, date_started, description, done_ratio, due_date, estimated_hours, identifier, last_updated, machine_id, nbcpus, owner_id, priority, project_id, status, subject) values ";
  std::string idWorkGenerated;
  std::string formatidwork;

  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  std::string vishnuid;

  std::string appId = "";
  std::string projectId = "";
  std::string timestamp = "CURRENT_TIMESTAMP";
  std::string owner = "";

  //if the user exists
  if (userServer.exist()) {
    //if the user is an admin
    if (userServer.isAdmin()) {

      vishnuid = convertToString(vishnuId);

      //Generation of workid
      idWorkGenerated = vishnu::getObjectId(vishnuId,
                                            "formatidwork",
                                            WORK,
                                            mwork->getSubject());

      //if the work id is generated
      if (idWorkGenerated.size() != 0) {
        mwork->setWorkId(idWorkGenerated);

        //if the workId does not exist
        if (getAttribut("where workid='"+mwork->getWorkId()+"'").size() == 0) {
          //To inactive the work status
//          mwork->setStatus(INACTIVE_STATUS);
// TODO
          mwork->setStatus(0);

          mdatabaseVishnu->process(sqlInsert + "('"+appId+"',"+timestamp+"\
            ,"+ timestamp+",'"+mwork->getDescription()+"',"+convertToString(mwork->getDoneRatio())+", \
            '"+convertToString(mwork->getDueDate())+"', '"+convertToString(mwork->getEstimatedHour())+"' \
            , '"+idWorkGenerated+"', "+timestamp+", '"+mwork->getMachineId()+"'\
            , '"+convertToString(mwork->getNbCPU())+"', '"+owner+"', '"+convertToString(mwork->getPriority())+"' \
            , '"+projectId+"', "+convertToString(mwork->getStatus())+", '"+mwork->getSubject()+"')");


        }//if the machine id is generated
        else {
          SystemException e (ERRCODE_SYSTEM, "There is a problem to parse the formatidwork");
          throw e;
        }
      }//END if the formatidmachine is defined
      else {
        SystemException e (ERRCODE_SYSTEM, "The formatidwork is not defined");
        throw e;
      }
    } //End if the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  }//End if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
} //END: add()

//
//  /**
//  * \brief Function to update a VISHNU work
//  * \return raises an exception on error
//  */
//  int
//  WorkServer::update() {
//
//    std::string sqlCommand = "";
//
//
//
//    UserServer userServer = UserServer(msessionServer);
//    userServer.init();
//
//    //if the user exists
//    if (userServer.exist()) {
//      //if the user is an admin
//      if (userServer.isAdmin()) {
//
//        //if the work to update exists
//        if (getAttribut("where workid='"+mwork->getWorkId()+"'").size() != 0) {
//
//          //if a new work name has been defined
//          if (mwork->getName().size() != 0) {
//            sqlCommand.append("UPDATE work SET name='"+mmachine->getName()+"'\
//            where machineId='"+mmachine->getWorkId()+"';");
//          }
//
//          //if a new site has been defined
//          if (mmachine->getSite().size() != 0) {
//            sqlCommand.append("UPDATE machine SET site='"+mmachine->getSite()+"'\
//            where machineId='"+mmachine->getWorkId()+"';");
//          }
//
//          //If a new status has been defined
//          if (mmachine->getStatus() != UNDEFINED_VALUE) {
//            sqlCommand.append("UPDATE machine SET status="+convertToString(mmachine->getStatus())+
//            " where machineId='"+mmachine->getWorkId()+"';");
//          }
//
//          //if a new ssh public key has been defined
//          if (mmachine->getSshPublicKey().size() != 0) {
//            sqlCommand.append("UPDATE machine SET sshpublickey='"+mmachine->getSshPublicKey()+"'"
//            " where machineId='"+mmachine->getWorkId()+"';");
//          }
//
//          //if a new language has been defined
//          if (mmachine->getLanguage().size() != 0) {
//            sqlCommand.append("UPDATE description SET lang='"+mmachine->getLanguage()+"'"
//            " where machine_nummachineid='"+getAttribut("where machineid='"+mmachine->getWorkId()+"'")+"';");
//          }
//
//          //if a new machine description has been defined
//          if (mmachine->getWorkDescription().size() != 0) {
//            sqlCommand.append("UPDATE description SET description='"+mmachine->getWorkDescription()+"'"
//            " where machine_nummachineid='"+getAttribut("where machineid='"+mmachine->getWorkId()+"'")+"';");
//          }
//
//          //If there is a change
//          if (!sqlCommand.empty()) {
//            mdatabaseVishnu->process(sqlCommand.c_str());
//          }
//
//        } //End if the machine to update exists
//        else {
//          UMSVishnuException e (ERRCODE_UNKNOWN_MACHINE);
//          throw e;
//        }
//
//      } //End if the user is an admin
//      else {
//          UMSVishnuException e (ERRCODE_NO_ADMIN);
//          throw e;
//      }
//    }//End if the user exists
//    else {
//      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
//      throw e;
//    }
//    return 0;
//  } //END: update()
//
//  /**
//  * \brief Function to delete a VISHNU machine
//  * \fn int deleteWork()
//  * \return raises an exception on error
//  */
//  int
//  WorkServer::deleteWork() {
//
//    UserServer userServer = UserServer(msessionServer);
//    userServer.init();
//    //if the user exists
//    if (userServer.exist()) {
//      //if the user is an admin
//      if (userServer.isAdmin()) {
//        //if the machine to update exists
//        if (getAttribut("where machineid='"+mmachine->getWorkId()+"'").size() != 0) {
//          mdatabaseVishnu->process("DELETE FROM machine where machineid='"+mmachine->getWorkId()+"'");
//        } //End if the machine to update exists
//        else {
//          UMSVishnuException e (ERRCODE_UNKNOWN_MACHINE);
//          throw e;
//        }
//      } //End if the user is an admin
//      else {
//        UMSVishnuException e (ERRCODE_NO_ADMIN);
//        throw e;
//      }
//    }//End if the user exists
//    else {
//      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
//      throw e;
//    }
//    return 0;
//  } //END: deleteWork()
//
/**
* \fn ~WorkServer()
* \brief Destructor
*/
WorkServer::~WorkServer() {
}

/**
* \brief Function to get work information
* \return  The user data structure
*/
TMS_Data::Work*
WorkServer::getData() {
  return mwork;
}

/**
* \brief Function to get work information from the database vishnu
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \return the value of the attribut or empty string if no results
*/

std::string
WorkServer::getAttribut(std::string condition, std::string attrname) {

  std::string sqlCommand("SELECT "+attrname+" FROM work "+condition);
  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
  return result->getFirstElement();
}

/**
* \brief Function to get the work
* \return The name of the work
*/
std::string
WorkServer::getWorkName() {

  std::string  workName = getAttribut("where workid='"+getData()->getWorkId()+"'", "subject");

  return workName;
}

/**
* \brief Function to check the workId
* \return raises an exception
*/
void WorkServer::checkWork() {

  if(getAttribut("where workid='"+mwork->getWorkId()+"'").size()==0){
    throw TMSVishnuException(ERRCODE_UNKNOWN_WORKID, mwork->getWorkId()+" does not exist among the defined"
                             " work by VISHNU System");
  }
}

