// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/JobResultImpl.cpp
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

#include "JobResult.hpp"
#include <TMS_Data/TMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::TMS_Data;

/*PROTECTED REGION ID(JobResultImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void JobResult::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(JobResultImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject JobResult::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::JOBRESULT__JOBID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_jobId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOBRESULT__OUTPUTPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_outputPath);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOBRESULT__ERRORPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_errorPath);
    }
        return _any;

    }
    throw "Error";
}

void JobResult::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::JOBRESULT__JOBID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_jobId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOBRESULT__OUTPUTPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_outputPath);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOBRESULT__ERRORPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_errorPath);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean JobResult::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::JOBRESULT__JOBID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_jobId);
    case ::TMS_Data::TMS_DataPackage::JOBRESULT__OUTPUTPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_outputPath);
    case ::TMS_Data::TMS_DataPackage::JOBRESULT__ERRORPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_errorPath);

    }
    throw "Error";
}

void JobResult::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr JobResult::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (::TMS_Data::TMS_DataPackage::_instance())->getJobResult();
    return _eclass;
}

