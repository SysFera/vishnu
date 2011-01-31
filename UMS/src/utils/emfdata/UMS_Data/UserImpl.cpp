// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/UserImpl.cpp
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

#include "User.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(UserImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void User::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(UserImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject User::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::USER__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_userId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::USER__PASSWORD:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_password);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::USER__FIRSTNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_firstname);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::USER__LASTNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_lastname);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::USER__PRIVILEGE:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::PrivilegeType >::toAny(
                _any, m_privilege);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::USER__EMAIL:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_email);
    }
        return _any;

    }
    throw "Error";
}

void User::eSet(::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::USER__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_userId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::USER__PASSWORD:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_password);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::USER__FIRSTNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_firstname);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::USER__LASTNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_lastname);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::USER__PRIVILEGE:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::PrivilegeType >::fromAny(
                _newValue, m_privilege);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::USER__EMAIL:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_email);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean User::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::USER__USERID:
        return m_userId != "";
    case ::UMS_Data::UMS_DataPackage::USER__PASSWORD:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_password);
    case ::UMS_Data::UMS_DataPackage::USER__FIRSTNAME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_firstname);
    case ::UMS_Data::UMS_DataPackage::USER__LASTNAME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_lastname);
    case ::UMS_Data::UMS_DataPackage::USER__PRIVILEGE:
        return m_privilege != -1;
    case ::UMS_Data::UMS_DataPackage::USER__EMAIL:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_email);

    }
    throw "Error";
}

void User::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr User::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getUser();
    return _eclass;
}

