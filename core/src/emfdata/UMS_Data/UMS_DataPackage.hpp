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
 * \brief The UMS_DataPackage class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef _UMS_DATAPACKAGE_HPP
#define _UMS_DATAPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <UMS_Data_forward.hpp>

namespace UMS_Data
{
    /**
     * \class UMS_DataPackage
     * \brief The package class (reflective API) for generated model UMS_Data
     */
    class UMS_DataPackage: public virtual ::ecore::EPackage
    {
    public:

        /**
         * \brief To get the instance of the singleton
         * \return Pointer to the instance
         */
        static UMS_DataPackage_ptr _instance();

        // IDs for classifiers

        /**
         * \brief Constant for Command class
         */
        static const int COMMAND = 0;

        /**
         * \brief Constant for CommandStatusType class
         */
        static const int COMMANDSTATUSTYPE = 1;

        /**
         * \brief Constant for Configuration class
         */
        static const int CONFIGURATION = 2;

        /**
         * \brief Constant for ConnectOptions class
         */
        static const int CONNECTOPTIONS = 3;

        /**
         * \brief Constant for ListCmdOptions class
         */
        static const int LISTCMDOPTIONS = 4;

        /**
         * \brief Constant for ListCommands class
         */
        static const int LISTCOMMANDS = 5;

        /**
         * \brief Constant for ListLocalAccOptions class
         */
        static const int LISTLOCALACCOPTIONS = 6;

        /**
         * \brief Constant for ListLocalAccounts class
         */
        static const int LISTLOCALACCOUNTS = 7;

        /**
         * \brief Constant for ListMachineOptions class
         */
        static const int LISTMACHINEOPTIONS = 8;

        /**
         * \brief Constant for ListMachines class
         */
        static const int LISTMACHINES = 9;

        /**
         * \brief Constant for ListOptionsValues class
         */
        static const int LISTOPTIONSVALUES = 10;

        /**
         * \brief Constant for ListOptOptions class
         */
        static const int LISTOPTOPTIONS = 11;

        /**
         * \brief Constant for ListSessionOptions class
         */
        static const int LISTSESSIONOPTIONS = 12;

        /**
         * \brief Constant for ListSessions class
         */
        static const int LISTSESSIONS = 13;

        /**
         * \brief Constant for ListUsers class
         */
        static const int LISTUSERS = 14;

        /**
         * \brief Constant for LocalAccount class
         */
        static const int LOCALACCOUNT = 15;

        /**
         * \brief Constant for Machine class
         */
        static const int MACHINE = 16;

        /**
         * \brief Constant for OptionValue class
         */
        static const int OPTIONVALUE = 17;

        /**
         * \brief Constant for PrivilegeType class
         */
        static const int PRIVILEGETYPE = 18;

        /**
         * \brief Constant for Session class
         */
        static const int SESSION = 19;

        /**
         * \brief Constant for SessionCloseType class
         */
        static const int SESSIONCLOSETYPE = 20;

        /**
         * \brief Constant for StatusType class
         */
        static const int STATUSTYPE = 21;

        /**
         * \brief Constant for User class
         */
        static const int USER = 22;

        /**
         * \brief Constant for CONNECTOPTIONS__CLOSEPOLICY feature
         */
        static const int CONNECTOPTIONS__CLOSEPOLICY = 0;

        /**
         * \brief Constant for CONNECTOPTIONS__SESSIONINACTIVITYDELAY feature
         */
        static const int CONNECTOPTIONS__SESSIONINACTIVITYDELAY = 1;

        /**
         * \brief Constant for CONNECTOPTIONS__SUBSTITUTEUSERID feature
         */
        static const int CONNECTOPTIONS__SUBSTITUTEUSERID = 2;

        /**
         * \brief Constant for SESSION__SESSIONID feature
         */
        static const int SESSION__SESSIONID = 3;

        /**
         * \brief Constant for SESSION__USERID feature
         */
        static const int SESSION__USERID = 4;

        /**
         * \brief Constant for SESSION__SESSIONKEY feature
         */
        static const int SESSION__SESSIONKEY = 5;

        /**
         * \brief Constant for SESSION__DATELASTCONNECT feature
         */
        static const int SESSION__DATELASTCONNECT = 6;

        /**
         * \brief Constant for SESSION__DATECREATION feature
         */
        static const int SESSION__DATECREATION = 7;

        /**
         * \brief Constant for SESSION__DATECLOSURE feature
         */
        static const int SESSION__DATECLOSURE = 8;

        /**
         * \brief Constant for SESSION__STATUS feature
         */
        static const int SESSION__STATUS = 9;

        /**
         * \brief Constant for SESSION__CLOSEPOLICY feature
         */
        static const int SESSION__CLOSEPOLICY = 10;

        /**
         * \brief Constant for SESSION__TIMEOUT feature
         */
        static const int SESSION__TIMEOUT = 11;

