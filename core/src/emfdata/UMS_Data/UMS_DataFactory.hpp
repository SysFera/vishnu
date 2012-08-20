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

/**
 * \file UMS_DataFactory.hpp
 * \brief The UMS_DataFactory class
 * \author Generated file
 * \date 31/03/2011
 */
#ifndef _UMS_DATAFACTORY_HPP
#define _UMS_DATAFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <UMS_Data.hpp>

namespace UMS_Data
{
    /**
     * \class UMS_DataFactory
     * \brief The factory class for generated model UMS_Data
     */
    class UMS_DataFactory: public virtual ::ecore::EFactory
    {
    public:

        /**
         * \brief To get the instance of the singleton
         * \return Pointer to the instance
         */
        static UMS_DataFactory_ptr _instance();

        /**
         * \brief Creates a new object of class ConnectOptions
         * \return Pointer to the object
         */
        virtual ConnectOptions_ptr createConnectOptions();
        /**
         * \brief Creates a new object of class Session
         * \return Pointer to the object
         */
        virtual Session_ptr createSession();
        /**
         * \brief Creates a new object of class ListSessions
         * \return Pointer to the object
         */
        virtual ListSessions_ptr createListSessions();
        /**
         * \brief Creates a new object of class ListSessionOptions
         * \return Pointer to the object
         */
        virtual ListSessionOptions_ptr createListSessionOptions();
        /**
         * \brief Creates a new object of class User
         * \return Pointer to the object
         */
        virtual User_ptr createUser();
        /**
         * \brief Creates a new object of class ListUsers
         * \return Pointer to the object
         */
        virtual ListUsers_ptr createListUsers();
        /**
         * \brief Creates a new object of class ListUsersOptions
         * \return Pointer to the object
         */
        virtual ListUsersOptions_ptr createListUsersOptions();
        /**
         * \brief Creates a new object of class Command
         * \return Pointer to the object
         */
        virtual Command_ptr createCommand();
        /**
         * \brief Creates a new object of class ListCommands
         * \return Pointer to the object
         */
        virtual ListCommands_ptr createListCommands();
        /**
         * \brief Creates a new object of class ListCmdOptions
         * \return Pointer to the object
         */
        virtual ListCmdOptions_ptr createListCmdOptions();
        /**
         * \brief Creates a new object of class LocalAccount
         * \return Pointer to the object
         */
        virtual LocalAccount_ptr createLocalAccount();
        /**
         * \brief Creates a new object of class ListLocalAccounts
         * \return Pointer to the object
         */
        virtual ListLocalAccounts_ptr createListLocalAccounts();
        /**
         * \brief Creates a new object of class ListLocalAccOptions
         * \return Pointer to the object
         */
        virtual ListLocalAccOptions_ptr createListLocalAccOptions();
        /**
         * \brief Creates a new object of class Machine
         * \return Pointer to the object
         */
        virtual Machine_ptr createMachine();
        /**
         * \brief Creates a new object of class ListMachines
         * \return Pointer to the object
         */
        virtual ListMachines_ptr createListMachines();
        /**
         * \brief Creates a new object of class ListMachineOptions
         * \return Pointer to the object
         */
        virtual ListMachineOptions_ptr createListMachineOptions();
        /**
         * \brief Creates a new object of class Configuration
         * \return Pointer to the object
         */
        virtual Configuration_ptr createConfiguration();
        /**
         * \brief Creates a new object of class OptionValue
         * \return Pointer to the object
         */
        virtual OptionValue_ptr createOptionValue();
        /**
         * \brief Creates a new object of class ListOptionsValues
         * \return Pointer to the object
         */
        virtual ListOptionsValues_ptr createListOptionsValues();
        /**
         * \brief Creates a new object of class ListOptOptions
         * \return Pointer to the object
         */
        virtual ListOptOptions_ptr createListOptOptions();
        /**
         * \brief Creates a new object of class AuthSystem
         * \return Pointer to the object
         */
        virtual AuthSystem_ptr createAuthSystem();
        /**
         * \brief Creates a new object of class ListAuthSystems
         * \return Pointer to the object
         */
        virtual ListAuthSystems_ptr createListAuthSystems();
        /**
         * \brief Creates a new object of class ListAuthSysOptions
         * \return Pointer to the object
         */
        virtual ListAuthSysOptions_ptr createListAuthSysOptions();
        /**
         * \brief Creates a new object of class AuthAccount
         * \return Pointer to the object
         */
        virtual AuthAccount_ptr createAuthAccount();
        /**
         * \brief Creates a new object of class ListAuthAccounts
         * \return Pointer to the object
         */
        virtual ListAuthAccounts_ptr createListAuthAccounts();
        /**
         * \brief Creates a new object of class ListAuthAccOptions
         * \return Pointer to the object
         */
        virtual ListAuthAccOptions_ptr createListAuthAccOptions();
        /**
         * \brief Creates a new object of class Version
         * \return Pointer to the object
         */
        virtual Version_ptr createVersion();

        /**
         * \brief To create an object of a given class (reflective API)
         * \param _eClass The class object to create (obtained using the Package class)
         * \return Generic pointer to the object
         */
        virtual ::ecore::EObject_ptr create(::ecore::EClass_ptr _eClass);

        /**
         * \brief To create an object from a string (deserialize)
         * \param _eDataType The datatype
         * \param _literalValue The string
         * \return Generic pointer to the object
         */
        virtual ::ecore::EJavaObject createFromString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EString const& _literalValue);

        /**
         * \brief To convert an object to a string (serialize)
         * \param _eDataType The datatype
         * \param _instanceValue Value of the instance
         * \return The object serialized in a string
         */
        virtual ::ecore::EString convertToString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EJavaObject const& _instanceValue);

    protected:

        /**
         * \brief The instance of the singleton
         */
        static std::auto_ptr< UMS_DataFactory > s_instance;

        /**
         * \brief Constructor
         */
        UMS_DataFactory();

    };

} // UMS_Data


#endif // _UMS_DATAFACTORY_HPP
