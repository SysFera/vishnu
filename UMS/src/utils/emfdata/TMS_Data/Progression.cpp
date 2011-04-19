// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/Progression.cpp
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
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
Progression::Progression()
{

    /*PROTECTED REGION ID(ProgressionImpl__ProgressionImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Progression::~Progression()
{
}

/*PROTECTED REGION ID(Progression.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& Progression::getJobId() const
{
    return m_jobId;
}

void Progression::setJobId(::ecore::EString const& _jobId)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getProgression__jobId(),
                _old_jobId,
                m_jobId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Progression::getJobName() const
{
    return m_jobName;
}

void Progression::setJobName(::ecore::EString const& _jobName)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getProgression__jobName(),
                _old_jobName,
                m_jobName
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Progression::getWallTime() const
{
    return m_wallTime;
}

void Progression::setWallTime(::ecore::EInt _wallTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_wallTime = m_wallTime;
#endif
    m_wallTime = _wallTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getProgression__wallTime(),
                _old_wallTime,
                m_wallTime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Progression::getStartTime() const
{
    return m_startTime;
}

void Progression::setStartTime(::ecore::ELong _startTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_startTime = m_startTime;
#endif
    m_startTime = _startTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getProgression__startTime(),
                _old_startTime,
                m_startTime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Progression::getEndTime() const
{
    return m_endTime;
}

void Progression::setEndTime(::ecore::ELong _endTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_endTime = m_endTime;
#endif
    m_endTime = _endTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getProgression__endTime(),
                _old_endTime,
                m_endTime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EDouble Progression::getPercent() const
{
    return m_percent;
}

void Progression::setPercent(::ecore::EDouble _percent)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EDouble _old_percent = m_percent;
#endif
    m_percent = _percent;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getProgression__percent(),
                _old_percent,
                m_percent
        );
        eNotify(&notification);
    }
#endif
}

::TMS_Data::JobStatus Progression::getStatus() const
{
    return m_status;
}

void Progression::setStatus(::TMS_Data::JobStatus _status)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getProgression__status(),
                _old_status,
                m_status
        );
        eNotify(&notification);
    }
#endif
}

// References