        /**
         * \brief Constant for LISTSESSIONS__SESSIONS feature
         */
        static const int LISTSESSIONS__SESSIONS = 12;

        /**
         * \brief Constant for LISTSESSIONOPTIONS__STATUS feature
         */
        static const int LISTSESSIONOPTIONS__STATUS = 13;

        /**
         * \brief Constant for LISTSESSIONOPTIONS__SESSIONCLOSEPOLICY feature
         */
        static const int LISTSESSIONOPTIONS__SESSIONCLOSEPOLICY = 14;

        /**
         * \brief Constant for LISTSESSIONOPTIONS__SESSIONINACTIVITYDELAY feature
         */
        static const int LISTSESSIONOPTIONS__SESSIONINACTIVITYDELAY = 15;

        /**
         * \brief Constant for LISTSESSIONOPTIONS__MACHINEID feature
         */
        static const int LISTSESSIONOPTIONS__MACHINEID = 16;

        /**
         * \brief Constant for LISTSESSIONOPTIONS__ADMINLISTOPTION feature
         */
        static const int LISTSESSIONOPTIONS__ADMINLISTOPTION = 17;

        /**
         * \brief Constant for LISTSESSIONOPTIONS__USERID feature
         */
        static const int LISTSESSIONOPTIONS__USERID = 18;

        /**
         * \brief Constant for LISTSESSIONOPTIONS__SESSIONID feature
         */
        static const int LISTSESSIONOPTIONS__SESSIONID = 19;

        /**
         * \brief Constant for LISTSESSIONOPTIONS__STARTDATEOPTION feature
         */
        static const int LISTSESSIONOPTIONS__STARTDATEOPTION = 20;

        /**
         * \brief Constant for LISTSESSIONOPTIONS__ENDDATEOPTION feature
         */
        static const int LISTSESSIONOPTIONS__ENDDATEOPTION = 21;

        /**
         * \brief Constant for USER__USERID feature
         */
        static const int USER__USERID = 22;

        /**
         * \brief Constant for USER__PASSWORD feature
         */
        static const int USER__PASSWORD = 23;

        /**
         * \brief Constant for USER__FIRSTNAME feature
         */
        static const int USER__FIRSTNAME = 24;

        /**
         * \brief Constant for USER__LASTNAME feature
         */
        static const int USER__LASTNAME = 25;

        /**
         * \brief Constant for USER__PRIVILEGE feature
         */
        static const int USER__PRIVILEGE = 26;

        /**
         * \brief Constant for USER__EMAIL feature
         */
        static const int USER__EMAIL = 27;

        /**
         * \brief Constant for USER__STATUS feature
         */
        static const int USER__STATUS = 28;

        /**
         * \brief Constant for LISTUSERS__USERS feature
         */
        static const int LISTUSERS__USERS = 29;

        /**
         * \brief Constant for COMMAND__COMMANDID feature
         */
        static const int COMMAND__COMMANDID = 30;

        /**
         * \brief Constant for COMMAND__SESSIONID feature
         */
        static const int COMMAND__SESSIONID = 31;

        /**
         * \brief Constant for COMMAND__MACHINEID feature
         */
        static const int COMMAND__MACHINEID = 32;

        /**
         * \brief Constant for COMMAND__CMDDESCRIPTION feature
         */
        static const int COMMAND__CMDDESCRIPTION = 33;

        /**
         * \brief Constant for COMMAND__CMDSTARTTIME feature
         */
        static const int COMMAND__CMDSTARTTIME = 34;

        /**
         * \brief Constant for COMMAND__CMDENDTIME feature
         */
        static const int COMMAND__CMDENDTIME = 35;

        /**
         * \brief Constant for COMMAND__STATUS feature
         */
        static const int COMMAND__STATUS = 36;

        /**
         * \brief Constant for LISTCOMMANDS__COMMANDS feature
         */
        static const int LISTCOMMANDS__COMMANDS = 37;

        /**
         * \brief Constant for LISTCMDOPTIONS__ADMINLISTOPTION feature
         */
        static const int LISTCMDOPTIONS__ADMINLISTOPTION = 38;

        /**
         * \brief Constant for LISTCMDOPTIONS__USERID feature
         */
        static const int LISTCMDOPTIONS__USERID = 39;

        /**
         * \brief Constant for LISTCMDOPTIONS__SESSIONID feature
         */
        static const int LISTCMDOPTIONS__SESSIONID = 40;

        /**
         * \brief Constant for LISTCMDOPTIONS__STARTDATEOPTION feature
         */
        static const int LISTCMDOPTIONS__STARTDATEOPTION = 41;

        /**
         * \brief Constant for LISTCMDOPTIONS__ENDDATEOPTION feature
         */
        static const int LISTCMDOPTIONS__ENDDATEOPTION = 42;

        /**
         * \brief Constant for LOCALACCOUNT__USERID feature
         */
        static const int LOCALACCOUNT__USERID = 43;

