// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * AMS_Data/ApplicationImpl.cpp
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

#include "Application.hpp"
#include <AMS_Data/AMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::AMS_Data;

/*PROTECTED REGION ID(ApplicationImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void Application::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(ApplicationImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject Application::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::AMS_Data::AMS_DataPackage::APPLICATION__PROFILE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_profile);
    }
        return _any;
    case ::AMS_Data::AMS_DataPackage::APPLICATION__DESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_description);
    }
        return _any;
    case ::AMS_Data::AMS_DataPackage::APPLICATION__INPUT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_input);
    }
        return _any;
    case ::AMS_Data::AMS_DataPackage::APPLICATION__OUTPUT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_output);
    }
        return _any;

    }
    throw "Error";
}

void Application::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::AMS_Data::AMS_DataPackage::APPLICATION__PROFILE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_profile);
    }
        return;
    case ::AMS_Data::AMS_DataPackage::APPLICATION__DESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_description);
    }
        return;
    case ::AMS_Data::AMS_DataPackage::APPLICATION__INPUT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_input);
    }
        return;
    case ::AMS_Data::AMS_DataPackage::APPLICATION__OUTPUT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_output);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean Application::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::AMS_Data::AMS_DataPackage::APPLICATION__PROFILE:
        return m_profile != "";
    case ::AMS_Data::AMS_DataPackage::APPLICATION__DESCRIPTION:
        return m_description != "";
    case ::AMS_Data::AMS_DataPackage::APPLICATION__INPUT:
        return m_input != "";
    case ::AMS_Data::AMS_DataPackage::APPLICATION__OUTPUT:
        return m_output != "";

    }
    throw "Error";
}

void Application::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr Application::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::AMS_Data::AMS_DataPackage_ptr > (::AMS_Data::AMS_DataPackage::_instance())->getApplication();
    return _eclass;
}

