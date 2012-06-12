// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/WorkImpl.cpp
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

#include "Work.hpp"
#include <TMS_Data/TMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::TMS_Data;

/*PROTECTED REGION ID(WorkImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void Work::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(WorkImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject Work::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::WORK__SESSIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_sessionId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__APPLICATIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_applicationId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__SUBJECT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_subject);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__PRIORITY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any,
                m_priority);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any, m_status);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__ENDDATE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_endDate);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__OWNER:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_owner);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__ESTIMATEDHOUR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_estimatedHour);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__DONERATIO:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any,
                m_doneRatio);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__DESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_description);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__DATECREATED:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_dateCreated);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__DATEENDED:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_dateEnded);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__DATESTARTED:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_dateStarted);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__LASTUPDATED:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_lastUpdated);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__WORKID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_workId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__PROJECTID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_projectId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__SUBMITDATE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_submitDate);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_machineId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__NBCPU:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any, m_nbCPU);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::WORK__DUEDATE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_dueDate);
    }
        return _any;

    }
    throw "Error";
}

void Work::eSet(::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::WORK__SESSIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_sessionId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__APPLICATIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_applicationId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__SUBJECT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_subject);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__PRIORITY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_priority);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_status);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__ENDDATE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_endDate);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__OWNER:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_owner);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__ESTIMATEDHOUR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_estimatedHour);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__DONERATIO:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_doneRatio);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__DESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_description);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__DATECREATED:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_dateCreated);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__DATEENDED:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_dateEnded);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__DATESTARTED:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_dateStarted);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__LASTUPDATED:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_lastUpdated);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__WORKID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_workId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__PROJECTID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_projectId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__SUBMITDATE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_submitDate);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__MACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_machineId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__NBCPU:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_nbCPU);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::WORK__DUEDATE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_dueDate);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean Work::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::WORK__SESSIONID:
        return m_sessionId != "";
    case ::TMS_Data::TMS_DataPackage::WORK__APPLICATIONID:
        return m_applicationId != "";
    case ::TMS_Data::TMS_DataPackage::WORK__SUBJECT:
        return m_subject != "";
    case ::TMS_Data::TMS_DataPackage::WORK__PRIORITY:
        return m_priority != -1;
    case ::TMS_Data::TMS_DataPackage::WORK__STATUS:
        return m_status != -1;
    case ::TMS_Data::TMS_DataPackage::WORK__ENDDATE:
        return m_endDate != -1;
    case ::TMS_Data::TMS_DataPackage::WORK__OWNER:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_owner);
    case ::TMS_Data::TMS_DataPackage::WORK__ESTIMATEDHOUR:
        return m_estimatedHour != -1;
    case ::TMS_Data::TMS_DataPackage::WORK__DONERATIO:
        return m_doneRatio != 0;
    case ::TMS_Data::TMS_DataPackage::WORK__DESCRIPTION:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_description);
    case ::TMS_Data::TMS_DataPackage::WORK__DATECREATED:
        return m_dateCreated != -1;
    case ::TMS_Data::TMS_DataPackage::WORK__DATEENDED:
        return m_dateEnded != -1;
    case ::TMS_Data::TMS_DataPackage::WORK__DATESTARTED:
        return m_dateStarted != -1;
    case ::TMS_Data::TMS_DataPackage::WORK__LASTUPDATED:
        return m_lastUpdated != -1;
    case ::TMS_Data::TMS_DataPackage::WORK__WORKID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_workId);
    case ::TMS_Data::TMS_DataPackage::WORK__PROJECTID:
        return m_projectId != "";
    case ::TMS_Data::TMS_DataPackage::WORK__SUBMITDATE:
        return m_submitDate != 0;
    case ::TMS_Data::TMS_DataPackage::WORK__MACHINEID:
        return m_machineId != "";
    case ::TMS_Data::TMS_DataPackage::WORK__NBCPU:
        return m_nbCPU != -1;
    case ::TMS_Data::TMS_DataPackage::WORK__DUEDATE:
        return m_dueDate != -1;

    }
    throw "Error";
}

void Work::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr Work::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (::TMS_Data::TMS_DataPackage::_instance())->getWork();
    return _eclass;
}

