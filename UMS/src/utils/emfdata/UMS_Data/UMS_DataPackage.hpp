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

#ifndef _UMS_DATAPACKAGE_HPP
#define _UMS_DATAPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <UMS_Data_forward.hpp>

namespace UMS_Data
{

    class UMS_DataPackage: public virtual ::ecore::EPackage
    {
    public:

        static UMS_DataPackage_ptr _instance();

        // IDs for classifiers

        static const int COMMAND = 0;

        static const int CONFIGURATION = 1;

        static const int CONNECTOPTIONS = 2;

        static const int LISTCMDOPTIONS = 3;

        static const int LISTCOMMANDS = 4;

        static const int LISTLOCALACCOPTIONS = 5;

        static const int LISTLOCALACCOUNTS = 6;

        static const int LISTMACHINEOPTIONS = 7;

        static const int LISTMACHINES = 8;

        static const int LISTOPTIONSVALUES = 9;

        static const int LISTOPTOPTIONS = 10;

        static const int LISTSESSIONOPTIONS = 11;

        static const int LISTSESSIONS = 12;

        static const int LISTUSERS = 13;

        static const int LOCALACCOUNT = 14;

        static const int MACHINE = 15;

        static const int OPTIONVALUE = 16;

        static const int PRIVILEGETYPE = 17;

        static const int SESSION = 18;

        static const int SESSIONCLOSETYPE = 19;

        static const int STATUSTYPE = 20;

        static const int USER = 21;

        static const int CONNECTOPTIONS__CLOSEPOLICY = 0;

        static const int CONNECTOPTIONS__SESSIONINACTIVITYDELAY = 1;

        static const int CONNECTOPTIONS__SUBSTITUTEUSERID = 2;

        static const int SESSION__SESSIONID = 3;

        static const int SESSION__USERID = 4;

        static const int SESSION__SESSIONKEY = 5;

        static const int SESSION__DATELASTCONNECT = 6;

        static const int SESSION__DATECREATION = 7;

        static const int SESSION__DATECLOSURE = 8;

        static const int SESSION__STATUS = 9;

        static const int SESSION__CLOSEPOLICY = 10;

        static const int SESSION__TIMEOUT = 11;

        static const int LISTSESSIONS__SESSIONS = 12;

        static const int LISTSESSIONOPTIONS__STATUS = 13;

        static const int LISTSESSIONOPTIONS__SESSIONCLOSEPOLICY = 14;

        static const int LISTSESSIONOPTIONS__SESSIONINACTIVITYDELAY = 15;

        static const int LISTSESSIONOPTIONS__MACHINEID = 16;

        static const int LISTSESSIONOPTIONS__ADMINLISTOPTION = 17;

        static const int LISTSESSIONOPTIONS__USERID = 18;

        static const int LISTSESSIONOPTIONS__SESSIONID = 19;

        static const int LISTSESSIONOPTIONS__STARTDATEOPTION = 20;

        static const int LISTSESSIONOPTIONS__ENDDATEOPTION = 21;

        static const int USER__USERID = 22;

        static const int USER__PASSWORD = 23;

        static const int USER__FIRSTNAME = 24;

        static const int USER__LASTNAME = 25;

        static const int USER__PRIVILEGE = 26;

        static const int USER__EMAIL = 27;

        static const int USER__STATUS = 28;

        static const int LISTUSERS__USERS = 29;

        static const int COMMAND__COMMANDID = 30;

        static const int COMMAND__SESSIONID = 31;

        static const int COMMAND__MACHINEID = 32;

        static const int COMMAND__CMDDESCRIPTION = 33;

        static const int COMMAND__CMDSTARTTIME = 34;

        static const int COMMAND__CMDENDTIME = 35;

        static const int LISTCOMMANDS__COMMANDS = 36;

        static const int LISTCMDOPTIONS__ADMINLISTOPTION = 37;

        static const int LISTCMDOPTIONS__USERID = 38;

        static const int LISTCMDOPTIONS__SESSIONID = 39;

