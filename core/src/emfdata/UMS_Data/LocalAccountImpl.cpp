// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/LocalAccountImpl.cpp
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

#include "LocalAccount.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(LocalAccountImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void LocalAccount::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(LocalAccountImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject LocalAccount::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_userId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_machineId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__ACLOGIN:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_acLogin);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__SSHKEYPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_sshKeyPath);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__HOMEDIRECTORY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_homeDirectory);
    }
        return _any;

    }
    throw "Error";
}

void LocalAccount::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_userId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_machineId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__ACLOGIN:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_acLogin);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__SSHKEYPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_sshKeyPath);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__HOMEDIRECTORY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_homeDirectory);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean LocalAccount::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__USERID:
        return m_userId != "";
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__MACHINEID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_machineId);
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__ACLOGIN:
        return m_acLogin != "";
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__SSHKEYPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_sshKeyPath);
    case ::UMS_Data::UMS_DataPackage::LOCALACCOUNT__HOMEDIRECTORY:
        return m_homeDirectory != "";

    }
    throw "Error";
}

void LocalAccount::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr LocalAccount::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getLocalAccount();
    return _eclass;
}

