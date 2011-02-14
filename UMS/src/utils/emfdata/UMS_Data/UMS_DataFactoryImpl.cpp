// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/UMS_DataFactoryImpl.cpp
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

#include <UMS_Data/UMS_DataFactory.hpp>
#include <UMS_Data/UMS_DataPackage.hpp>
#include <UMS_Data/ConnectOptions.hpp>
#include <UMS_Data/Session.hpp>
#include <UMS_Data/ListSessions.hpp>
#include <UMS_Data/ListSessionOptions.hpp>
#include <UMS_Data/User.hpp>
#include <UMS_Data/ListUsers.hpp>
#include <UMS_Data/Command.hpp>
#include <UMS_Data/ListCommands.hpp>
#include <UMS_Data/ListCmdOptions.hpp>
#include <UMS_Data/LocalAccount.hpp>
#include <UMS_Data/ListLocalAccounts.hpp>
#include <UMS_Data/ListLocalAccOptions.hpp>
#include <UMS_Data/Machine.hpp>
#include <UMS_Data/ListMachines.hpp>
#include <UMS_Data/ListMachineOptions.hpp>
#include <UMS_Data/Configuration.hpp>
#include <UMS_Data/OptionValue.hpp>
#include <UMS_Data/ListOptionsValues.hpp>
#include <UMS_Data/ListOptOptions.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

UMS_DataFactory::UMS_DataFactory()
{
    s_instance.reset(this);
}

::ecore::EObject_ptr UMS_DataFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case UMS_DataPackage::CONNECTOPTIONS:
        return createConnectOptions();
    case UMS_DataPackage::SESSION:
        return createSession();
    case UMS_DataPackage::LISTSESSIONS:
        return createListSessions();
    case UMS_DataPackage::LISTSESSIONOPTIONS:
        return createListSessionOptions();
    case UMS_DataPackage::USER:
        return createUser();
    case UMS_DataPackage::LISTUSERS:
        return createListUsers();
    case UMS_DataPackage::COMMAND:
        return createCommand();
    case UMS_DataPackage::LISTCOMMANDS:
        return createListCommands();
    case UMS_DataPackage::LISTCMDOPTIONS:
        return createListCmdOptions();
    case UMS_DataPackage::LOCALACCOUNT:
        return createLocalAccount();
    case UMS_DataPackage::LISTLOCALACCOUNTS:
        return createListLocalAccounts();
    case UMS_DataPackage::LISTLOCALACCOPTIONS:
        return createListLocalAccOptions();
    case UMS_DataPackage::MACHINE:
        return createMachine();
    case UMS_DataPackage::LISTMACHINES:
        return createListMachines();
    case UMS_DataPackage::LISTMACHINEOPTIONS:
        return createListMachineOptions();
    case UMS_DataPackage::CONFIGURATION:
        return createConfiguration();
    case UMS_DataPackage::OPTIONVALUE:
        return createOptionValue();
    case UMS_DataPackage::LISTOPTIONSVALUES:
        return createListOptionsValues();
    case UMS_DataPackage::LISTOPTOPTIONS:
        return createListOptOptions();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject UMS_DataFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const& _literalValue)
{
    switch (_eDataType->getClassifierID())
    {
    case UMS_DataPackage::SESSIONCLOSETYPE:
    {
        ::ecore::EJavaObject _any;
        UMS_DataPackage_ptr _epkg =
                dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (getEPackage());
        return _epkg->getSessionCloseType()->getEEnumLiteralByLiteral(
                _literalValue)->getValue();
    }
    case UMS_DataPackage::PRIVILEGETYPE:
    {
        ::ecore::EJavaObject _any;
        UMS_DataPackage_ptr _epkg =
                dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (getEPackage());
        return _epkg->getPrivilegeType()->getEEnumLiteralByLiteral(
                _literalValue)->getValue();
    }
    case UMS_DataPackage::STATUSTYPE:
    {
        ::ecore::EJavaObject _any;
        UMS_DataPackage_ptr _epkg =
                dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (getEPackage());
        return _epkg->getStatusType()->getEEnumLiteralByLiteral(_literalValue)->getValue();
    }
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString UMS_DataFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const& _instanceValue)
{
    switch (_eDataType->getClassifierID())
    {
    case UMS_DataPackage::SESSIONCLOSETYPE:
    {
        UMS_DataPackage_ptr _epkg = ::UMS_Data::instanceOf<
                ::UMS_Data::UMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getSessionCloseType()->getEEnumLiteral(_value)->getName();
    }
    case UMS_DataPackage::PRIVILEGETYPE:
    {
        UMS_DataPackage_ptr _epkg = ::UMS_Data::instanceOf<
                ::UMS_Data::UMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getPrivilegeType()->getEEnumLiteral(_value)->getName();
    }
    case UMS_DataPackage::STATUSTYPE:
    {
        UMS_DataPackage_ptr _epkg = ::UMS_Data::instanceOf<
                ::UMS_Data::UMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getStatusType()->getEEnumLiteral(_value)->getName();
    }
    default:
        throw "IllegalArgumentException";
    }
}

ConnectOptions_ptr UMS_DataFactory::createConnectOptions()
{
    return new ConnectOptions();
}
Session_ptr UMS_DataFactory::createSession()
{
    return new Session();
}
ListSessions_ptr UMS_DataFactory::createListSessions()
{
    return new ListSessions();
}
ListSessionOptions_ptr UMS_DataFactory::createListSessionOptions()
{
    return new ListSessionOptions();
}
User_ptr UMS_DataFactory::createUser()
{
    return new User();
}
ListUsers_ptr UMS_DataFactory::createListUsers()
{
    return new ListUsers();
}
Command_ptr UMS_DataFactory::createCommand()
{
    return new Command();
}
ListCommands_ptr UMS_DataFactory::createListCommands()
{
    return new ListCommands();
}
ListCmdOptions_ptr UMS_DataFactory::createListCmdOptions()
{
    return new ListCmdOptions();
}
LocalAccount_ptr UMS_DataFactory::createLocalAccount()
{
    return new LocalAccount();
}
ListLocalAccounts_ptr UMS_DataFactory::createListLocalAccounts()
{
    return new ListLocalAccounts();
}
ListLocalAccOptions_ptr UMS_DataFactory::createListLocalAccOptions()
{
    return new ListLocalAccOptions();
}
Machine_ptr UMS_DataFactory::createMachine()
{
    return new Machine();
}
ListMachines_ptr UMS_DataFactory::createListMachines()
{
    return new ListMachines();
}
ListMachineOptions_ptr UMS_DataFactory::createListMachineOptions()
{
    return new ListMachineOptions();
}
Configuration_ptr UMS_DataFactory::createConfiguration()
{
    return new Configuration();
}
OptionValue_ptr UMS_DataFactory::createOptionValue()
{
    return new OptionValue();
}
ListOptionsValues_ptr UMS_DataFactory::createListOptionsValues()
{
    return new ListOptionsValues();
}
ListOptOptions_ptr UMS_DataFactory::createListOptOptions()
{
    return new ListOptOptions();
}