        static const int LISTCMDOPTIONS__STARTDATEOPTION = 40;

        static const int LISTCMDOPTIONS__ENDDATEOPTION = 41;

        static const int LOCALACCOUNT__USERID = 42;

        static const int LOCALACCOUNT__MACHINEID = 43;

        static const int LOCALACCOUNT__ACLOGIN = 44;

        static const int LOCALACCOUNT__SSHKEYPATH = 45;

        static const int LOCALACCOUNT__HOMEDIRECTORY = 46;

        static const int LISTLOCALACCOUNTS__ACCOUNTS = 47;

        static const int LISTLOCALACCOPTIONS__ADMINLISTOPTION = 48;

        static const int LISTLOCALACCOPTIONS__USERID = 49;

        static const int LISTLOCALACCOPTIONS__MACHINEID = 50;

        static const int MACHINE__MACHINEID = 51;

        static const int MACHINE__NAME = 52;

        static const int MACHINE__SITE = 53;

        static const int MACHINE__MACHINEDESCRIPTION = 54;

        static const int MACHINE__LANGUAGE = 55;

        static const int MACHINE__STATUS = 56;

        static const int LISTMACHINES__MACHINES = 57;

        static const int LISTMACHINEOPTIONS__USERID = 58;

        static const int LISTMACHINEOPTIONS__LISTALLMACHINE = 59;

        static const int LISTMACHINEOPTIONS__MACHINEID = 60;

        static const int CONFIGURATION__LISTCONFUSERS = 61;

        static const int CONFIGURATION__LISTCONFMACHINES = 62;

        static const int CONFIGURATION__LISTCONFLOCALACCOUNTS = 63;

        static const int OPTIONVALUE__OPTIONNAME = 64;

        static const int OPTIONVALUE__VALUE = 65;

        static const int LISTOPTIONSVALUES__OPTIONVALUES = 66;

        static const int LISTOPTOPTIONS__LISTALLDEFTVALUE = 67;

        static const int LISTOPTOPTIONS__USERID = 68;

        static const int LISTOPTOPTIONS__OPTIONNAME = 69;

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

        virtual ::ecore::EClass_ptr getConnectOptions();

        virtual ::ecore::EClass_ptr getSession();

        virtual ::ecore::EClass_ptr getListSessions();

        virtual ::ecore::EClass_ptr getListSessionOptions();

        virtual ::ecore::EClass_ptr getUser();

        virtual ::ecore::EClass_ptr getListUsers();

        virtual ::ecore::EClass_ptr getCommand();

        virtual ::ecore::EClass_ptr getListCommands();

        virtual ::ecore::EClass_ptr getListCmdOptions();

        virtual ::ecore::EClass_ptr getLocalAccount();

        virtual ::ecore::EClass_ptr getListLocalAccounts();

        virtual ::ecore::EClass_ptr getListLocalAccOptions();

        virtual ::ecore::EClass_ptr getMachine();

        virtual ::ecore::EClass_ptr getListMachines();

        virtual ::ecore::EClass_ptr getListMachineOptions();

        virtual ::ecore::EClass_ptr getConfiguration();

        virtual ::ecore::EClass_ptr getOptionValue();

        virtual ::ecore::EClass_ptr getListOptionsValues();

        virtual ::ecore::EClass_ptr getListOptOptions();

        virtual ::ecore::EEnum_ptr getSessionCloseType();

        virtual ::ecore::EEnum_ptr getPrivilegeType();

        virtual ::ecore::EEnum_ptr getStatusType();

        // EStructuralFeatures methods

        virtual ::ecore::EAttribute_ptr getConnectOptions__closePolicy();

        virtual ::ecore::EAttribute_ptr
                getConnectOptions__sessionInactivityDelay();

        virtual ::ecore::EAttribute_ptr getConnectOptions__substituteUserId();

        virtual ::ecore::EAttribute_ptr getSession__sessionId();

        virtual ::ecore::EAttribute_ptr getSession__userId();