        /**
         * \brief Constant for LOCALACCOUNT__MACHINEID feature
         */
        static const int LOCALACCOUNT__MACHINEID = 44;

        /**
         * \brief Constant for LOCALACCOUNT__ACLOGIN feature
         */
        static const int LOCALACCOUNT__ACLOGIN = 45;

        /**
         * \brief Constant for LOCALACCOUNT__SSHKEYPATH feature
         */
        static const int LOCALACCOUNT__SSHKEYPATH = 46;

        /**
         * \brief Constant for LOCALACCOUNT__HOMEDIRECTORY feature
         */
        static const int LOCALACCOUNT__HOMEDIRECTORY = 47;

        /**
         * \brief Constant for LISTLOCALACCOUNTS__ACCOUNTS feature
         */
        static const int LISTLOCALACCOUNTS__ACCOUNTS = 48;

        /**
         * \brief Constant for LISTLOCALACCOPTIONS__ADMINLISTOPTION feature
         */
        static const int LISTLOCALACCOPTIONS__ADMINLISTOPTION = 49;

        /**
         * \brief Constant for LISTLOCALACCOPTIONS__USERID feature
         */
        static const int LISTLOCALACCOPTIONS__USERID = 50;

        /**
         * \brief Constant for LISTLOCALACCOPTIONS__MACHINEID feature
         */
        static const int LISTLOCALACCOPTIONS__MACHINEID = 51;

        /**
         * \brief Constant for MACHINE__MACHINEID feature
         */
        static const int MACHINE__MACHINEID = 52;

        /**
         * \brief Constant for MACHINE__NAME feature
         */
        static const int MACHINE__NAME = 53;

        /**
         * \brief Constant for MACHINE__SITE feature
         */
        static const int MACHINE__SITE = 54;

        /**
         * \brief Constant for MACHINE__MACHINEDESCRIPTION feature
         */
        static const int MACHINE__MACHINEDESCRIPTION = 55;

        /**
         * \brief Constant for MACHINE__LANGUAGE feature
         */
        static const int MACHINE__LANGUAGE = 56;

        /**
         * \brief Constant for MACHINE__STATUS feature
         */
        static const int MACHINE__STATUS = 57;

        /**
         * \brief Constant for MACHINE__SSHPUBLICKEY feature
         */
        static const int MACHINE__SSHPUBLICKEY = 58;

        /**
         * \brief Constant for LISTMACHINES__MACHINES feature
         */
        static const int LISTMACHINES__MACHINES = 59;

        /**
         * \brief Constant for LISTMACHINEOPTIONS__USERID feature
         */
        static const int LISTMACHINEOPTIONS__USERID = 60;

        /**
         * \brief Constant for LISTMACHINEOPTIONS__LISTALLMACHINE feature
         */
        static const int LISTMACHINEOPTIONS__LISTALLMACHINE = 61;

        /**
         * \brief Constant for LISTMACHINEOPTIONS__MACHINEID feature
         */
        static const int LISTMACHINEOPTIONS__MACHINEID = 62;

        /**
         * \brief Constant for CONFIGURATION__LISTCONFUSERS feature
         */
        static const int CONFIGURATION__LISTCONFUSERS = 63;

        /**
         * \brief Constant for CONFIGURATION__LISTCONFMACHINES feature
         */
        static const int CONFIGURATION__LISTCONFMACHINES = 64;

        /**
         * \brief Constant for CONFIGURATION__LISTCONFLOCALACCOUNTS feature
         */
        static const int CONFIGURATION__LISTCONFLOCALACCOUNTS = 65;

        /**
         * \brief Constant for CONFIGURATION__FILEPATH feature
         */
        static const int CONFIGURATION__FILEPATH = 66;

        /**
         * \brief Constant for OPTIONVALUE__OPTIONNAME feature
         */
        static const int OPTIONVALUE__OPTIONNAME = 67;

        /**
         * \brief Constant for OPTIONVALUE__VALUE feature
         */
        static const int OPTIONVALUE__VALUE = 68;

        /**
         * \brief Constant for LISTOPTIONSVALUES__OPTIONVALUES feature
         */
        static const int LISTOPTIONSVALUES__OPTIONVALUES = 69;

        /**
         * \brief Constant for LISTOPTOPTIONS__LISTALLDEFTVALUE feature
         */
        static const int LISTOPTOPTIONS__LISTALLDEFTVALUE = 70;

        /**
         * \brief Constant for LISTOPTOPTIONS__USERID feature
         */
        static const int LISTOPTOPTIONS__USERID = 71;

        /**
         * \brief Constant for LISTOPTOPTIONS__OPTIONNAME feature
         */
        static const int LISTOPTOPTIONS__OPTIONNAME = 72;

        // EClassifiers methods

