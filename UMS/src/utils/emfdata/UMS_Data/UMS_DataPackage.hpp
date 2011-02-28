// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/UMS_DataPackage.hpp
 * Copyright (C) Cátedra SAES-UMU 2010 <andres.senac@um.es>
 *
 * EMF4CPP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EMF4CPP is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * \file UMS_DataPackage.hpp
 * \brief Represents the package for the ums data. Internal class
 * \author Generated file
 * \date 31/01/2011
 */

#ifndef _UMS_DATAPACKAGE_HPP
#define _UMS_DATAPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <UMS_Data_forward.hpp>

namespace UMS_Data
{

    /**
     * \class UMS_DataPackage
     * \brief Package of the UMS data
     */
    class UMS_DataPackage: public virtual ::ecore::EPackage
    {
    public:

	/**
	 * \brief To get the instance
	 * \fn static UMS_DataPackage_ptr _instance()
	 * \return Pointer to the instance
	 */
        static UMS_DataPackage_ptr _instance();

        // IDs for classifiers
	/**
	 * \brief Constant for command
	 */
        static const int COMMAND = 0;

	/**
	 * \brief Constant for configuration
	 */
        static const int CONFIGURATION = 1;

	/**
	 * \brief Constant for connect option
	 */
        static const int CONNECTOPTIONS = 2;

	/**
	 * \brief Constant for list cmd options
	 */
        static const int LISTCMDOPTIONS = 3;

	/**
	 * \brief Constant for list commands
	 */
        static const int LISTCOMMANDS = 4;

	/**
	 * \brief Constant for list local account options
	 */
        static const int LISTLOCALACCOPTIONS = 5;

	/**
	 * \brief Constant for list local accounts
	 */
        static const int LISTLOCALACCOUNTS = 6;

	/**
	 * \brief Constant for list machine options
	 */
        static const int LISTMACHINEOPTIONS = 7;

	/**
	 * \brief Constant for list machines
	 */
        static const int LISTMACHINES = 8;

	/**
	 * \brief Constant for list option values
	 */
        static const int LISTOPTIONSVALUES = 9;

	/**
	 * \brief Constant for list opt options
	 */
        static const int LISTOPTOPTIONS = 10;

	/**
	 * \brief Constant for list session options
	 */
        static const int LISTSESSIONOPTIONS = 11;

	/**
	 * \brief Constant for list sessions
	 */
        static const int LISTSESSIONS = 12;

	/**
	 * \brief Constant for list users
	 */
        static const int LISTUSERS = 13;

	/**
	 * \brief Constant for local account
	 */
        static const int LOCALACCOUNT = 14;

	/**
	 * \brief Constant for machine
	 */
        static const int MACHINE = 15;

	/**
	 * \brief Constant for option value
	 */
        static const int OPTIONVALUE = 16;

	/**
	 * \brief Constant for privilege type
	 */
        static const int PRIVILEGETYPE = 17;

	/**
	 * \brief Constant for session
	 */
        static const int SESSION = 18;

	/**
	 * \brief Constant for session close type
	 */
        static const int SESSIONCLOSETYPE = 19;

	/**
	 * \brief Constant for status type
	 */
        static const int STATUSTYPE = 20;

	/**
	 * \brief Constant for user
	 */
        static const int USER = 21;

	/**
	 * \brief Constant for connect options close policy
	 */
        static const int CONNECTOPTIONS__CLOSEPOLICY = 0;

	/**
	 * \brief Constant for connect options session inactivity delay
	 */
        static const int CONNECTOPTIONS__SESSIONINACTIVITYDELAY = 1;

	/**
	 * \brief Constant for connect options substitue user id
	 */
        static const int CONNECTOPTIONS__SUBSTITUTEUSERID = 2;

	/**
	 * \brief Constant for session session id
	 */
        static const int SESSION__SESSIONID = 3;

	/**
	 * \brief Constant for session user id
	 */
        static const int SESSION__USERID = 4;

	/**
	 * \brief Constant for session key
	 */
        static const int SESSION__SESSIONKEY = 5;