        virtual ::ecore::EAttribute_ptr getSession__sessionKey();

        virtual ::ecore::EAttribute_ptr getSession__dateLastConnect();

        virtual ::ecore::EAttribute_ptr getSession__dateCreation();

        virtual ::ecore::EAttribute_ptr getSession__dateClosure();

        virtual ::ecore::EAttribute_ptr getSession__status();

        virtual ::ecore::EAttribute_ptr getSession__closePolicy();

        virtual ::ecore::EAttribute_ptr getSession__timeout();

        virtual ::ecore::EReference_ptr getListSessions__sessions();

        virtual ::ecore::EAttribute_ptr getListSessionOptions__status();

        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__sessionClosePolicy();

        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__sessionInactivityDelay();

        virtual ::ecore::EAttribute_ptr getListSessionOptions__machineId();

        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__adminListOption();

        virtual ::ecore::EAttribute_ptr getListSessionOptions__userId();

        virtual ::ecore::EAttribute_ptr getListSessionOptions__sessionId();

        virtual ::ecore::EAttribute_ptr
                getListSessionOptions__startDateOption();

        virtual ::ecore::EAttribute_ptr getListSessionOptions__endDateOption();

        virtual ::ecore::EAttribute_ptr getUser__userId();

        virtual ::ecore::EAttribute_ptr getUser__password();

        virtual ::ecore::EAttribute_ptr getUser__firstname();

        virtual ::ecore::EAttribute_ptr getUser__lastname();

        virtual ::ecore::EAttribute_ptr getUser__privilege();

        virtual ::ecore::EAttribute_ptr getUser__email();

        virtual ::ecore::EAttribute_ptr getUser__status();

        virtual ::ecore::EReference_ptr getListUsers__users();

        virtual ::ecore::EAttribute_ptr getCommand__commandId();

        virtual ::ecore::EAttribute_ptr getCommand__sessionId();

        virtual ::ecore::EAttribute_ptr getCommand__machineId();

        virtual ::ecore::EAttribute_ptr getCommand__cmdDescription();

        virtual ::ecore::EAttribute_ptr getCommand__cmdStartTime();

        virtual ::ecore::EAttribute_ptr getCommand__cmdEndTime();

        virtual ::ecore::EReference_ptr getListCommands__Commands();

        virtual ::ecore::EAttribute_ptr getListCmdOptions__adminListOption();

        virtual ::ecore::EAttribute_ptr getListCmdOptions__userId();

        virtual ::ecore::EAttribute_ptr getListCmdOptions__sessionId();

        virtual ::ecore::EAttribute_ptr getListCmdOptions__startDateOption();

        virtual ::ecore::EAttribute_ptr getListCmdOptions__endDateOption();

        virtual ::ecore::EAttribute_ptr getLocalAccount__userId();

        virtual ::ecore::EAttribute_ptr getLocalAccount__machineId();

        virtual ::ecore::EAttribute_ptr getLocalAccount__acLogin();

        virtual ::ecore::EAttribute_ptr getLocalAccount__sshKeyPath();

        virtual ::ecore::EAttribute_ptr getLocalAccount__homeDirectory();

        virtual ::ecore::EReference_ptr getListLocalAccounts__accounts();

        virtual ::ecore::EAttribute_ptr
                getListLocalAccOptions__adminListOption();

        virtual ::ecore::EAttribute_ptr getListLocalAccOptions__userId();

        virtual ::ecore::EAttribute_ptr getListLocalAccOptions__machineId();

        virtual ::ecore::EAttribute_ptr getMachine__machineId();

        virtual ::ecore::EAttribute_ptr getMachine__name();

        virtual ::ecore::EAttribute_ptr getMachine__site();

        virtual ::ecore::EAttribute_ptr getMachine__machineDescription();

        virtual ::ecore::EAttribute_ptr getMachine__language();

        virtual ::ecore::EAttribute_ptr getMachine__status();

        virtual ::ecore::EReference_ptr getListMachines__machines();

        virtual ::ecore::EAttribute_ptr getListMachineOptions__userId();

