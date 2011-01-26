// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/CommandImpl.cpp
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

#include "Command.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(CommandImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void Command::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(CommandImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject Command::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::COMMAND__COMMANDID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_commandId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::COMMAND__SESSIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_sessionId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::COMMAND__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_machineId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::COMMAND__CMDDESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_cmdDescription);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::COMMAND__CMDSTARTTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_cmdStartTime);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::COMMAND__CMDENDTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_cmdEndTime);
    }
        return _any;

    }
    throw "Error";
}

void Command::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::COMMAND__COMMANDID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_commandId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::COMMAND__SESSIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_sessionId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::COMMAND__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_machineId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::COMMAND__CMDDESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_cmdDescription);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::COMMAND__CMDSTARTTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_cmdStartTime);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::COMMAND__CMDENDTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_cmdEndTime);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean Command::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::COMMAND__COMMANDID:
        return m_commandId != "";
    case ::UMS_Data::UMS_DataPackage::COMMAND__SESSIONID:
        return m_sessionId != "";
    case ::UMS_Data::UMS_DataPackage::COMMAND__MACHINEID:
        return m_machineId != "";
    case ::UMS_Data::UMS_DataPackage::COMMAND__CMDDESCRIPTION:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_cmdDescription);
    case ::UMS_Data::UMS_DataPackage::COMMAND__CMDSTARTTIME:
        return m_cmdStartTime != -1;
    case ::UMS_Data::UMS_DataPackage::COMMAND__CMDENDTIME:
        return m_cmdEndTime != -1;

    }
    throw "Error";
}

void Command::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr Command::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getCommand();
    return _eclass;
}

