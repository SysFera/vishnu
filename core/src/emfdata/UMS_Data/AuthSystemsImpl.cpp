// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/AuthSystemsImpl.cpp
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

#include "AuthSystems.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <UMS_Data/AuthSystemsOptions.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(AuthSystemsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void AuthSystems::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(AuthSystemsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject AuthSystems::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__AUTHSYSTEMID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_authSystemId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__NAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_name);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__URI:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_URI);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__AUTHLOGIN:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_authLogin);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__AUTHPASSWORD:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_authPassword);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__USERPASSWORDENCRYPTION:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::EncryptionMethod >::toAny(
                _any, m_userPasswordEncryption);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__TYPE:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::AuthType >::toAny(_any,
                m_type);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::StatusType >::toAny(_any,
                m_status);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__OPTIONS:
    {
        _any = static_cast< ::ecore::EObject* > (m_options);
    }
        return _any;

    }
    throw "Error";
}

void AuthSystems::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__AUTHSYSTEMID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_authSystemId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__NAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_name);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__URI:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_URI);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__AUTHLOGIN:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_authLogin);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__AUTHPASSWORD:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_authPassword);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__USERPASSWORDENCRYPTION:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::EncryptionMethod >::fromAny(
                _newValue, m_userPasswordEncryption);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__TYPE:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::AuthType >::fromAny(
                _newValue, m_type);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::UMS_Data::StatusType >::fromAny(
                _newValue, m_status);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__OPTIONS:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EObject_ptr >(_newValue);
        ::UMS_Data::AuthSystemsOptions_ptr _t1 =
                dynamic_cast< ::UMS_Data::AuthSystemsOptions_ptr > (_t0);
        ::UMS_Data::AuthSystems::setOptions(_t1);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean AuthSystems::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__AUTHSYSTEMID:
        return m_authSystemId != "";
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__NAME:
        return m_name != "";
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__URI:
        return m_URI != "";
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__AUTHLOGIN:
        return m_authLogin != "";
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__AUTHPASSWORD:
        return m_authPassword != "";
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__USERPASSWORDENCRYPTION:
        return m_userPasswordEncryption != 0;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__TYPE:
        return m_type != 0;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__STATUS:
        return m_status != 1;
    case ::UMS_Data::UMS_DataPackage::AUTHSYSTEMS__OPTIONS:
        return m_options;

    }
    throw "Error";
}

void AuthSystems::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr AuthSystems::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getAuthSystems();
    return _eclass;
}

