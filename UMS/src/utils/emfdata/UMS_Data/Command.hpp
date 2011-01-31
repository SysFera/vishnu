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

    class Command: public virtual ::ecore::EObject

    {
    public:
        Command();

        virtual ~Command();

        virtual void _initialize();

        // Operations


        // Attributes
        ::ecore::EString const& getCommandId() const;
        void setCommandId(::ecore::EString const& _commandId);

        ::ecore::EString const& getSessionId() const;
        void setSessionId(::ecore::EString const& _sessionId);

        ::ecore::EString const& getMachineId() const;
        void setMachineId(::ecore::EString const& _machineId);

        ::ecore::EString const& getCmdDescription() const;
        void setCmdDescription(::ecore::EString const& _cmdDescription);

        ::ecore::ELong getCmdStartTime() const;
        void setCmdStartTime(::ecore::ELong _cmdStartTime);

        ::ecore::ELong getCmdEndTime() const;
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