        /**
         * \brief Returns the reflective object for class ConnectOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getConnectOptions();

        /**
         * \brief Returns the reflective object for class Session
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getSession();

        /**
         * \brief Returns the reflective object for class ListSessions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListSessions();

        /**
         * \brief Returns the reflective object for class ListSessionOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListSessionOptions();

        /**
         * \brief Returns the reflective object for class User
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getUser();

        /**
         * \brief Returns the reflective object for class ListUsers
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListUsers();

        /**
         * \brief Returns the reflective object for class Command
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getCommand();

        /**
         * \brief Returns the reflective object for class ListCommands
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListCommands();

        /**
         * \brief Returns the reflective object for class ListCmdOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListCmdOptions();

        /**
         * \brief Returns the reflective object for class LocalAccount
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getLocalAccount();

        /**
         * \brief Returns the reflective object for class ListLocalAccounts
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListLocalAccounts();

        /**
         * \brief Returns the reflective object for class ListLocalAccOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListLocalAccOptions();

        /**
         * \brief Returns the reflective object for class Machine
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getMachine();

        /**
         * \brief Returns the reflective object for class ListMachines
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListMachines();

        /**
         * \brief Returns the reflective object for class ListMachineOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListMachineOptions();

        /**
         * \brief Returns the reflective object for class Configuration
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getConfiguration();

        /**
         * \brief Returns the reflective object for class OptionValue
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getOptionValue();

        /**
         * \brief Returns the reflective object for class ListOptionsValues
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListOptionsValues();

        /**
         * \brief Returns the reflective object for class ListOptOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListOptOptions();

        /**
         * \brief Returns the reflective object for class SessionCloseType
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EEnum_ptr getSessionCloseType();

        /**
         * \brief Returns the reflective object for class PrivilegeType
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EEnum_ptr getPrivilegeType();

        /**
         * \brief Returns the reflective object for class StatusType
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EEnum_ptr getStatusType();

        /**
         * \brief Returns the reflective object for class CommandStatusType
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EEnum_ptr getCommandStatusType();

        // EStructuralFeatures methods

        /**
         * \brief Returns the reflective object for feature closePolicy of class ConnectOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getConnectOptions__closePolicy();

        /**
         * \brief Returns the reflective object for feature sessionInactivityDelay of class ConnectOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr
                getConnectOptions__sessionInactivityDelay();

        /**
         * \brief Returns the reflective object for feature substituteUserId of class ConnectOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getConnectOptions__substituteUserId();

        /**
         * \brief Returns the reflective object for feature sessionId of class Session
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSession__sessionId();

        /**
         * \brief Returns the reflective object for feature userId of class Session
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSession__userId();

        /**
         * \brief Returns the reflective object for feature sessionKey of class Session
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSession__sessionKey();

        /**
         * \brief Returns the reflective object for feature dateLastConnect of class Session
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSession__dateLastConnect();

        /**
         * \brief Returns the reflective object for feature dateCreation of class Session
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSession__dateCreation();

        /**
         * \brief Returns the reflective object for feature dateClosure of class Session
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSession__dateClosure();

        /**
         * \brief Returns the reflective object for feature status of class Session
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSession__status();

        /**
         * \brief Returns the reflective object for feature closePolicy of class Session
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSession__closePolicy();

        /**
         * \brief Returns the reflective object for feature timeout of class Session
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSession__timeout();

        /**
         * \brief Returns the reflective object for feature sessions of class ListSessions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListSessions__sessions();

        /**
         * \brief Returns the reflective object for feature status of class ListSessionOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListSessionOptions__status();

        /**
         * \brief Returns the reflective object for feature sessionClosePolicy of class ListSessionOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__sessionClosePolicy();

        /**
         * \brief Returns the reflective object for feature sessionInactivityDelay of class ListSessionOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__sessionInactivityDelay();

        /**
         * \brief Returns the reflective object for feature machineId of class ListSessionOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListSessionOptions__machineId();

        /**
         * \brief Returns the reflective object for feature adminListOption of class ListSessionOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__adminListOption();

        /**
         * \brief Returns the reflective object for feature userId of class ListSessionOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListSessionOptions__userId();

        /**
         * \brief Returns the reflective object for feature sessionId of class ListSessionOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListSessionOptions__sessionId();

        /**
         * \brief Returns the reflective object for feature startDateOption of class ListSessionOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__startDateOption();

        /**
         * \brief Returns the reflective object for feature endDateOption of class ListSessionOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListSessionOptions__endDateOption();

        /**
         * \brief Returns the reflective object for feature userId of class User
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getUser__userId();

        /**
         * \brief Returns the reflective object for feature password of class User
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getUser__password();

        /**
         * \brief Returns the reflective object for feature firstname of class User
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getUser__firstname();

        /**
         * \brief Returns the reflective object for feature lastname of class User
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getUser__lastname();

        /**
         * \brief Returns the reflective object for feature privilege of class User
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getUser__privilege();

        /**
         * \brief Returns the reflective object for feature email of class User
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getUser__email();

        /**
         * \brief Returns the reflective object for feature status of class User
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getUser__status();

        /**
         * \brief Returns the reflective object for feature users of class ListUsers
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListUsers__users();

        /**
         * \brief Returns the reflective object for feature commandId of class Command
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getCommand__commandId();

        /**
         * \brief Returns the reflective object for feature sessionId of class Command
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getCommand__sessionId();

        /**
         * \brief Returns the reflective object for feature machineId of class Command
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getCommand__machineId();

        /**
         * \brief Returns the reflective object for feature cmdDescription of class Command
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getCommand__cmdDescription();

        /**
         * \brief Returns the reflective object for feature cmdStartTime of class Command
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getCommand__cmdStartTime();

        /**
         * \brief Returns the reflective object for feature cmdEndTime of class Command
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getCommand__cmdEndTime();

        /**
         * \brief Returns the reflective object for feature status of class Command
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getCommand__status();

        /**
         * \brief Returns the reflective object for feature Commands of class ListCommands
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListCommands__Commands();

        /**
         * \brief Returns the reflective object for feature adminListOption of class ListCmdOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListCmdOptions__adminListOption();

        /**
         * \brief Returns the reflective object for feature userId of class ListCmdOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListCmdOptions__userId();

        /**
         * \brief Returns the reflective object for feature sessionId of class ListCmdOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListCmdOptions__sessionId();

        /**
         * \brief Returns the reflective object for feature startDateOption of class ListCmdOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListCmdOptions__startDateOption();

        /**
         * \brief Returns the reflective object for feature endDateOption of class ListCmdOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListCmdOptions__endDateOption();

        /**
         * \brief Returns the reflective object for feature userId of class LocalAccount
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getLocalAccount__userId();

        /**
         * \brief Returns the reflective object for feature machineId of class LocalAccount
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getLocalAccount__machineId();

        /**
         * \brief Returns the reflective object for feature acLogin of class LocalAccount
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getLocalAccount__acLogin();

        /**
         * \brief Returns the reflective object for feature sshKeyPath of class LocalAccount
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getLocalAccount__sshKeyPath();

        /**
         * \brief Returns the reflective object for feature homeDirectory of class LocalAccount
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getLocalAccount__homeDirectory();

        /**
         * \brief Returns the reflective object for feature accounts of class ListLocalAccounts
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListLocalAccounts__accounts();

        /**
         * \brief Returns the reflective object for feature adminListOption of class ListLocalAccOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr
                getListLocalAccOptions__adminListOption();

        /**
         * \brief Returns the reflective object for feature userId of class ListLocalAccOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListLocalAccOptions__userId();

        /**
         * \brief Returns the reflective object for feature machineId of class ListLocalAccOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListLocalAccOptions__machineId();

        /**
         * \brief Returns the reflective object for feature machineId of class Machine
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMachine__machineId();

        /**
         * \brief Returns the reflective object for feature name of class Machine
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMachine__name();

        /**
         * \brief Returns the reflective object for feature site of class Machine
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMachine__site();

        /**
         * \brief Returns the reflective object for feature machineDescription of class Machine
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMachine__machineDescription();

        /**
         * \brief Returns the reflective object for feature language of class Machine
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMachine__language();

        /**
         * \brief Returns the reflective object for feature status of class Machine
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMachine__status();

        /**
         * \brief Returns the reflective object for feature sshPublicKey of class Machine
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMachine__sshPublicKey();

        /**
         * \brief Returns the reflective object for feature machines of class ListMachines
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListMachines__machines();

        /**
         * \brief Returns the reflective object for feature userId of class ListMachineOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListMachineOptions__userId();

        /**
         * \brief Returns the reflective object for feature listAllmachine of class ListMachineOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListMachineOptions__listAllmachine();

        /**
         * \brief Returns the reflective object for feature machineId of class ListMachineOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListMachineOptions__machineId();

        /**
         * \brief Returns the reflective object for feature listConfUsers of class Configuration
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getConfiguration__listConfUsers();

        /**
         * \brief Returns the reflective object for feature listConfMachines of class Configuration
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getConfiguration__listConfMachines();

        /**
         * \brief Returns the reflective object for feature listConfLocalAccounts of class Configuration
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr
                getConfiguration__listConfLocalAccounts();

        /**
         * \brief Returns the reflective object for feature filePath of class Configuration
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getConfiguration__filePath();

        /**
         * \brief Returns the reflective object for feature optionName of class OptionValue
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getOptionValue__optionName();

        /**
         * \brief Returns the reflective object for feature value of class OptionValue
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getOptionValue__value();

        /**
         * \brief Returns the reflective object for feature optionValues of class ListOptionsValues
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListOptionsValues__optionValues();

        /**
         * \brief Returns the reflective object for feature listAllDeftValue of class ListOptOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListOptOptions__listAllDeftValue();

        /**
         * \brief Returns the reflective object for feature userId of class ListOptOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListOptOptions__userId();

        /**
         * \brief Returns the reflective object for feature optionName of class ListOptOptions
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListOptOptions__optionName();

    protected:

        /**
         * \brief The instance of the package
         */
        static std::auto_ptr< UMS_DataPackage > s_instance;

