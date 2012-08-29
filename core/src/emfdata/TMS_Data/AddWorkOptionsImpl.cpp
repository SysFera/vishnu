// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/AddWorkOptionsImpl.cpp
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

#include "AddWorkOptions.hpp"
#include <TMS_Data/TMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::TMS_Data;

/*PROTECTED REGION ID(AddWorkOptionsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void AddWorkOptions::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(AddWorkOptionsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject AddWorkOptions::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__APPLICATIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_applicationId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__SUBJECT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_subject);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__PRIORITY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any,
                m_priority);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__OWNER:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_owner);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__ESTIMATEDHOUR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_estimatedHour);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__DESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_description);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__PROJECTID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_projectId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_machineId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__NBCPU:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any, m_nbCPU);
    }
        return _any;

    }
    throw "Error";
}

void AddWorkOptions::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__APPLICATIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_applicationId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__SUBJECT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_subject);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__PRIORITY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_priority);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__OWNER:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_owner);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__ESTIMATEDHOUR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_estimatedHour);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__DESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_description);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__PROJECTID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_projectId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_machineId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__NBCPU:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_nbCPU);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean AddWorkOptions::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__APPLICATIONID:
        return m_applicationId != "";
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__SUBJECT:
        return m_subject != "";
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__PRIORITY:
        return m_priority != -1;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__OWNER:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_owner);
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__ESTIMATEDHOUR:
        return m_estimatedHour != -1;
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__DESCRIPTION:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_description);
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__PROJECTID:
        return m_projectId != "";
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__MACHINEID:
        return m_machineId != "";
    case ::TMS_Data::TMS_DataPackage::ADDWORKOPTIONS__NBCPU:
        return m_nbCPU != -1;

    }
    throw "Error";
}

void AddWorkOptions::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr AddWorkOptions::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (::TMS_Data::TMS_DataPackage::_instance())->getAddWorkOptions();
    return _eclass;
}

