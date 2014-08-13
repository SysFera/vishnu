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
    case ::TMS_Data::TMS_DataPackage::JOB__MACHINE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_machine);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__ID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_id);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__NAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_name);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__PATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_path);
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
    case ::TMS_Data::TMS_DataPackage::JOB__NBCPUS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any, m_nbCpus);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__WORKINGDIR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_workingDir);
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
    case ::TMS_Data::TMS_DataPackage::JOB__LOCALACCOUNT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_localAccount);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__QUEUE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_queue);
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
    case ::TMS_Data::TMS_DataPackage::JOB__DESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_description);
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
    case ::TMS_Data::TMS_DataPackage::JOB__BATCHJOBID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_batchJobId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__OUTPUTDIR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_outputDir);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__WORKID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any, m_workId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_userId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__VMID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_vmId);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__VMIP:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_vmIp);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__RELATEDSTEPS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_relatedSteps);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITERROR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_submitError);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::JOB__PRIORITY:
    {
        ::ecorecpp::mapping::any_traits< ::TMS_Data::JobStatus >::toAny(_any,
                m_priority);
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
    case ::TMS_Data::TMS_DataPackage::JOB__MACHINE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_machine);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__ID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_id);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__NAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_name);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__PATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_path);
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
    case ::TMS_Data::TMS_DataPackage::JOB__NBCPUS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_nbCpus);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__WORKINGDIR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_workingDir);
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
    case ::TMS_Data::TMS_DataPackage::JOB__LOCALACCOUNT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_localAccount);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__QUEUE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_queue);
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
    case ::TMS_Data::TMS_DataPackage::JOB__DESCRIPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_description);
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
    case ::TMS_Data::TMS_DataPackage::JOB__BATCHJOBID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_batchJobId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__OUTPUTDIR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_outputDir);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__WORKID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_workId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_userId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__VMID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_vmId);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__VMIP:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_vmIp);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__RELATEDSTEPS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_relatedSteps);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITERROR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_submitError);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::JOB__PRIORITY:
    {
        ::ecorecpp::mapping::any_traits< ::TMS_Data::JobStatus >::fromAny(
                _newValue, m_priority);
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
    case ::TMS_Data::TMS_DataPackage::JOB__MACHINE:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_machine);
    case ::TMS_Data::TMS_DataPackage::JOB__ID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(m_id);
    case ::TMS_Data::TMS_DataPackage::JOB__NAME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_name);
    case ::TMS_Data::TMS_DataPackage::JOB__PATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_path);
    case ::TMS_Data::TMS_DataPackage::JOB__OUTPUTPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_outputPath);
    case ::TMS_Data::TMS_DataPackage::JOB__ERRORPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_errorPath);
    case ::TMS_Data::TMS_DataPackage::JOB__NBCPUS:
        return m_nbCpus != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__WORKINGDIR:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_workingDir);
    case ::TMS_Data::TMS_DataPackage::JOB__STATUS:
        return m_status != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITDATE:
        return m_submitDate != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__ENDDATE:
        return m_endDate != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__LOCALACCOUNT:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_localAccount);
    case ::TMS_Data::TMS_DataPackage::JOB__QUEUE:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_queue);
    case ::TMS_Data::TMS_DataPackage::JOB__WALLCLOCKLIMIT:
        return m_wallClockLimit != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__GROUPNAME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_groupName);
    case ::TMS_Data::TMS_DataPackage::JOB__DESCRIPTION:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_description);
    case ::TMS_Data::TMS_DataPackage::JOB__MEMLIMIT:
        return m_memLimit != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__NBNODES:
        return m_nbNodes != -1;
    case ::TMS_Data::TMS_DataPackage::JOB__NBNODESANDCPUPERNODE:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_nbNodesAndCpuPerNode);
    case ::TMS_Data::TMS_DataPackage::JOB__BATCHJOBID:
        return m_batchJobId != "";
    case ::TMS_Data::TMS_DataPackage::JOB__OUTPUTDIR:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_outputDir);
    case ::TMS_Data::TMS_DataPackage::JOB__WORKID:
        return m_workId != 0;
    case ::TMS_Data::TMS_DataPackage::JOB__USERID:
        return m_userId != "";
    case ::TMS_Data::TMS_DataPackage::JOB__VMID:
        return m_vmId != "";
    case ::TMS_Data::TMS_DataPackage::JOB__VMIP:
        return m_vmIp != "";
    case ::TMS_Data::TMS_DataPackage::JOB__RELATEDSTEPS:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_relatedSteps);
    case ::TMS_Data::TMS_DataPackage::JOB__SUBMITERROR:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_submitError);
    case ::TMS_Data::TMS_DataPackage::JOB__PRIORITY:
        return m_priority != -1;

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