        UMS_DataPackage();

        // EClass instances 

        /**
         * \brief The instance for the class ConnectOptions
         */
        ::ecore::EClass_ptr m_ConnectOptionsEClass;

        /**
         * \brief The instance for the class Session
         */
        ::ecore::EClass_ptr m_SessionEClass;

        /**
         * \brief The instance for the class ListSessions
         */
        ::ecore::EClass_ptr m_ListSessionsEClass;

        /**
         * \brief The instance for the class ListSessionOptions
         */
        ::ecore::EClass_ptr m_ListSessionOptionsEClass;

        /**
         * \brief The instance for the class User
         */
        ::ecore::EClass_ptr m_UserEClass;

        /**
         * \brief The instance for the class ListUsers
         */
        ::ecore::EClass_ptr m_ListUsersEClass;

        /**
         * \brief The instance for the class Command
         */
        ::ecore::EClass_ptr m_CommandEClass;

        /**
         * \brief The instance for the class ListCommands
         */
        ::ecore::EClass_ptr m_ListCommandsEClass;

        /**
         * \brief The instance for the class ListCmdOptions
         */
        ::ecore::EClass_ptr m_ListCmdOptionsEClass;

        /**
         * \brief The instance for the class LocalAccount
         */
        ::ecore::EClass_ptr m_LocalAccountEClass;