        virtual ::ecore::EAttribute_ptr getListMachineOptions__listAllmachine();

        virtual ::ecore::EAttribute_ptr getListMachineOptions__machineId();

        virtual ::ecore::EReference_ptr getConfiguration__listConfUsers();

        virtual ::ecore::EReference_ptr getConfiguration__listConfMachines();

        virtual ::ecore::EReference_ptr
                getConfiguration__listConfLocalAccounts();

        virtual ::ecore::EAttribute_ptr getOptionValue__optionName();

        virtual ::ecore::EAttribute_ptr getOptionValue__value();

        virtual ::ecore::EReference_ptr getListOptionsValues__optionValues();

        virtual ::ecore::EAttribute_ptr getListOptOptions__listAllDeftValue();

        virtual ::ecore::EAttribute_ptr getListOptOptions__userId();

        virtual ::ecore::EAttribute_ptr getListOptOptions__optionName();

    protected:

        static std::auto_ptr< UMS_DataPackage > s_instance;

        UMS_DataPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_ConnectOptionsEClass;

        ::ecore::EClass_ptr m_SessionEClass;

        ::ecore::EClass_ptr m_ListSessionsEClass;

        ::ecore::EClass_ptr m_ListSessionOptionsEClass;

        ::ecore::EClass_ptr m_UserEClass;

        ::ecore::EClass_ptr m_ListUsersEClass;

        ::ecore::EClass_ptr m_CommandEClass;

        ::ecore::EClass_ptr m_ListCommandsEClass;

        ::ecore::EClass_ptr m_ListCmdOptionsEClass;

        ::ecore::EClass_ptr m_LocalAccountEClass;

        ::ecore::EClass_ptr m_ListLocalAccountsEClass;

        ::ecore::EClass_ptr m_ListLocalAccOptionsEClass;

        ::ecore::EClass_ptr m_MachineEClass;

        ::ecore::EClass_ptr m_ListMachinesEClass;

        ::ecore::EClass_ptr m_ListMachineOptionsEClass;

        ::ecore::EClass_ptr m_ConfigurationEClass;

        ::ecore::EClass_ptr m_OptionValueEClass;

        ::ecore::EClass_ptr m_ListOptionsValuesEClass;

        ::ecore::EClass_ptr m_ListOptOptionsEClass;

        // EEnuminstances 

        ::ecore::EEnum_ptr m_SessionCloseTypeEEnum;

        ::ecore::EEnum_ptr m_PrivilegeTypeEEnum;

        ::ecore::EEnum_ptr m_StatusTypeEEnum;

        // EDataType instances 


        // EStructuralFeatures instances

        ::ecore::EAttribute_ptr m_ConnectOptions__closePolicy;

        ::ecore::EAttribute_ptr m_ConnectOptions__sessionInactivityDelay;

        ::ecore::EAttribute_ptr m_ConnectOptions__substituteUserId;

        ::ecore::EAttribute_ptr m_Session__sessionId;

        ::ecore::EAttribute_ptr m_Session__userId;

        ::ecore::EAttribute_ptr m_Session__sessionKey;

        ::ecore::EAttribute_ptr m_Session__dateLastConnect;

        ::ecore::EAttribute_ptr m_Session__dateCreation;

        ::ecore::EAttribute_ptr m_Session__dateClosure;

        ::ecore::EAttribute_ptr m_Session__status;

        ::ecore::EAttribute_ptr m_Session__closePolicy;

        ::ecore::EAttribute_ptr m_Session__timeout;

        ::ecore::EReference_ptr m_ListSessions__sessions;

        ::ecore::EAttribute_ptr m_ListSessionOptions__status;

        ::ecore::EAttribute_ptr m_ListSessionOptions__sessionClosePolicy;

        ::ecore::EAttribute_ptr m_ListSessionOptions__sessionInactivityDelay;

        ::ecore::EAttribute_ptr m_ListSessionOptions__machineId;