	/**
	 * \brief Constant for session date last connect
	 */
        static const int SESSION__DATELASTCONNECT = 6;

	/**
	 * \brief Constant for session date creation
	 */
        static const int SESSION__DATECREATION = 7;

	/**
	 * \brief Constant for session date closure
	 */
        static const int SESSION__DATECLOSURE = 8;

	/**
	 * \brief Constant for session status
	 */
        static const int SESSION__STATUS = 9;

	/**
	 * \brief Constant for session close policy
	 */
        static const int SESSION__CLOSEPOLICY = 10;

	/**
	 * \brief Constant for session timeout
	 */
        static const int SESSION__TIMEOUT = 11;

	/**
	 * \brief Constant for list sessions session
	 */
        static const int LISTSESSIONS__SESSIONS = 12;

	/**
	 * \brief Constant for list session options status
	 */
        static const int LISTSESSIONOPTIONS__STATUS = 13;

	/**
	 * \brief Constant for list session options session close policy
	 */
        static const int LISTSESSIONOPTIONS__SESSIONCLOSEPOLICY = 14;

	/**
	 * \brief Constant for list session options session inactivity delay
	 */
        static const int LISTSESSIONOPTIONS__SESSIONINACTIVITYDELAY = 15;

	/**
	 * \brief Constant for list session options machine id
	 */
        static const int LISTSESSIONOPTIONS__MACHINEID = 16;

	/**
	 * \brief Constant for list session options admin list option
	 */
        static const int LISTSESSIONOPTIONS__ADMINLISTOPTION = 17;

	/**
	 * \brief Constant for list session options user id
	 */
        static const int LISTSESSIONOPTIONS__USERID = 18;

	/**
	 * \brief Constant for list session options session id
	 */
        static const int LISTSESSIONOPTIONS__SESSIONID = 19;

	/**
	 * \brief Constant for list session options start date
	 */
        static const int LISTSESSIONOPTIONS__STARTDATEOPTION = 20;

	/**
	 * \brief Constant for list session options end date
	 */
        static const int LISTSESSIONOPTIONS__ENDDATEOPTION = 21;

	/**
	 * \brief Constant for user user id
	 */
        static const int USER__USERID = 22;

	/**
	 * \brief Constant for user password
	 */
        static const int USER__PASSWORD = 23;

	/**
	 * \brief Constant for user firstname
	 */
        static const int USER__FIRSTNAME = 24;

	/**
	 * \brief Constant for user lastname
	 */
        static const int USER__LASTNAME = 25;

	/**
	 * \brief Constant for user privilege
	 */
        static const int USER__PRIVILEGE = 26;

	/**
	 * \brief Constant for user email
	 */
        static const int USER__EMAIL = 27;

	/**
	 * \brief Constant for user status
	 */
        static const int USER__STATUS = 28;

	/**
	 * \brief Constant for list user users
	 */
        static const int LISTUSERS__USERS = 29;

	/**
	 * \brief Constant for command command id
	 */
        static const int COMMAND__COMMANDID = 30;

	/**
	 * \brief Constant for command session id
	 */
        static const int COMMAND__SESSIONID = 31;

	/**
	 * \brief Constant for command machine id
	 */
        static const int COMMAND__MACHINEID = 32;

	/**
	 * \brief Constant for command command description
	 */
        static const int COMMAND__CMDDESCRIPTION = 33;

	/**
	 * \brief Constant for command start time
	 */
        static const int COMMAND__CMDSTARTTIME = 34;

	/**
	 * \brief Constant for command end time
	 */
        static const int COMMAND__CMDENDTIME = 35;

	/**
	 * \brief Constant for list command commands
	 */
        static const int LISTCOMMANDS__COMMANDS = 36;

	/**
	 * \brief Constant for list command options admin list option
	 */
        static const int LISTCMDOPTIONS__ADMINLISTOPTION = 37;

	/**
	 * \brief Constant for list command options user id
	 */
        static const int LISTCMDOPTIONS__USERID = 38;