        /**
         * \brief The instance for the class ListLocalAccounts
         */
        ::ecore::EClass_ptr m_ListLocalAccountsEClass;

        /**
         * \brief The instance for the class ListLocalAccOptions
         */
        ::ecore::EClass_ptr m_ListLocalAccOptionsEClass;

        /**
         * \brief The instance for the class Machine
         */
        ::ecore::EClass_ptr m_MachineEClass;

        /**
         * \brief The instance for the class ListMachines
         */
        ::ecore::EClass_ptr m_ListMachinesEClass;

        /**
         * \brief The instance for the class ListMachineOptions
         */
        ::ecore::EClass_ptr m_ListMachineOptionsEClass;

        /**
         * \brief The instance for the class Configuration
         */
        ::ecore::EClass_ptr m_ConfigurationEClass;

        /**
         * \brief The instance for the class OptionValue
         */
        ::ecore::EClass_ptr m_OptionValueEClass;

        /**
         * \brief The instance for the class ListOptionsValues
         */
        ::ecore::EClass_ptr m_ListOptionsValuesEClass;

        /**
         * \brief The instance for the class ListOptOptions
         */
        ::ecore::EClass_ptr m_ListOptOptionsEClass;

        // EEnuminstances 

        /**
         * \brief The instance for the enum SessionCloseType
         */
        ::ecore::EEnum_ptr m_SessionCloseTypeEEnum;

        /**
         * \brief The instance for the enum PrivilegeType
         */
        ::ecore::EEnum_ptr m_PrivilegeTypeEEnum;

        /**
         * \brief The instance for the enum StatusType
         */
        ::ecore::EEnum_ptr m_StatusTypeEEnum;

        /**
         * \brief The instance for the enum CommandStatusType
         */
        ::ecore::EEnum_ptr m_CommandStatusTypeEEnum;

        // EDataType instances 


        // EStructuralFeatures instances

        /**
         * \brief The instance for the feature closePolicy of class ConnectOptions
         */
        ::ecore::EAttribute_ptr m_ConnectOptions__closePolicy;

        /**
         * \brief The instance for the feature sessionInactivityDelay of class ConnectOptions
         */
        ::ecore::EAttribute_ptr m_ConnectOptions__sessionInactivityDelay;

        /**
         * \brief The instance for the feature substituteUserId of class ConnectOptions
         */
        ::ecore::EAttribute_ptr m_ConnectOptions__substituteUserId;

        /**
         * \brief The instance for the feature sessionId of class Session
         */
        ::ecore::EAttribute_ptr m_Session__sessionId;

        /**
         * \brief The instance for the feature userId of class Session
         */
        ::ecore::EAttribute_ptr m_Session__userId;

        /**
         * \brief The instance for the feature sessionKey of class Session
         */
        ::ecore::EAttribute_ptr m_Session__sessionKey;

        /**
         * \brief The instance for the feature dateLastConnect of class Session
         */
        ::ecore::EAttribute_ptr m_Session__dateLastConnect;

        /**
         * \brief The instance for the feature dateCreation of class Session
         */
        ::ecore::EAttribute_ptr m_Session__dateCreation;

        /**
         * \brief The instance for the feature dateClosure of class Session
         */
        ::ecore::EAttribute_ptr m_Session__dateClosure;

