// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/AuthSystemImpl.cpp
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

#include "AuthSystem.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(AuthSystemImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void AuthSystem::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(AuthSystemImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject AuthSystem::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__AUTHSYSTEMID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_authSystemId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__NAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_name);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__URI:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_URI);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__AUTHLOGIN:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_authLogin);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__AUTHPASSWORD:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_authPassword);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__USERPASSWORDENCRYPTION:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::EncryptionMethod >::toAny(
                _any, m_userPasswordEncryption);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__TYPE:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::AuthType >::toAny(_any,
                m_type);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::StatusType >::toAny(_any,
                m_status);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__LDAPBASE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_ldapBase);
    }
        return _any;

    }
    throw "Error";
}

void AuthSystem::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__AUTHSYSTEMID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_authSystemId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__NAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_name);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__URI:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_URI);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__AUTHLOGIN:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_authLogin);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__AUTHPASSWORD:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_authPassword);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__USERPASSWORDENCRYPTION:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::EncryptionMethod >::fromAny(
                _newValue, m_userPasswordEncryption);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__TYPE:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::AuthType >::fromAny(
                _newValue, m_type);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::StatusType >::fromAny(
                _newValue, m_status);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__LDAPBASE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_ldapBase);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean AuthSystem::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__AUTHSYSTEMID:
        return m_authSystemId != "";
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__NAME:
        return m_name != "";
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__URI:
        return m_URI != "";
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__AUTHLOGIN:
        return m_authLogin != "";
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__AUTHPASSWORD:
        return m_authPassword != "";
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__USERPASSWORDENCRYPTION:
        return m_userPasswordEncryption != 0;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__TYPE:
        return m_type != 0;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__STATUS:
        return m_status != 1;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEM__LDAPBASE:
        return m_ldapBase != "";

    }
    throw "Error";
}

void AuthSystem::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr AuthSystem::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getAuthSystem();
    return _eclass;
}