	/**
	 * \brief Constant for list command option session id
	 */
        static const int LISTCMDOPTIONS__SESSIONID = 39;

	/**
	 * \brief Constant for list command options start date
	 */
        static const int LISTCMDOPTIONS__STARTDATEOPTION = 40;

	/**
	 * \brief Constant for list command options end date
	 */
        static const int LISTCMDOPTIONS__ENDDATEOPTION = 41;

	/**
	 * \brief Constant for local account user id
	 */
        static const int LOCALACCOUNT__USERID = 42;

	/**
	 * \brief Constant for local account machine id
	 */
        static const int LOCALACCOUNT__MACHINEID = 43;

	/**
	 * \brief Constant for local account account login
	 */
        static const int LOCALACCOUNT__ACLOGIN = 44;

	/**
	 * \brief Constant for local account ssh key path
	 */
        static const int LOCALACCOUNT__SSHKEYPATH = 45;

	/**
	 * \brief Constant for local account hom directory
	 */
        static const int LOCALACCOUNT__HOMEDIRECTORY = 46;

	/**
	 * \brief Constant for list local accounts accounts
	 */
        static const int LISTLOCALACCOUNTS__ACCOUNTS = 47;

	/**
	 * \brief Constant for list local accounts admin list option
	 */
        static const int LISTLOCALACCOPTIONS__ADMINLISTOPTION = 48;

	/**
	 * \brief Constant for list local account options user id
	 */
        static const int LISTLOCALACCOPTIONS__USERID = 49;

	/**
	 * \brief Constant for list local account options machine id
	 */
        static const int LISTLOCALACCOPTIONS__MACHINEID = 50;

	/**
	 * \brief Constant for machine machine id
	 */
        static const int MACHINE__MACHINEID = 51;

	/**
	 * \brief Constant for machine name
	 */
        static const int MACHINE__NAME = 52;

	/**
	 * \brief Constant for machine site
	 */
        static const int MACHINE__SITE = 53;

	/**
	 * \brief Constant for machine machine descritpion
	 */
        static const int MACHINE__MACHINEDESCRIPTION = 54;

	/**
	 * \brief Constant for machine language
	 */
        static const int MACHINE__LANGUAGE = 55;

	/**
	 * \brief Constant for machine status
	 */
        static const int MACHINE__STATUS = 56;

	/**
	 * \brief Constant for machine ssh public key
	 */
        static const int MACHINE__SSHPUBLICKEY = 57;

	/**
	 * \brief Constant for list machine machines
	 */
        static const int LISTMACHINES__MACHINES = 58;

	/**
	 * \brief Constant for list machine option user id
	 */
        static const int LISTMACHINEOPTIONS__USERID = 59;

	/**
	 * \brief Constant for list machine option list all
	 */
        static const int LISTMACHINEOPTIONS__LISTALLMACHINE = 60;

	/**
	 * \brief Constant for list machine option machine id
	 */
        static const int LISTMACHINEOPTIONS__MACHINEID = 61;

	/**
	 * \brief Constant for configuration list configuration user
	 */
        static const int CONFIGURATION__LISTCONFUSERS = 62;

	/**
	 * \brief Constant for configuration list configuration machines
	 */
        static const int CONFIGURATION__LISTCONFMACHINES = 63;

	/**
	 * \brief Constant for configuration list configuration local account
	 */
        static const int CONFIGURATION__LISTCONFLOCALACCOUNTS = 64;

	/**
	 * \brief Constant for option value option name
	 */
        static const int OPTIONVALUE__OPTIONNAME = 65;

	/**
	 * \brief Constant for option value value
	 */
        static const int OPTIONVALUE__VALUE = 66;

	/**
	 * \brief Constant for list option values option values
	 */
        static const int LISTOPTIONSVALUES__OPTIONVALUES = 67;

	/**
	 * \brief Constant for list options list all defined value
	 */
        static const int LISTOPTOPTIONS__LISTALLDEFTVALUE = 68;

