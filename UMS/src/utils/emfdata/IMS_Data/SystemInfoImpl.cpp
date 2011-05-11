// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/SystemInfoImpl.cpp
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

#include "SystemInfo.hpp"
#include <IMS_Data/IMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::IMS_Data;

/*PROTECTED REGION ID(SystemInfoImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void SystemInfo::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(SystemInfoImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject SystemInfo::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::SYSTEMINFO__MEMORY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any, m_memory);
    }
        return _any;
    case ::IMS_Data::IMS_DataPackage::SYSTEMINFO__DISKSPACE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_diskSpace);
    }
        return _any;
    case ::IMS_Data::IMS_DataPackage::SYSTEMINFO__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_machineId);
    }
        return _any;

    }
    throw "Error";
}

void SystemInfo::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::SYSTEMINFO__MEMORY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_memory);
    }
        return;
    case ::IMS_Data::IMS_DataPackage::SYSTEMINFO__DISKSPACE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_diskSpace);
    }
        return;
    case ::IMS_Data::IMS_DataPackage::SYSTEMINFO__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_machineId);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean SystemInfo::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::SYSTEMINFO__MEMORY:
        return m_memory != 0;
    case ::IMS_Data::IMS_DataPackage::SYSTEMINFO__DISKSPACE:
        return m_diskSpace != 0;
    case ::IMS_Data::IMS_DataPackage::SYSTEMINFO__MACHINEID:
        return m_machineId != "";

    }
    throw "Error";
}

void SystemInfo::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr SystemInfo::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::IMS_Data::IMS_DataPackage_ptr > (::IMS_Data::IMS_DataPackage::_instance())->getSystemInfo();
    return _eclass;
}

