// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListSessionOptionsImpl.cpp
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

#include "ListSessionOptions.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(ListSessionOptionsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void ListSessionOptions::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(ListSessionOptionsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject ListSessionOptions::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONLISTOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::SessionStateType >::toAny(
                _any, m_sessionListOption);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONCLOSEPOLICY:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::SessionCloseType >::toAny(
                _any, m_sessionClosePolicy);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONINACTIVITYDELAY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any,
                m_sessionInactivityDelay);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_machineId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__ADMINLISTOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::toAny(_any,
                m_adminListOption);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_userId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_sessionId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__STARTDATEOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_startDateOption);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__ENDDATEOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_endDateOption);
    }
        return _any;

    }
    throw "Error";
}

void ListSessionOptions::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONLISTOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::SessionStateType >::fromAny(
                _newValue, m_sessionListOption);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONCLOSEPOLICY:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::SessionCloseType >::fromAny(
                _newValue, m_sessionClosePolicy);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONINACTIVITYDELAY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_sessionInactivityDelay);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_machineId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__ADMINLISTOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::fromAny(
                _newValue, m_adminListOption);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_userId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_sessionId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__STARTDATEOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_startDateOption);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__ENDDATEOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_endDateOption);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean ListSessionOptions::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONLISTOPTION:
        return m_sessionListOption != 1;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONCLOSEPOLICY:
        return m_sessionClosePolicy != 0;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONINACTIVITYDELAY:
        return m_sessionInactivityDelay != 0;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__MACHINEID:
        return m_machineId != "";
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__ADMINLISTOPTION:
        return m_adminListOption != false;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__USERID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_userId);
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__SESSIONID:
        return m_sessionId != "";
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__STARTDATEOPTION:
        return m_startDateOption != -1;
    case ::UMS_Data::UMS_DataPackage::LISTSESSIONOPTIONS__ENDDATEOPTION:
        return m_endDateOption != -1;

    }
    throw "Error";
}

void ListSessionOptions::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr ListSessionOptions::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getListSessionOptions();
    return _eclass;
}