	/**
	 * \brief Constant for list opt options user id
	 */
        static const int LISTOPTOPTIONS__USERID = 69;

	/**
	 * \brief Constant for list option options option name
	 */
        static const int LISTOPTOPTIONS__OPTIONNAME = 70;

        // IDs for classifiers for class Command 


        // IDs for classifiers for class Configuration 


        // IDs for classifiers for class ConnectOptions 


        // IDs for classifiers for class ListCmdOptions 


        // IDs for classifiers for class ListCommands 


        // IDs for classifiers for class ListLocalAccOptions 


        // IDs for classifiers for class ListLocalAccounts 


        // IDs for classifiers for class ListMachineOptions 


        // IDs for classifiers for class ListMachines 


        // IDs for classifiers for class ListOptionsValues 


        // IDs for classifiers for class ListOptOptions 


        // IDs for classifiers for class ListSessionOptions 


        // IDs for classifiers for class ListSessions 


        // IDs for classifiers for class ListUsers 


        // IDs for classifiers for class LocalAccount 


        // IDs for classifiers for class Machine 


        // IDs for classifiers for class OptionValue 


        // IDs for classifiers for class Session 


        // IDs for classifiers for class User 


        // EClassifiers methods
	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getConnectOptions()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getConnectOptions();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getSession()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getSession();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListSessions()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListSessions();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListSessionOptions()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListSessionOptions();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getUser()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getUser();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListUsers()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListUsers();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getCommand()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getCommand();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListCommands()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListCommands();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListCmdOptions()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListCmdOptions();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getLocalAccount()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getLocalAccount();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListLocalAccounts()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListLocalAccounts();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListLocalAccOptions()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListLocalAccOptions();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getMachine()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getMachine();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListMachines()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListMachines();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListMachineOptions()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListMachineOptions();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getConfiguration()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getConfiguration();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getOptionValue()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getOptionValue();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListOptionsValues()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListOptionsValues();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EClass_ptr getListOptOptions()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EClass_ptr getListOptOptions();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EEnum_ptr getSessionCloseType()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EEnum_ptr getSessionCloseType();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EEnum_ptr getPrivilegeType()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EEnum_ptr getPrivilegeType();

	/**
	 * \brief To get an object of the class
	 * \fn ::ecore::EEnum_ptr getStatusType()
	 * \return To get an object of the class
	 */
        virtual ::ecore::EEnum_ptr getStatusType();

