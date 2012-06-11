// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/Work.cpp
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
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
Work::Work() :
    m_sessionId(""), m_applicationId(0), m_subject(""), m_priority(-1),
            m_status(-1), m_endDate(-1), m_estimatedHour(-1), m_doneRatio(0),
            m_batchJobId(""), m_dateCreated(-1), m_dateEnded(-1),
            m_dateStarted(-1), m_lastUpdated(-1), m_projectId(""),
            m_submitDate(0), m_machineId(""), m_nbCPU(-1)
{

    /*PROTECTED REGION ID(WorkImpl__WorkImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Work::~Work()
{
}

/*PROTECTED REGION ID(Work.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& Work::getSessionId() const
{
    return m_sessionId;
}

void Work::setSessionId(::ecore::EString const& _sessionId)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__sessionId(),
                _old_sessionId,
                m_sessionId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Work::getApplicationId() const
{
    return m_applicationId;
}

void Work::setApplicationId(::ecore::ELong _applicationId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_applicationId = m_applicationId;
#endif
    m_applicationId = _applicationId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__applicationId(),
                _old_applicationId,
                m_applicationId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Work::getSubject() const
{
    return m_subject;
}

void Work::setSubject(::ecore::EString const& _subject)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_subject = m_subject;
#endif
    m_subject = _subject;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__subject(),
                _old_subject,
                m_subject
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Work::getPriority() const
{
    return m_priority;
}

void Work::setPriority(::ecore::EInt _priority)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_priority = m_priority;
#endif
    m_priority = _priority;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__priority(),
                _old_priority,
                m_priority
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Work::getStatus() const
{
    return m_status;
}

void Work::setStatus(::ecore::EInt _status)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_status = m_status;
#endif
    m_status = _status;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__status(),
                _old_status,
                m_status
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Work::getEndDate() const
{
    return m_endDate;
}

void Work::setEndDate(::ecore::ELong _endDate)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__endDate(),
                _old_endDate,
                m_endDate
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Work::getOwner() const
{
    return m_owner;
}

void Work::setOwner(::ecore::EString const& _owner)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__owner(),
                _old_owner,
                m_owner
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Work::getEstimatedHour() const
{
    return m_estimatedHour;
}

void Work::setEstimatedHour(::ecore::ELong _estimatedHour)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_estimatedHour = m_estimatedHour;
#endif
    m_estimatedHour = _estimatedHour;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__estimatedHour(),
                _old_estimatedHour,
                m_estimatedHour
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Work::getDoneRatio() const
{
    return m_doneRatio;
}

void Work::setDoneRatio(::ecore::EInt _doneRatio)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_doneRatio = m_doneRatio;
#endif
    m_doneRatio = _doneRatio;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__doneRatio(),
                _old_doneRatio,
                m_doneRatio
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Work::getDescription() const
{
    return m_description;
}

void Work::setDescription(::ecore::EString const& _description)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_description = m_description;
#endif
    m_description = _description;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__description(),
                _old_description,
                m_description
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Work::getBatchJobId() const
{
    return m_batchJobId;
}

void Work::setBatchJobId(::ecore::EString const& _batchJobId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_batchJobId = m_batchJobId;
#endif
    m_batchJobId = _batchJobId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__batchJobId(),
                _old_batchJobId,
                m_batchJobId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Work::getDateCreated() const
{
    return m_dateCreated;
}

void Work::setDateCreated(::ecore::ELong _dateCreated)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_dateCreated = m_dateCreated;
#endif
    m_dateCreated = _dateCreated;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__dateCreated(),
                _old_dateCreated,
                m_dateCreated
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Work::getDateEnded() const
{
    return m_dateEnded;
}

void Work::setDateEnded(::ecore::ELong _dateEnded)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_dateEnded = m_dateEnded;
#endif
    m_dateEnded = _dateEnded;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__dateEnded(),
                _old_dateEnded,
                m_dateEnded
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Work::getDateStarted() const
{
    return m_dateStarted;
}

void Work::setDateStarted(::ecore::ELong _dateStarted)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_dateStarted = m_dateStarted;
#endif
    m_dateStarted = _dateStarted;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__dateStarted(),
                _old_dateStarted,
                m_dateStarted
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Work::getLastUpdated() const
{
    return m_lastUpdated;
}

void Work::setLastUpdated(::ecore::ELong _lastUpdated)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_lastUpdated = m_lastUpdated;
#endif
    m_lastUpdated = _lastUpdated;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__lastUpdated(),
                _old_lastUpdated,
                m_lastUpdated
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Work::getWorkId() const
{
    return m_workId;
}

void Work::setWorkId(::ecore::EString const& _workId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_workId = m_workId;
#endif
    m_workId = _workId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__workId(),
                _old_workId,
                m_workId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Work::getProjectId() const
{
    return m_projectId;
}

void Work::setProjectId(::ecore::EString const& _projectId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_projectId = m_projectId;
#endif
    m_projectId = _projectId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__projectId(),
                _old_projectId,
                m_projectId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Work::getSubmitDate() const
{
    return m_submitDate;
}

void Work::setSubmitDate(::ecore::ELong _submitDate)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__submitDate(),
                _old_submitDate,
                m_submitDate
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Work::getMachineId() const
{
    return m_machineId;
}

void Work::setMachineId(::ecore::EString const& _machineId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_machineId = m_machineId;
#endif
    m_machineId = _machineId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__machineId(),
                _old_machineId,
                m_machineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Work::getNbCPU() const
{
    return m_nbCPU;
}

void Work::setNbCPU(::ecore::EInt _nbCPU)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbCPU = m_nbCPU;
#endif
    m_nbCPU = _nbCPU;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getWork__nbCPU(),
                _old_nbCPU,
                m_nbCPU
        );
        eNotify(&notification);
    }
#endif
}

// References

