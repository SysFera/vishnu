// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListMachineOptionsImpl.cpp
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

#include "ListMachineOptions.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(ListMachineOptionsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void ListMachineOptions::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(ListMachineOptionsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject ListMachineOptions::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTMACHINEOPTIONS__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_userId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTMACHINEOPTIONS__LISTALLMACHINE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::toAny(_any,
                m_listAllmachine);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTMACHINEOPTIONS__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_machineId);
    }
        return _any;

    }
    throw "Error";
}

void ListMachineOptions::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTMACHINEOPTIONS__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_userId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTMACHINEOPTIONS__LISTALLMACHINE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::fromAny(
                _newValue, m_listAllmachine);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTMACHINEOPTIONS__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_machineId);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean ListMachineOptions::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTMACHINEOPTIONS__USERID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_userId);
    case ::UMS_Data::UMS_DataPackage::LISTMACHINEOPTIONS__LISTALLMACHINE:
        return m_listAllmachine != false;
    case ::UMS_Data::UMS_DataPackage::LISTMACHINEOPTIONS__MACHINEID:
        return m_machineId != "";

    }
    throw "Error";
}

void ListMachineOptions::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr ListMachineOptions::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getListMachineOptions();
    return _eclass;
}