        // EStructuralFeatures methods

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getConnectOptions__closePolicy()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getConnectOptions__closePolicy();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getConnectOptions__sessionInactivityDelay()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr
                getConnectOptions__sessionInactivityDelay();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getConnectOptions__substituteUserId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getConnectOptions__substituteUserId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getSession__sessionId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getSession__sessionId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getSession__userId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getSession__userId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getSession__sessionKey()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getSession__sessionKey();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getSession__dateLastConnect()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getSession__dateLastConnect();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getSession__dateCreation()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getSession__dateCreation();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getSession__dateClosure()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getSession__dateClosure();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getSession__status()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getSession__status();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getSession__closePolicy()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getSession__closePolicy();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getSession__timeout()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getSession__timeout();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListSessions__sessions()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EReference_ptr getListSessions__sessions();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListSessionOptions__status()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListSessionOptions__status();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListSessionOptions__sessionClosePolicy()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__sessionClosePolicy();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListSessionOptions__sessionInactivityDelay()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__sessionInactivityDelay();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListSessionOptions__machineId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListSessionOptions__machineId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListSessionOptions__adminListOption()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__adminListOption();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListSessionOptions__userId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListSessionOptions__userId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListSessionOptions__sessionId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListSessionOptions__sessionId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListSessionOptions__startDateOption()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__startDateOption();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListSessionOptions__endDateOption()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListSessionOptions__endDateOption();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getUser__userId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getUser__userId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getUser__password()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getUser__password();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getUser__firstname()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getUser__firstname();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getUser__lastname()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getUser__lastname();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getUser__privilege()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getUser__privilege();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getUser__email()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getUser__email();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getUser__status()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getUser__status();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EReference_ptr getListUsers__users()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EReference_ptr getListUsers__users();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getCommand__commandId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getCommand__commandId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getCommand__sessionId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getCommand__sessionId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getCommand__machineId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getCommand__machineId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getCommand__cmdDescription()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getCommand__cmdDescription();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getCommand__cmdStartTime()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getCommand__cmdStartTime();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getCommand__cmdEndTime()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getCommand__cmdEndTime();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EReference_ptr getListCommands__Commands()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EReference_ptr getListCommands__Commands();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListCmdOptions__adminListOption()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListCmdOptions__adminListOption();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListCmdOptions__userId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListCmdOptions__userId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListCmdOptions__sessionId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListCmdOptions__sessionId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListCmdOptions__startDateOption()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListCmdOptions__startDateOption();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListCmdOptions__endDateOption()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListCmdOptions__endDateOption();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getLocalAccount__userId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getLocalAccount__userId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getLocalAccount__machineId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getLocalAccount__machineId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getLocalAccount__acLogin()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getLocalAccount__acLogin();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getLocalAccount__sshKeyPath()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getLocalAccount__sshKeyPath();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getLocalAccount__homeDirectory()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getLocalAccount__homeDirectory();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EReference_ptr getListLocalAccounts__accounts()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EReference_ptr getListLocalAccounts__accounts();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListLocalAccOptions__adminListOption()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr
                getListLocalAccOptions__adminListOption();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListLocalAccOptions__userId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListLocalAccOptions__userId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListLocalAccOptions__machineId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListLocalAccOptions__machineId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getMachine__machineId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getMachine__machineId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getMachine__name()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getMachine__name();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getMachine__site()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getMachine__site();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getMachine__machineDescription()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getMachine__machineDescription();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getMachine__language()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getMachine__language();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getMachine__status()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getMachine__status();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getMachine__sshPublicKey()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getMachine__sshPublicKey();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EReference_ptr getListMachines__machines()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EReference_ptr getListMachines__machines();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListMachineOptions__userId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListMachineOptions__userId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListMachineOptions__listAllmachine()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListMachineOptions__listAllmachine();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListMachineOptions__machineId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListMachineOptions__machineId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EReference_ptr getConfiguration__listConfUsers()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EReference_ptr getConfiguration__listConfUsers();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EReference_ptr getConfiguration__listConfMachines()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EReference_ptr getConfiguration__listConfMachines();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EReference_ptr getConfiguration__listConfLocalAccounts()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EReference_ptr
                getConfiguration__listConfLocalAccounts();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getOptionValue__optionName()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getOptionValue__optionName();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getOptionValue__value()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getOptionValue__value();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EReference_ptr getListOptionsValues__optionValues()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EReference_ptr getListOptionsValues__optionValues();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListOptOptions__listAllDeftValue()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListOptOptions__listAllDeftValue();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListOptOptions__userId()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListOptOptions__userId();

	/**
	 * \brief To get an attribute of a class
	 * \fn ::ecore::EAttribute_ptr getListOptOptions__optionName()()
	 * \return To get an attribute of a class
	 */
        virtual ::ecore::EAttribute_ptr getListOptOptions__optionName();

    protected:

	/**
	 * \brief The instance
	 */
        static std::auto_ptr< UMS_DataPackage > s_instance;

        UMS_DataPackage();

        // EClass instances 

	/**
	 * \brief The connect option class
	 */
        ::ecore::EClass_ptr m_ConnectOptionsEClass;

	/**
	 * \brief The session class
	 */
        ::ecore::EClass_ptr m_SessionEClass;

	/**
	 * \brief The list session class
	 */
        ::ecore::EClass_ptr m_ListSessionsEClass;

