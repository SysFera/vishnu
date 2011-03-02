// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/Command.hpp
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
 * \file Command.hpp
 * \brief The Command class
 * \author Generated file
 * \date 20/02/2011
 */

#ifndef UMS_DATA_COMMAND_HPP
#define UMS_DATA_COMMAND_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Command_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class Command
     * \brief Implementation of the Command class
     */
    class Command: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Command
         */
        Command();
        /**
         * \brief The destructor for Command
         */
        virtual ~Command();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the commandId
         * \return The commandId attribute value
         **/
        ::ecore::EString const& getCommandId() const;
        /**
         * \brief To set the commandId
         * \param _commandId The commandId value
         **/
        void setCommandId(::ecore::EString const& _commandId);

        /**
         * \brief To get the sessionId
         * \return The sessionId attribute value
         **/
        ::ecore::EString const& getSessionId() const;
        /**
         * \brief To set the sessionId
         * \param _sessionId The sessionId value
         **/
        void setSessionId(::ecore::EString const& _sessionId);

        /**
         * \brief To get the machineId
         * \return The machineId attribute value
         **/
        ::ecore::EString const& getMachineId() const;
        /**
         * \brief To set the machineId
         * \param _machineId The machineId value
         **/
        void setMachineId(::ecore::EString const& _machineId);

        /**
         * \brief To get the cmdDescription
         * \return The cmdDescription attribute value
         **/
        ::ecore::EString const& getCmdDescription() const;
        /**
         * \brief To set the cmdDescription
         * \param _cmdDescription The cmdDescription value
         **/
        void setCmdDescription(::ecore::EString const& _cmdDescription);

        /**
         * \brief To get the cmdStartTime
         * \return The cmdStartTime attribute value
         **/
        ::ecore::ELong getCmdStartTime() const;
        /**
         * \brief To set the cmdStartTime
         * \param _cmdStartTime The cmdStartTime value
         **/
        void setCmdStartTime(::ecore::ELong _cmdStartTime);

        /**
         * \brief To get the cmdEndTime
         * \return The cmdEndTime attribute value
         **/
        ::ecore::ELong getCmdEndTime() const;
        /**
         * \brief To set the cmdEndTime
         * \param _cmdEndTime The cmdEndTime value
         **/
        void setCmdEndTime(::ecore::ELong _cmdEndTime);

        // References


        /*PROTECTED REGION ID(Command) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

        // EObjectImpl
        virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID,
                ::ecore::EBoolean _resolve);
        virtual void eSet(::ecore::EInt _featureID,
                ::ecore::EJavaObject const& _newValue);
        virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID);
        virtual void eUnset(::ecore::EInt _featureID);
        virtual ::ecore::EClass_ptr _eClass();

        /*PROTECTED REGION ID(CommandImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_commandId;

        ::ecore::EString m_sessionId;

        ::ecore::EString m_machineId;

        ::ecore::EString m_cmdDescription;

        ::ecore::ELong m_cmdStartTime;

        ::ecore::ELong m_cmdEndTime;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_COMMAND_HPP
