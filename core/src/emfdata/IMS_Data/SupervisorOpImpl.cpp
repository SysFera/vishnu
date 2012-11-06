// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/SupervisorOpImpl.cpp
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

#include "SupervisorOp.hpp"
#include <IMS_Data/IMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::IMS_Data;

/*PROTECTED REGION ID(SupervisorOpImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void SupervisorOp::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(SupervisorOpImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject SupervisorOp::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::SUPERVISOROP__URI:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_URI);
    }
        return _any;
    case ::IMS_Data::IMS_DataPackage::SUPERVISOROP__NAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_name);
    }
        return _any;

    }
    throw "Error";
}

void SupervisorOp::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::SUPERVISOROP__URI:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_URI);
    }
        return;
    case ::IMS_Data::IMS_DataPackage::SUPERVISOROP__NAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_name);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean SupervisorOp::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::SUPERVISOROP__URI:
        return m_URI != "";
    case ::IMS_Data::IMS_DataPackage::SUPERVISOROP__NAME:
        return m_name != "";

    }
    throw "Error";
}

void SupervisorOp::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr SupervisorOp::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::IMS_Data::IMS_DataPackage_ptr > (::IMS_Data::IMS_DataPackage::_instance())->getSupervisorOp();
    return _eclass;
}