	/**
	 * \brief The list session option class
	 */
        ::ecore::EClass_ptr m_ListSessionOptionsEClass;

	/**
	 * \brief The user class
	 */
        ::ecore::EClass_ptr m_UserEClass;

	/**
	 * \brief The list user class
	 */
        ::ecore::EClass_ptr m_ListUsersEClass;

	/**
	 * \brief The command class
	 */
        ::ecore::EClass_ptr m_CommandEClass;

	/**
	 * \brief The list command class
	 */
        ::ecore::EClass_ptr m_ListCommandsEClass;

	/**
	 * \brief The list command option class
	 */
        ::ecore::EClass_ptr m_ListCmdOptionsEClass;

	/**
	 * \brief The local account class
	 */
        ::ecore::EClass_ptr m_LocalAccountEClass;

	/**
	 * \brief The list local account class
	 */
        ::ecore::EClass_ptr m_ListLocalAccountsEClass;

	/**
	 * \brief The list local account option class
	 */
        ::ecore::EClass_ptr m_ListLocalAccOptionsEClass;

	/**
	 * \brief The machine class
	 */
        ::ecore::EClass_ptr m_MachineEClass;

	/**
	 * \brief The list machine class
	 */
        ::ecore::EClass_ptr m_ListMachinesEClass;

	/**
	 * \brief The list machine option class
	 */
        ::ecore::EClass_ptr m_ListMachineOptionsEClass;

	/**
	 * \brief The configuration class
	 */
        ::ecore::EClass_ptr m_ConfigurationEClass;

	/**
	 * \brief The option value class
	 */
        ::ecore::EClass_ptr m_OptionValueEClass;

	/**
	 * \brief The list option value class
	 */
        ::ecore::EClass_ptr m_ListOptionsValuesEClass;

	/**
	 * \brief The list option option class
	 */
        ::ecore::EClass_ptr m_ListOptOptionsEClass;

        // EEnuminstances 

	/**
	 * \brief The session close type enum
	 */
        ::ecore::EEnum_ptr m_SessionCloseTypeEEnum;

	/**
	 * \brief The privilege type enum
	 */
        ::ecore::EEnum_ptr m_PrivilegeTypeEEnum;

	/**
	 * \brief The session status type enum
	 */
        ::ecore::EEnum_ptr m_StatusTypeEEnum;

        // EDataType instances 


        // EStructuralFeatures instances

	/**
	 * \brief The connect option close policy attribute
	 */
        ::ecore::EAttribute_ptr m_ConnectOptions__closePolicy;

	/**
	 * \brief The connect option session inactivity attribute
	 */
        ::ecore::EAttribute_ptr m_ConnectOptions__sessionInactivityDelay;

	/**
	 * \brief The connect option sustitute user id attribute
	 */
        ::ecore::EAttribute_ptr m_ConnectOptions__substituteUserId;

	/**
	 * \brief The session sessionid attribute
	 */
        ::ecore::EAttribute_ptr m_Session__sessionId;

	/**
	 * \brief The session user id attribute
	 */
        ::ecore::EAttribute_ptr m_Session__userId;

	/**
	 * \brief The session session key attribute
	 */
        ::ecore::EAttribute_ptr m_Session__sessionKey;

	/**
	 * \brief The session last connect attribute
	 */
        ::ecore::EAttribute_ptr m_Session__dateLastConnect;

	/**
	 * \brief The session creation attribute
	 */
        ::ecore::EAttribute_ptr m_Session__dateCreation;

	/**
	 * \brief The session closure date attribute
	 */
        ::ecore::EAttribute_ptr m_Session__dateClosure;

	/**
	 * \brief The session status attribute
	 */
        ::ecore::EAttribute_ptr m_Session__status;

	/**
	 * \brief The session close policy attribute
	 */
        ::ecore::EAttribute_ptr m_Session__closePolicy;

	/**
	 * \brief The session timeout attribute
	 */
        ::ecore::EAttribute_ptr m_Session__timeout;

	/**
	 * \brief The list session session attribute
	 */
        ::ecore::EReference_ptr m_ListSessions__sessions;

