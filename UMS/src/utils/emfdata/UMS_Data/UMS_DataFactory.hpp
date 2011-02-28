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
 * \brief Factory for all UMS objects
 * \author Generated file
 * \date 31/01/2011
 */

#ifndef _UMS_DATAFACTORY_HPP
#define _UMS_DATAFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <UMS_Data.hpp>

namespace UMS_Data
{
    /**
     * \class UMS_DataFactory
     * \brief Factory for all UMS objects
     */
    class UMS_DataFactory: public virtual ::ecore::EFactory
    {
    public:
	/**
	 * \brief Getter of the factory (singleton)
	 * \fn static UMS_DataFactory_ptr _instance()
	 */
        static UMS_DataFactory_ptr _instance();

	/**
	 * \brief To get a connect option object pointer
	 * \fn virtual ConnectOptions_ptr createConnectOptions()
	 */
        virtual ConnectOptions_ptr createConnectOptions();
	/**
	 * \brief To get a session object pointer
	 * \fn virtual Session_ptr createSession()
	 */
        virtual Session_ptr createSession();
	/**
	 * \brief To get a list session object pointer
	 * \fn virtual ListSessions_ptr createListSessions()
	 */
        virtual ListSessions_ptr createListSessions();
	/**
	 * \brief To get a list session option object pointer
	 * \fn virtual ListSessionOptions_ptr ListSessionOptions()
	 */
        virtual ListSessionOptions_ptr createListSessionOptions();
	/**
	 * \brief To get a user object pointer
	 * \fn virtual User_ptr createUser()
	 */
        virtual User_ptr createUser();
	/**
	 * \brief To get a  list user object pointer
	 * \fn virtual ListUsers_ptr createListUsers()
	 */
        virtual ListUsers_ptr createListUsers();
	/**
	 * \brief To get a command object pointer
	 * \fn virtual Command_ptr createCommand()
	 */
        virtual Command_ptr createCommand();
	/**
	 * \brief To get a list command option object pointer
	 * \fn virtual ListCommands_ptr createListCommands()
	 */
        virtual ListCommands_ptr createListCommands();
	/**
	 * \brief To get a list command option object pointer
	 * \fn virtual ListCmdOptions_ptr createListCmdOptions()
	 */
        virtual ListCmdOptions_ptr createListCmdOptions();
	/**
	 * \brief To get a local accounr object pointer
	 * \fn virtual LocalAccount_ptr createLocalAccount()
	 */
        virtual LocalAccount_ptr createLocalAccount();
	/**
	 * \brief To get a list local account object pointer
	 * \fn virtual ListLocalAccounts_ptr createListLocalAccounts()
	 */
        virtual ListLocalAccounts_ptr createListLocalAccounts();
	/**
	 * \brief To get a list local option object pointer
	 * \fn virtual ListLocalAccOptions_ptr createListLocalAccOptions()
	 */
        virtual ListLocalAccOptions_ptr createListLocalAccOptions();
	/**
	 * \brief To get a machine object pointer
	 * \fn virtual Machine_ptr createMachine()
	 */
        virtual Machine_ptr createMachine();
	/**
	 * \brief To get a list machine object pointer
	 * \fn virtual ListMachines_ptr createListMachines()
	 */
        virtual ListMachines_ptr createListMachines();
	/**
	 * \brief To get a list machine option object pointer
	 * \fn virtual ListMachineOptions_ptr createListMachineOptions()
	 */
        virtual ListMachineOptions_ptr createListMachineOptions();
	/**
	 * \brief To get a configuration object pointer
	 * \fn virtual Configuration_ptr createConfiguration()
	 */
        virtual Configuration_ptr createConfiguration();
	/**
	 * \brief To get an option value object pointer
	 * \fn virtual OptionValue_ptr createOptionValue()
	 */
        virtual OptionValue_ptr createOptionValue();
	/**
	 * \brief To get a list option value object pointer
	 * \fn virtual ListOptionsValues_ptr createListOptionsValues()
	 */
        virtual ListOptionsValues_ptr createListOptionsValues();
	/**
	 * \brief To get a list option of option object pointer
	 * \fn virtual ListOptOptions_ptr createListOptOptions()
	 */
        virtual ListOptOptions_ptr createListOptOptions();

	/**
	 * \brief To create an object of a class
	 * \fn virtual ::ecore::EObject_ptr create(::ecore::EClass_ptr _eClass)
	 * \param _eClass The class type to create
	 * \return The created object
	 */
        virtual ::ecore::EObject_ptr create(::ecore::EClass_ptr _eClass);
	/**
	 * \brief To create an object from a string
	 * \fn virtual ::ecore::EJavaObject createFromString(
	 *             ::ecore::EDataType_ptr _eDataType,
         *             ::ecore::EString const& _literalValue)
	 * \param _eDataType Datatype
	 * \param _literalString string
	 * \return The created object
	 */
        virtual ::ecore::EJavaObject createFromString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EString const& _literalValue);
	/**
	 * \brief To convert an object to a string
	 * \fn virtual ::ecore::EString convertToString(
	 *             ::ecore::EDataType_ptr _eDataType,
         *             ::ecore::EJavaObject const& _instanceValue)
	 * \param _eDataType Datatype
	 * \param _instanceValue Value of the instance
	 * \return The object as a string
	 */
        virtual ::ecore::EString convertToString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EJavaObject const& _instanceValue);

    protected:

	/**
	 * \brief The unique instance of the singleton
	 */
        static std::auto_ptr< UMS_DataFactory > s_instance;

	/**
	 * \brief Constructor
	 * \fn UMS_DataFactory()
	 */
        UMS_DataFactory();

    };

} // UMS_Data


#endif // _UMS_DATAFACTORY_HPP
