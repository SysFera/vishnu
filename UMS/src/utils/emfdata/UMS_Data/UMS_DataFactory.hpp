// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/UMS_DataFactory.hpp
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

#ifndef _UMS_DATAFACTORY_HPP
#define _UMS_DATAFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <UMS_Data.hpp>

namespace UMS_Data
{

    class UMS_DataFactory: public virtual ::ecore::EFactory
    {
    public:

        static UMS_DataFactory_ptr _instance();

        virtual ConnectOptions_ptr createConnectOptions();
        virtual Session_ptr createSession();
        virtual ListSessions_ptr createListSessions();
        virtual ListSessionOptions_ptr createListSessionOptions();
        virtual User_ptr createUser();
        virtual ListUsers_ptr createListUsers();
        virtual Command_ptr createCommand();
        virtual ListCommands_ptr createListCommands();
        virtual ListCmdOptions_ptr createListCmdOptions();
        virtual LocalAccount_ptr createLocalAccount();
        virtual ListLocalAccounts_ptr createListLocalAccounts();
        virtual ListLocalAccOptions_ptr createListLocalAccOptions();
        virtual Machine_ptr createMachine();
        virtual ListMachines_ptr createListMachines();
        virtual ListMachineOptions_ptr createListMachineOptions();
        virtual Configuration_ptr createConfiguration();
        virtual OptionValue_ptr createOptionValue();
        virtual ListOptionsValues_ptr createListOptionsValues();
        virtual ListOptOptions_ptr createListOptOptions();

        virtual ::ecore::EObject_ptr create(::ecore::EClass_ptr _eClass);
        virtual ::ecore::EJavaObject createFromString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EString const& _literalValue);
        virtual ::ecore::EString convertToString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EJavaObject const& _instanceValue);

    protected:

        static std::auto_ptr< UMS_DataFactory > s_instance;

        UMS_DataFactory();

    };

} // UMS_Data


#endif // _UMS_DATAFACTORY_HPP