	/**
	 * \brief The list session option status attribute
	 */
        ::ecore::EAttribute_ptr m_ListSessionOptions__status;

	/**
	 * \brief The list session options close policy attribute
	 */
        ::ecore::EAttribute_ptr m_ListSessionOptions__sessionClosePolicy;

	/**
	 * \brief The list session option inactivity delay attribute
	 */
        ::ecore::EAttribute_ptr m_ListSessionOptions__sessionInactivityDelay;

	/**
	 * \brief The list session option machine id attribute
	 */
        ::ecore::EAttribute_ptr m_ListSessionOptions__machineId;

	/**
	 * \brief The connect list session option admin list option attribute
	 */
        ::ecore::EAttribute_ptr m_ListSessionOptions__adminListOption;

	/**
	 * \brief The list session option user id attribute
	 */
        ::ecore::EAttribute_ptr m_ListSessionOptions__userId;

	/**
	 * \brief The list session option session id attribute
	 */
        ::ecore::EAttribute_ptr m_ListSessionOptions__sessionId;

	/**
	 * \brief The list session option start date attribute
	 */
        ::ecore::EAttribute_ptr m_ListSessionOptions__startDateOption;

	/**
	 * \brief The list session option end date attribute
	 */
        ::ecore::EAttribute_ptr m_ListSessionOptions__endDateOption;

	/**
	 * \brief The user user id attribute
	 */
        ::ecore::EAttribute_ptr m_User__userId;

	/**
	 * \brief The connect user password attribute
	 */
        ::ecore::EAttribute_ptr m_User__password;

	/**
	 * \brief The user firstname attribute
	 */
        ::ecore::EAttribute_ptr m_User__firstname;

	/**
	 * \brief The user lastname attribute
	 */
        ::ecore::EAttribute_ptr m_User__lastname;

	/**
	 * \brief The user privilege attribute
	 */
        ::ecore::EAttribute_ptr m_User__privilege;

	/**
	 * \brief The user email attribute
	 */
        ::ecore::EAttribute_ptr m_User__email;

	/**
	 * \brief The user status attribute
	 */
        ::ecore::EAttribute_ptr m_User__status;

	/**
	 * \brief The list user user attribute
	 */
        ::ecore::EReference_ptr m_ListUsers__users;

	/**
	 * \brief The command command id attribute
	 */
        ::ecore::EAttribute_ptr m_Command__commandId;

	/**
	 * \brief The command session id attribute
	 */
        ::ecore::EAttribute_ptr m_Command__sessionId;

	/**
	 * \brief The command macine id attribute
	 */
        ::ecore::EAttribute_ptr m_Command__machineId;

	/**
	 * \brief The command command description attribute
	 */
        ::ecore::EAttribute_ptr m_Command__cmdDescription;

	/**
	 * \brief The command start attribute
	 */
        ::ecore::EAttribute_ptr m_Command__cmdStartTime;

	/**
	 * \brief The command end attribute
	 */
        ::ecore::EAttribute_ptr m_Command__cmdEndTime;

	/**
	 * \brief The list command commands attribute
	 */
        ::ecore::EReference_ptr m_ListCommands__Commands;

	/**
	 * \brief The list command option admin list option attribute
	 */
        ::ecore::EAttribute_ptr m_ListCmdOptions__adminListOption;

	/**
	 * \brief The list command options user id attribute
	 */
        ::ecore::EAttribute_ptr m_ListCmdOptions__userId;

	/**
	 * \brief The list command options session id attribute
	 */
        ::ecore::EAttribute_ptr m_ListCmdOptions__sessionId;

	/**
	 * \brief The list command options start attribute
	 */
        ::ecore::EAttribute_ptr m_ListCmdOptions__startDateOption;

	/**
	 * \brief The list command option end attribute
	 */
        ::ecore::EAttribute_ptr m_ListCmdOptions__endDateOption;

	/**
	 * \brief The local account user id attribute
	 */
        ::ecore::EAttribute_ptr m_LocalAccount__userId;

