/**
 * \file RequestFactory.cpp
 * \brief This file implements the virtual factory
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 04/06/13
 */
#include "RequestFactory.hpp"

RequestFactory::RequestFactory(){
  mrequest.insert (std::pair<int, std::string>(VR_COND_AUTHSYS_AND_STATUS, "WHERE authsystemid='%1%' AND status=%2%;"));
  mrequest.insert (std::pair<int, std::string>(VR_COND_AUTHSYSID, "WHERE authsystemid='%1%';"));
  mrequest.insert (std::pair<int, std::string>(VR_COND_USERID, "WHERE userid='%1%';"));
  mrequest.insert (std::pair<int, std::string>(VR_UPDATE_AUTHACCOUNT_LOGIN_WITH_AUTHSYSTEM_AND_USER, "UPDATE authaccount SET aclogin='%1%' WHERE authsystem_authsystemid=%2% AND users_numuserid=%3%;"));
  mrequest.insert (std::pair<int, std::string>(VR_INSERT_AUTHACCOUNT, "INSERT INTO authaccount (authsystem_authsystemid, users_numuserid, aclogin, status) VALUES ('%1%', '%2%', '%3%', %4%);"));
  mrequest.insert (std::pair<int, std::string>(VR_UPDATE_AUTHACCOUNT_STATUS_WITH_AUTHSYSTEM_AND_USER, "UPDATE authaccount SET status=%1% WHERE authsystem_authsystemid=%2% AND users_numuserid=%3%;"));
  mrequest.insert (std::pair<int, std::string>(VR_SELECT_AUTHACCOUNT, "SELECT %1% FROM authaccount %2%;"));
  mrequest.insert (std::pair<int, std::string>(VR_COND_AUTHSYS_USER_STATUS, "WHERE authsystem_authsystemid='%1%' AND users_numuserid=%2% AND status!=%3%;"));
  mrequest.insert (std::pair<int, std::string>(VR_COND_AUTHSYS_LOGIN_STATUS, "WHERE authsystem_authsystemid=%1% AND aclogin='%2%' AND status=%3%;"));


}

RequestFactory::~RequestFactory(){
}

