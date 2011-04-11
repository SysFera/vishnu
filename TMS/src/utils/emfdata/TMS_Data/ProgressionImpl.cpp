// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ProgressionImpl.cpp
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

#include "Progression.hpp"
#include <TMS_Data/TMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::TMS_Data;

/*PROTECTED REGION ID(ProgressionImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void Progression::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(ProgressionImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject Progression::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__JOBID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_jobId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__JOBNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_jobName);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__WALLTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any,
                m_wallTime);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__STARTTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_startTime);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__ENDTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_endTime);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__PERCENT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EDouble >::toAny(_any,
                m_percent);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::TMS_Data::JobStatus >::toAny(_any,
                m_status);
    }
        return _any;

    }
    throw "Error";
}

void Progression::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__JOBID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_jobId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__JOBNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_jobName);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__WALLTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_wallTime);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__STARTTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_startTime);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__ENDTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_endTime);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__PERCENT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EDouble >::fromAny(_newValue,
                m_percent);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::TMS_Data::JobStatus >::fromAny(
                _newValue, m_status);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean Progression::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__JOBID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_jobId);
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__JOBNAME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_jobName);
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__WALLTIME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EInt >::is_set(
                m_wallTime);
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__STARTTIME:
        return ::ecorecpp::mapping::set_traits< ::ecore::ELong >::is_set(
                m_startTime);
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__ENDTIME:
        return ::ecorecpp::mapping::set_traits< ::ecore::ELong >::is_set(
                m_endTime);
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__PERCENT:
        return ::ecorecpp::mapping::set_traits< ::ecore::EDouble >::is_set(
                m_percent);
    case ::TMS_Data::TMS_DataPackage::PROGRESSION__STATUS:
        return ::ecorecpp::mapping::set_traits< ::TMS_Data::JobStatus >::is_set(
                m_status);

    }
    throw "Error";
}

void Progression::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr Progression::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (::TMS_Data::TMS_DataPackage::_instance())->getProgression();
    return _eclass;
}