        ::ecore::EAttribute_ptr m_ListSessionOptions__adminListOption;

        ::ecore::EAttribute_ptr m_ListSessionOptions__userId;

        ::ecore::EAttribute_ptr m_ListSessionOptions__sessionId;

        ::ecore::EAttribute_ptr m_ListSessionOptions__startDateOption;

        ::ecore::EAttribute_ptr m_ListSessionOptions__endDateOption;

        ::ecore::EAttribute_ptr m_User__userId;

        ::ecore::EAttribute_ptr m_User__password;

        ::ecore::EAttribute_ptr m_User__firstname;

        ::ecore::EAttribute_ptr m_User__lastname;

        ::ecore::EAttribute_ptr m_User__privilege;

        ::ecore::EAttribute_ptr m_User__email;

        ::ecore::EAttribute_ptr m_User__status;

        ::ecore::EReference_ptr m_ListUsers__users;

        ::ecore::EAttribute_ptr m_Command__commandId;

        ::ecore::EAttribute_ptr m_Command__sessionId;

        ::ecore::EAttribute_ptr m_Command__machineId;

        ::ecore::EAttribute_ptr m_Command__cmdDescription;

        ::ecore::EAttribute_ptr m_Command__cmdStartTime;

        ::ecore::EAttribute_ptr m_Command__cmdEndTime;

        ::ecore::EReference_ptr m_ListCommands__Commands;

        ::ecore::EAttribute_ptr m_ListCmdOptions__adminListOption;

        ::ecore::EAttribute_ptr m_ListCmdOptions__userId;

        ::ecore::EAttribute_ptr m_ListCmdOptions__sessionId;

        ::ecore::EAttribute_ptr m_ListCmdOptions__startDateOption;

        ::ecore::EAttribute_ptr m_ListCmdOptions__endDateOption;

        ::ecore::EAttribute_ptr m_LocalAccount__userId;

        ::ecore::EAttribute_ptr m_LocalAccount__machineId;

        ::ecore::EAttribute_ptr m_LocalAccount__acLogin;

        ::ecore::EAttribute_ptr m_LocalAccount__sshKeyPath;

        ::ecore::EAttribute_ptr m_LocalAccount__homeDirectory;

        ::ecore::EReference_ptr m_ListLocalAccounts__accounts;

        ::ecore::EAttribute_ptr m_ListLocalAccOptions__adminListOption;

        ::ecore::EAttribute_ptr m_ListLocalAccOptions__userId;

        ::ecore::EAttribute_ptr m_ListLocalAccOptions__machineId;

        ::ecore::EAttribute_ptr m_Machine__machineId;

        ::ecore::EAttribute_ptr m_Machine__name;

        ::ecore::EAttribute_ptr m_Machine__site;

        ::ecore::EAttribute_ptr m_Machine__machineDescription;

        ::ecore::EAttribute_ptr m_Machine__language;

        ::ecore::EAttribute_ptr m_Machine__status;

        ::ecore::EReference_ptr m_ListMachines__machines;

        ::ecore::EAttribute_ptr m_ListMachineOptions__userId;

        ::ecore::EAttribute_ptr m_ListMachineOptions__listAllmachine;

        ::ecore::EAttribute_ptr m_ListMachineOptions__machineId;

        ::ecore::EReference_ptr m_Configuration__listConfUsers;

        ::ecore::EReference_ptr m_Configuration__listConfMachines;

        ::ecore::EReference_ptr m_Configuration__listConfLocalAccounts;

        ::ecore::EAttribute_ptr m_OptionValue__optionName;

        ::ecore::EAttribute_ptr m_OptionValue__value;

        ::ecore::EReference_ptr m_ListOptionsValues__optionValues;

        ::ecore::EAttribute_ptr m_ListOptOptions__listAllDeftValue;

        ::ecore::EAttribute_ptr m_ListOptOptions__userId;

        ::ecore::EAttribute_ptr m_ListOptOptions__optionName;

    };

} // UMS_Data


#endif // _UMS_DATAPACKAGE_HPP