        /**
         * \brief The instance for the feature status of class Session
         */
        ::ecore::EAttribute_ptr m_Session__status;

        /**
         * \brief The instance for the feature closePolicy of class Session
         */
        ::ecore::EAttribute_ptr m_Session__closePolicy;

        /**
         * \brief The instance for the feature timeout of class Session
         */
        ::ecore::EAttribute_ptr m_Session__timeout;

        /**
         * \brief The instance for the feature sessions of class ListSessions
         */
        ::ecore::EReference_ptr m_ListSessions__sessions;

        /**
         * \brief The instance for the feature status of class ListSessionOptions
         */
        ::ecore::EAttribute_ptr m_ListSessionOptions__status;

        /**
         * \brief The instance for the feature sessionClosePolicy of class ListSessionOptions
         */
        ::ecore::EAttribute_ptr m_ListSessionOptions__sessionClosePolicy;

        /**
         * \brief The instance for the feature sessionInactivityDelay of class ListSessionOptions
         */
        ::ecore::EAttribute_ptr m_ListSessionOptions__sessionInactivityDelay;

        /**
         * \brief The instance for the feature machineId of class ListSessionOptions
         */
        ::ecore::EAttribute_ptr m_ListSessionOptions__machineId;

        /**
         * \brief The instance for the feature adminListOption of class ListSessionOptions
         */
        ::ecore::EAttribute_ptr m_ListSessionOptions__adminListOption;

        /**
         * \brief The instance for the feature userId of class ListSessionOptions
         */
        ::ecore::EAttribute_ptr m_ListSessionOptions__userId;

        /**
         * \brief The instance for the feature sessionId of class ListSessionOptions
         */
        ::ecore::EAttribute_ptr m_ListSessionOptions__sessionId;

        /**
         * \brief The instance for the feature startDateOption of class ListSessionOptions
         */
        ::ecore::EAttribute_ptr m_ListSessionOptions__startDateOption;

        /**
         * \brief The instance for the feature endDateOption of class ListSessionOptions
         */
        ::ecore::EAttribute_ptr m_ListSessionOptions__endDateOption;

        /**
         * \brief The instance for the feature userId of class User
         */
        ::ecore::EAttribute_ptr m_User__userId;

        /**
         * \brief The instance for the feature password of class User
         */
        ::ecore::EAttribute_ptr m_User__password;

        /**
         * \brief The instance for the feature firstname of class User
         */
        ::ecore::EAttribute_ptr m_User__firstname;

        /**
         * \brief The instance for the feature lastname of class User
         */
        ::ecore::EAttribute_ptr m_User__lastname;

        /**
         * \brief The instance for the feature privilege of class User
         */
        ::ecore::EAttribute_ptr m_User__privilege;

        /**
         * \brief The instance for the feature email of class User
         */
        ::ecore::EAttribute_ptr m_User__email;

        /**
         * \brief The instance for the feature status of class User
         */
        ::ecore::EAttribute_ptr m_User__status;

        /**
         * \brief The instance for the feature users of class ListUsers
         */
        ::ecore::EReference_ptr m_ListUsers__users;

        /**
         * \brief The instance for the feature commandId of class Command
         */
        ::ecore::EAttribute_ptr m_Command__commandId;

        /**
         * \brief The instance for the feature sessionId of class Command
         */
        ::ecore::EAttribute_ptr m_Command__sessionId;

        /**
         * \brief The instance for the feature machineId of class Command
         */
        ::ecore::EAttribute_ptr m_Command__machineId;

        /**
         * \brief The instance for the feature cmdDescription of class Command
         */
        ::ecore::EAttribute_ptr m_Command__cmdDescription;

        /**
         * \brief The instance for the feature cmdStartTime of class Command
         */
        ::ecore::EAttribute_ptr m_Command__cmdStartTime;

        /**
         * \brief The instance for the feature cmdEndTime of class Command
         */
        ::ecore::EAttribute_ptr m_Command__cmdEndTime;

        /**
         * \brief The instance for the feature status of class Command
         */
        ::ecore::EAttribute_ptr m_Command__status;

        /**
         * \brief The instance for the feature Commands of class ListCommands
         */
        ::ecore::EReference_ptr m_ListCommands__Commands;

        /**
         * \brief The instance for the feature adminListOption of class ListCmdOptions
         */
        ::ecore::EAttribute_ptr m_ListCmdOptions__adminListOption;

        /**
         * \brief The instance for the feature userId of class ListCmdOptions
         */
        ::ecore::EAttribute_ptr m_ListCmdOptions__userId;

        /**
         * \brief The instance for the feature sessionId of class ListCmdOptions
         */
        ::ecore::EAttribute_ptr m_ListCmdOptions__sessionId;

        /**
         * \brief The instance for the feature startDateOption of class ListCmdOptions
         */
        ::ecore::EAttribute_ptr m_ListCmdOptions__startDateOption;

