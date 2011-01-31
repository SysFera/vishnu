// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ConnectOptionsImpl.cpp
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

#include "ConnectOptions.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(ConnectOptionsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void ConnectOptions::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(ConnectOptionsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject ConnectOptions::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::CONNECTOPTIONS__CLOSEPOLICY:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::SessionCloseType >::toAny(
                _any, m_closePolicy);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::CONNECTOPTIONS__SESSIONINACTIVITYDELAY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any,
                m_sessionInactivityDelay);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::CONNECTOPTIONS__SUBSTITUTEUSERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_substituteUserId);
    }
        return _any;

    }
    throw "Error";
}

void ConnectOptions::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::CONNECTOPTIONS__CLOSEPOLICY:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::SessionCloseType >::fromAny(
                _newValue, m_closePolicy);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::CONNECTOPTIONS__SESSIONINACTIVITYDELAY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_sessionInactivityDelay);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::CONNECTOPTIONS__SUBSTITUTEUSERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_substituteUserId);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean ConnectOptions::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::CONNECTOPTIONS__CLOSEPOLICY:
        return m_closePolicy != 0;
    case ::UMS_Data::UMS_DataPackage::CONNECTOPTIONS__SESSIONINACTIVITYDELAY:
        return m_sessionInactivityDelay != 0;
    case ::UMS_Data::UMS_DataPackage::CONNECTOPTIONS__SUBSTITUTEUSERID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_substituteUserId);

    }
    throw "Error";
}

void ConnectOptions::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr ConnectOptions::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getConnectOptions();
    return _eclass;
}

