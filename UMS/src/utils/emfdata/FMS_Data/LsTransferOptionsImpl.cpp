// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/LsTransferOptionsImpl.cpp
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

#include "LsTransferOptions.hpp"
#include <FMS_Data/FMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::FMS_Data;

/*PROTECTED REGION ID(LsTransferOptionsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void LsTransferOptions::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(LsTransferOptionsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject LsTransferOptions::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__TRANSFERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_transferId);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__FROMMACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_fromMachineId);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_userId);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::FMS_Data::Status >::toAny(_any,
                m_status);
    }
        return _any;

    }
    throw "Error";
}

void LsTransferOptions::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__TRANSFERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_transferId);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__FROMMACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_fromMachineId);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_userId);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::FMS_Data::Status >::fromAny(
                _newValue, m_status);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean LsTransferOptions::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__TRANSFERID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_transferId);
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__FROMMACHINEID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_fromMachineId);
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__USERID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_userId);
    case ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__STATUS:
        return m_status != 4;

    }
    throw "Error";
}

void LsTransferOptions::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr LsTransferOptions::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::FMS_Data::FMS_DataPackage_ptr > (::FMS_Data::FMS_DataPackage::_instance())->getLsTransferOptions();
    return _eclass;
}

