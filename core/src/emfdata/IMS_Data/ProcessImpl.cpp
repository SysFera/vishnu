// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/ProcessImpl.cpp
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

#include "Process.hpp"
#include <IMS_Data/IMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::IMS_Data;

/*PROTECTED REGION ID(ProcessImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void Process::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(ProcessImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject Process::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::PROCESS__PROCESSNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_processName);
    }
        return _any;
    case ::IMS_Data::IMS_DataPackage::PROCESS__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_machineId);
    }
        return _any;
    case ::IMS_Data::IMS_DataPackage::PROCESS__DIETID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_dietId);
    }
        return _any;
    case ::IMS_Data::IMS_DataPackage::PROCESS__STATE:
    {
        ::ecorecpp::mapping::any_traits< ::IMS_Data::ProcessState >::toAny(
                _any, m_state);
    }
        return _any;
    case ::IMS_Data::IMS_DataPackage::PROCESS__TIMESTAMP:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_timestamp);
    }
        return _any;
    case ::IMS_Data::IMS_DataPackage::PROCESS__SCRIPT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_script);
    }
        return _any;

    }
    throw "Error";
}

void Process::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::PROCESS__PROCESSNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_processName);
    }
        return;
    case ::IMS_Data::IMS_DataPackage::PROCESS__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_machineId);
    }
        return;
    case ::IMS_Data::IMS_DataPackage::PROCESS__DIETID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_dietId);
    }
        return;
    case ::IMS_Data::IMS_DataPackage::PROCESS__STATE:
    {
        ::ecorecpp::mapping::any_traits< ::IMS_Data::ProcessState >::fromAny(
                _newValue, m_state);
    }
        return;
    case ::IMS_Data::IMS_DataPackage::PROCESS__TIMESTAMP:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_timestamp);
    }
        return;
    case ::IMS_Data::IMS_DataPackage::PROCESS__SCRIPT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_script);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean Process::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::PROCESS__PROCESSNAME:
        return m_processName != "";
    case ::IMS_Data::IMS_DataPackage::PROCESS__MACHINEID:
        return m_machineId != "";
    case ::IMS_Data::IMS_DataPackage::PROCESS__DIETID:
        return m_dietId != "";
    case ::IMS_Data::IMS_DataPackage::PROCESS__STATE:
        return m_state != 0;
    case ::IMS_Data::IMS_DataPackage::PROCESS__TIMESTAMP:
        return m_timestamp != 0;
    case ::IMS_Data::IMS_DataPackage::PROCESS__SCRIPT:
        return m_script != "";

    }
    throw "Error";
}

void Process::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr Process::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::IMS_Data::IMS_DataPackage_ptr > (::IMS_Data::IMS_DataPackage::_instance())->getProcess();
    return _eclass;
}