        /**
         * \brief The instance for the feature endDateOption of class ListCmdOptions
         */
        ::ecore::EAttribute_ptr m_ListCmdOptions__endDateOption;

        /**
         * \brief The instance for the feature userId of class LocalAccount
         */
        ::ecore::EAttribute_ptr m_LocalAccount__userId;

        /**
         * \brief The instance for the feature machineId of class LocalAccount
         */
        ::ecore::EAttribute_ptr m_LocalAccount__machineId;

        /**
         * \brief The instance for the feature acLogin of class LocalAccount
         */
        ::ecore::EAttribute_ptr m_LocalAccount__acLogin;

        /**
         * \brief The instance for the feature sshKeyPath of class LocalAccount
         */
        ::ecore::EAttribute_ptr m_LocalAccount__sshKeyPath;

        /**
         * \brief The instance for the feature homeDirectory of class LocalAccount
         */
        ::ecore::EAttribute_ptr m_LocalAccount__homeDirectory;

        /**
         * \brief The instance for the feature accounts of class ListLocalAccounts
         */
        ::ecore::EReference_ptr m_ListLocalAccounts__accounts;

        /**
         * \brief The instance for the feature adminListOption of class ListLocalAccOptions
         */
        ::ecore::EAttribute_ptr m_ListLocalAccOptions__adminListOption;

        /**
         * \brief The instance for the feature userId of class ListLocalAccOptions
         */
        ::ecore::EAttribute_ptr m_ListLocalAccOptions__userId;

        /**
         * \brief The instance for the feature machineId of class ListLocalAccOptions
         */
        ::ecore::EAttribute_ptr m_ListLocalAccOptions__machineId;

        /**
         * \brief The instance for the feature machineId of class Machine
         */
        ::ecore::EAttribute_ptr m_Machine__machineId;

        /**
         * \brief The instance for the feature name of class Machine
         */
        ::ecore::EAttribute_ptr m_Machine__name;

        /**
         * \brief The instance for the feature site of class Machine
         */
        ::ecore::EAttribute_ptr m_Machine__site;

        /**
         * \brief The instance for the feature machineDescription of class Machine
         */
        ::ecore::EAttribute_ptr m_Machine__machineDescription;

        /**
         * \brief The instance for the feature language of class Machine
         */
        ::ecore::EAttribute_ptr m_Machine__language;

        /**
         * \brief The instance for the feature status of class Machine
         */
        ::ecore::EAttribute_ptr m_Machine__status;

        /**
         * \brief The instance for the feature sshPublicKey of class Machine
         */
        ::ecore::EAttribute_ptr m_Machine__sshPublicKey;

        /**
         * \brief The instance for the feature machines of class ListMachines
         */
        ::ecore::EReference_ptr m_ListMachines__machines;

        /**
         * \brief The instance for the feature userId of class ListMachineOptions
         */
        ::ecore::EAttribute_ptr m_ListMachineOptions__userId;

        /**
         * \brief The instance for the feature listAllmachine of class ListMachineOptions
         */
        ::ecore::EAttribute_ptr m_ListMachineOptions__listAllmachine;

        /**
         * \brief The instance for the feature machineId of class ListMachineOptions
         */
        ::ecore::EAttribute_ptr m_ListMachineOptions__machineId;

        /**
         * \brief The instance for the feature listConfUsers of class Configuration
         */
        ::ecore::EReference_ptr m_Configuration__listConfUsers;

        /**
         * \brief The instance for the feature listConfMachines of class Configuration
         */
        ::ecore::EReference_ptr m_Configuration__listConfMachines;

        /**
         * \brief The instance for the feature listConfLocalAccounts of class Configuration
         */
        ::ecore::EReference_ptr m_Configuration__listConfLocalAccounts;

        /**
         * \brief The instance for the feature filePath of class Configuration
         */
        ::ecore::EAttribute_ptr m_Configuration__filePath;

        /**
         * \brief The instance for the feature optionName of class OptionValue
         */
        ::ecore::EAttribute_ptr m_OptionValue__optionName;

        /**
         * \brief The instance for the feature value of class OptionValue
         */
        ::ecore::EAttribute_ptr m_OptionValue__value;

        /**
         * \brief The instance for the feature optionValues of class ListOptionsValues
         */
        ::ecore::EReference_ptr m_ListOptionsValues__optionValues;

        /**
         * \brief The instance for the feature listAllDeftValue of class ListOptOptions
         */
        ::ecore::EAttribute_ptr m_ListOptOptions__listAllDeftValue;

        /**
         * \brief The instance for the feature userId of class ListOptOptions
         */
        ::ecore::EAttribute_ptr m_ListOptOptions__userId;

        /**
         * \brief The instance for the feature optionName of class ListOptOptions
         */
        ::ecore::EAttribute_ptr m_ListOptOptions__optionName;

    };

} // UMS_Data


#endif // _UMS_DATAPACKAGE_HPP
