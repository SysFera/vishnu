// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/Job.cpp
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
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
Job::Job() :
    m_jobPrio(-1), m_nbCpus(-1), m_status(-1), m_endDate(-1), m_memLimit(-1),
            m_nbNodes(-1)
{

    /*PROTECTED REGION ID(JobImpl__JobImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Job::~Job()
{
}

/*PROTECTED REGION ID(Job.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& Job::getSessionId() const
{
    return m_sessionId;
}

void Job::setSessionId(::ecore::EString const& _sessionId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_sessionId = m_sessionId;
#endif
    m_sessionId = _sessionId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__sessionId(),
                _old_sessionId,
                m_sessionId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getSubmitMachineId() const
{
    return m_submitMachineId;
}

void Job::setSubmitMachineId(::ecore::EString const& _submitMachineId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_submitMachineId = m_submitMachineId;
#endif
    m_submitMachineId = _submitMachineId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__submitMachineId(),
                _old_submitMachineId,
                m_submitMachineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getSubmitMachineName() const
{
    return m_submitMachineName;
}

void Job::setSubmitMachineName(::ecore::EString const& _submitMachineName)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_submitMachineName = m_submitMachineName;
#endif
    m_submitMachineName = _submitMachineName;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__submitMachineName(),
                _old_submitMachineName,
                m_submitMachineName
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getJobId() const
{
    return m_jobId;
}

void Job::setJobId(::ecore::EString const& _jobId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_jobId = m_jobId;
#endif
    m_jobId = _jobId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__jobId(),
                _old_jobId,
                m_jobId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getJobName() const
{
    return m_jobName;
}

void Job::setJobName(::ecore::EString const& _jobName)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_jobName = m_jobName;
#endif
    m_jobName = _jobName;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__jobName(),
                _old_jobName,
                m_jobName
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getJobPath() const
{
    return m_jobPath;
}

void Job::setJobPath(::ecore::EString const& _jobPath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_jobPath = m_jobPath;
#endif
    m_jobPath = _jobPath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__jobPath(),
                _old_jobPath,
                m_jobPath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getOutputPath() const
{
    return m_outputPath;
}

void Job::setOutputPath(::ecore::EString const& _outputPath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_outputPath = m_outputPath;
#endif
    m_outputPath = _outputPath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__outputPath(),
                _old_outputPath,
                m_outputPath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getErrorPath() const
{
    return m_errorPath;
}

void Job::setErrorPath(::ecore::EString const& _errorPath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_errorPath = m_errorPath;
#endif
    m_errorPath = _errorPath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__errorPath(),
                _old_errorPath,
                m_errorPath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getScriptContent() const
{
    return m_scriptContent;
}

void Job::setScriptContent(::ecore::EString const& _scriptContent)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_scriptContent = m_scriptContent;
#endif
    m_scriptContent = _scriptContent;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__scriptContent(),
                _old_scriptContent,
                m_scriptContent
        );
        eNotify(&notification);
    }
#endif
}

::TMS_Data::JobPriority Job::getJobPrio() const
{
    return m_jobPrio;
}

void Job::setJobPrio(::TMS_Data::JobPriority _jobPrio)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::TMS_Data::JobPriority _old_jobPrio = m_jobPrio;
#endif
    m_jobPrio = _jobPrio;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__jobPrio(),
                _old_jobPrio,
                m_jobPrio
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Job::getNbCpus() const
{
    return m_nbCpus;
}

void Job::setNbCpus(::ecore::EInt _nbCpus)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbCpus = m_nbCpus;
#endif
    m_nbCpus = _nbCpus;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__nbCpus(),
                _old_nbCpus,
                m_nbCpus
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getJobWorkingDir() const
{
    return m_jobWorkingDir;
}

void Job::setJobWorkingDir(::ecore::EString const& _jobWorkingDir)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_jobWorkingDir = m_jobWorkingDir;
#endif
    m_jobWorkingDir = _jobWorkingDir;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__jobWorkingDir(),
                _old_jobWorkingDir,
                m_jobWorkingDir
        );
        eNotify(&notification);
    }
#endif
}

::TMS_Data::JobStatus Job::getStatus() const
{
    return m_status;
}

void Job::setStatus(::TMS_Data::JobStatus _status)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::TMS_Data::JobStatus _old_status = m_status;
#endif
    m_status = _status;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__status(),
                _old_status,
                m_status
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Job::getSubmitDate() const
{
    return m_submitDate;
}

void Job::setSubmitDate(::ecore::ELong _submitDate)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_submitDate = m_submitDate;
#endif
    m_submitDate = _submitDate;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__submitDate(),
                _old_submitDate,
                m_submitDate
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Job::getEndDate() const
{
    return m_endDate;
}

void Job::setEndDate(::ecore::ELong _endDate)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_endDate = m_endDate;
#endif
    m_endDate = _endDate;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__endDate(),
                _old_endDate,
                m_endDate
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getOwner() const
{
    return m_owner;
}

void Job::setOwner(::ecore::EString const& _owner)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_owner = m_owner;
#endif
    m_owner = _owner;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__owner(),
                _old_owner,
                m_owner
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getJobQueue() const
{
    return m_jobQueue;
}

void Job::setJobQueue(::ecore::EString const& _jobQueue)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_jobQueue = m_jobQueue;
#endif
    m_jobQueue = _jobQueue;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__jobQueue(),
                _old_jobQueue,
                m_jobQueue
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Job::getWallClockLimit() const
{
    return m_wallClockLimit;
}

void Job::setWallClockLimit(::ecore::ELong _wallClockLimit)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_wallClockLimit = m_wallClockLimit;
#endif
    m_wallClockLimit = _wallClockLimit;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__wallClockLimit(),
                _old_wallClockLimit,
                m_wallClockLimit
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getGroupName() const
{
    return m_groupName;
}

void Job::setGroupName(::ecore::EString const& _groupName)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_groupName = m_groupName;
#endif
    m_groupName = _groupName;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__groupName(),
                _old_groupName,
                m_groupName
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Job::getJobDescription() const
{
    return m_jobDescription;
}

void Job::setJobDescription(::ecore::EString const& _jobDescription)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_jobDescription = m_jobDescription;
#endif
    m_jobDescription = _jobDescription;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__jobDescription(),
                _old_jobDescription,
                m_jobDescription
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Job::getMemLimit() const
{
    return m_memLimit;
}

void Job::setMemLimit(::ecore::EInt _memLimit)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_memLimit = m_memLimit;
#endif
    m_memLimit = _memLimit;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__memLimit(),
                _old_memLimit,
                m_memLimit
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Job::getNbNodes() const
{
    return m_nbNodes;
}

void Job::setNbNodes(::ecore::EInt _nbNodes)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbNodes = m_nbNodes;
#endif
    m_nbNodes = _nbNodes;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__nbNodes(),
                _old_nbNodes,
                m_nbNodes
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Job::getNbNodesAndCpuPerNode() const
{
    return m_nbNodesAndCpuPerNode;
}

void Job::setNbNodesAndCpuPerNode(::ecore::EInt _nbNodesAndCpuPerNode)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbNodesAndCpuPerNode = m_nbNodesAndCpuPerNode;
#endif
    m_nbNodesAndCpuPerNode = _nbNodesAndCpuPerNode;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getJob__nbNodesAndCpuPerNode(),
                _old_nbNodesAndCpuPerNode,
                m_nbNodesAndCpuPerNode
        );
        eNotify(&notification);
    }
#endif
}

// References