	/**
	 * \brief The local account machine id attribute
	 */
        ::ecore::EAttribute_ptr m_LocalAccount__machineId;

	/**
	 * \brief The local account account login attribute
	 */
        ::ecore::EAttribute_ptr m_LocalAccount__acLogin;

	/**
	 * \brief The local account ssh key path attribute
	 */
        ::ecore::EAttribute_ptr m_LocalAccount__sshKeyPath;

	/**
	 * \brief The local account home attribute
	 */
        ::ecore::EAttribute_ptr m_LocalAccount__homeDirectory;

	/**
	 * \brief The list local accounts account attribute
	 */
        ::ecore::EReference_ptr m_ListLocalAccounts__accounts;

	/**
	 * \brief The list local account option admin list option attribute
	 */
        ::ecore::EAttribute_ptr m_ListLocalAccOptions__adminListOption;

	/**
	 * \brief The list local account option user id attribute
	 */
        ::ecore::EAttribute_ptr m_ListLocalAccOptions__userId;

	/**
	 * \brief The list local account option machine id attribute
	 */
        ::ecore::EAttribute_ptr m_ListLocalAccOptions__machineId;

	/**
	 * \brief The machine machine id attribute
	 */
        ::ecore::EAttribute_ptr m_Machine__machineId;

	/**
	 * \brief The machine name attribute
	 */
        ::ecore::EAttribute_ptr m_Machine__name;

	/**
	 * \brief The machine site attribute
	 */
        ::ecore::EAttribute_ptr m_Machine__site;

	/**
	 * \brief The machine description attribute
	 */
        ::ecore::EAttribute_ptr m_Machine__machineDescription;

	/**
	 * \brief The machine language attribute
	 */
        ::ecore::EAttribute_ptr m_Machine__language;

	/**
	 * \brief The machine status attribute
	 */
        ::ecore::EAttribute_ptr m_Machine__status;

	/**
	 * \brief The machine ssh public key attribute
	 */
        ::ecore::EAttribute_ptr m_Machine__sshPublicKey;

	/**
	 * \brief The list machine machine attribute
	 */
        ::ecore::EReference_ptr m_ListMachines__machines;

	/**
	 * \brief The list machine option user id attribute
	 */
        ::ecore::EAttribute_ptr m_ListMachineOptions__userId;

	/**
	 * \brief The list machine optionlist all machine attribute
	 */
        ::ecore::EAttribute_ptr m_ListMachineOptions__listAllmachine;

	/**
	 * \brief The list machine option machine id attribute
	 */
        ::ecore::EAttribute_ptr m_ListMachineOptions__machineId;

	/**
	 * \brief The configuration list configuration user attribute
	 */
        ::ecore::EReference_ptr m_Configuration__listConfUsers;

	/**
	 * \brief The configuration list configuration machine attribute
	 */
        ::ecore::EReference_ptr m_Configuration__listConfMachines;

	/**
	 * \brief The configuration list configuration local account attribute
	 */
        ::ecore::EReference_ptr m_Configuration__listConfLocalAccounts;

	/**
	 * \brief The option value option name attribute
	 */
        ::ecore::EAttribute_ptr m_OptionValue__optionName;

	/**
	 * \brief The option value value attribute
	 */
        ::ecore::EAttribute_ptr m_OptionValue__value;

	/**
	 * \brief The list option value option value attribute
	 */
        ::ecore::EReference_ptr m_ListOptionsValues__optionValues;

	/**
	 * \brief The list option option list all defined value attribute
	 */
        ::ecore::EAttribute_ptr m_ListOptOptions__listAllDeftValue;

	/**
	 * \brief The list option option user id attribute
	 */
        ::ecore::EAttribute_ptr m_ListOptOptions__userId;

	/**
	 * \brief The list option option option name attribute
	 */
        ::ecore::EAttribute_ptr m_ListOptOptions__optionName;

    };

} // UMS_Data


#endif // _UMS_DATAPACKAGE_HPP
