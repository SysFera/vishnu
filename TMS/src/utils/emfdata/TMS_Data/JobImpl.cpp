// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/JobImpl.cpp
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

#include "Job.hpp"
#include <TMS_Data/TMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::TMS_Data;

/*PROTECTED REGION ID(JobImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void Job::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(JobImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject Job::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::JOB__SESSIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_sessionId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITMACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_submitMachineId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITMACHINENAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_submitMachineName);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_jobId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_jobName);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_jobPath);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__OUTPUTPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_outputPath);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__ERRORPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_errorPath);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBPRIO:
    {
        ::ecorecpp::mapping::any_traits< ::TMS_Data::JobPriority >::toAny(_any,
                m_jobPrio);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__NBCPUS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any, m_nbCpus);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBWORKINGDIR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_jobWorkingDir);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::TMS_Data::JobStatus >::toAny(_any,
                m_status);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITDATE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_submitDate);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__ENDDATE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_endDate);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__OWNER:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_owner);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBQUEUE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_jobQueue);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__WALLCLOCKLIMIT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_wallClockLimit);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__GROUPNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_groupName);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBDESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_jobDescription);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__MEMLIMIT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any,
                m_memLimit);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__NBNODES:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any, m_nbNodes);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__NBNODESANDCPUPERNODE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_nbNodesAndCpuPerNode);
    }
        return _any;

    }
    throw "Error";
}

void Job::eSet(::ecore::EInt _featureID, ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::JOB__SESSIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_sessionId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITMACHINEID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_submitMachineId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITMACHINENAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_submitMachineName);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_jobId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_jobName);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_jobPath);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__OUTPUTPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_outputPath);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__ERRORPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_errorPath);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBPRIO:
    {
        ::ecorecpp::mapping::any_traits< ::TMS_Data::JobPriority >::fromAny(
                _newValue, m_jobPrio);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__NBCPUS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_nbCpus);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBWORKINGDIR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_jobWorkingDir);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__STATUS:
    {
        ::ecorecpp::mapping::any_traits< ::TMS_Data::JobStatus >::fromAny(
                _newValue, m_status);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITDATE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_submitDate);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__ENDDATE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_endDate);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__OWNER:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_owner);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBQUEUE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_jobQueue);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__WALLCLOCKLIMIT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_wallClockLimit);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__GROUPNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_groupName);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBDESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_jobDescription);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__MEMLIMIT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_memLimit);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__NBNODES:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_nbNodes);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__NBNODESANDCPUPERNODE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_nbNodesAndCpuPerNode);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean Job::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::JOB__SESSIONID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_sessionId);
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITMACHINEID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_submitMachineId);
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITMACHINENAME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_submitMachineName);
    case ::TMS_Data::TMS_DataPackage::JOB__JOBID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_jobId);
    case ::TMS_Data::TMS_DataPackage::JOB__JOBNAME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_jobName);
    case ::TMS_Data::TMS_DataPackage::JOB__JOBPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_jobPath);
    case ::TMS_Data::TMS_DataPackage::JOB__OUTPUTPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_outputPath);
    case ::TMS_Data::TMS_DataPackage::JOB__ERRORPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_errorPath);
    case ::TMS_Data::TMS_DataPackage::JOB__JOBPRIO:
        return m_jobPrio != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__NBCPUS:
        return m_nbCpus != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__JOBWORKINGDIR:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_jobWorkingDir);
    case ::TMS_Data::TMS_DataPackage::JOB__STATUS:
        return m_status != NOT_SUBMITTED;
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITDATE:
        return ::ecorecpp::mapping::set_traits< ::ecore::ELong >::is_set(
                m_submitDate);
    case ::TMS_Data::TMS_DataPackage::JOB__ENDDATE:
        return m_endDate != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__OWNER:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_owner);
    case ::TMS_Data::TMS_DataPackage::JOB__JOBQUEUE:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_jobQueue);
    case ::TMS_Data::TMS_DataPackage::JOB__WALLCLOCKLIMIT:
        return ::ecorecpp::mapping::set_traits< ::ecore::ELong >::is_set(
                m_wallClockLimit);
    case ::TMS_Data::TMS_DataPackage::JOB__GROUPNAME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_groupName);
    case ::TMS_Data::TMS_DataPackage::JOB__JOBDESCRIPTION:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_jobDescription);
    case ::TMS_Data::TMS_DataPackage::JOB__MEMLIMIT:
        return m_memLimit != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__NBNODES:
        return m_nbNodes != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__NBNODESANDCPUPERNODE:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_nbNodesAndCpuPerNode);

    }
    throw "Error";
}

void Job::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr Job::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (::TMS_Data::TMS_DataPackage::_instance())->getJob();
    return _eclass;
}

