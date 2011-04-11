// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListJobsOptions.cpp
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

#include "ListJobsOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
ListJobsOptions::ListJobsOptions()
{

    /*PROTECTED REGION ID(ListJobsOptionsImpl__ListJobsOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListJobsOptions::~ListJobsOptions()
{
}

/*PROTECTED REGION ID(ListJobsOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& ListJobsOptions::getJobId() const
{
    return m_jobId;
}

void ListJobsOptions::setJobId(::ecore::EString const& _jobId)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobsOptions__jobId(),
                _old_jobId,
                m_jobId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt ListJobsOptions::getNbCpu() const
{
    return m_nbCpu;
}

void ListJobsOptions::setNbCpu(::ecore::EInt _nbCpu)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbCpu = m_nbCpu;
#endif
    m_nbCpu = _nbCpu;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobsOptions__nbCpu(),
                _old_nbCpu,
                m_nbCpu
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong ListJobsOptions::getFromSubmitDate() const
{
    return m_fromSubmitDate;
}

void ListJobsOptions::setFromSubmitDate(::ecore::ELong _fromSubmitDate)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_fromSubmitDate = m_fromSubmitDate;
#endif
    m_fromSubmitDate = _fromSubmitDate;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobsOptions__fromSubmitDate(),
                _old_fromSubmitDate,
                m_fromSubmitDate
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong ListJobsOptions::getToSubmitDate() const
{
    return m_toSubmitDate;
}

void ListJobsOptions::setToSubmitDate(::ecore::ELong _toSubmitDate)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_toSubmitDate = m_toSubmitDate;
#endif
    m_toSubmitDate = _toSubmitDate;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobsOptions__toSubmitDate(),
                _old_toSubmitDate,
                m_toSubmitDate
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListJobsOptions::getOwner() const
{
    return m_owner;
}

void ListJobsOptions::setOwner(::ecore::EString const& _owner)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobsOptions__owner(),
                _old_owner,
                m_owner
        );
        eNotify(&notification);
    }
#endif
}

::TMS_Data::JobStatus ListJobsOptions::getStatus() const
{
    return m_status;
}

void ListJobsOptions::setStatus(::TMS_Data::JobStatus _status)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobsOptions__status(),
                _old_status,
                m_status
        );
        eNotify(&notification);
    }
#endif
}

::TMS_Data::JobPriority ListJobsOptions::getPriority() const
{
    return m_priority;
}

void ListJobsOptions::setPriority(::TMS_Data::JobPriority _priority)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::TMS_Data::JobPriority _old_priority = m_priority;
#endif
    m_priority = _priority;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobsOptions__priority(),
                _old_priority,
                m_priority
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListJobsOptions::getOutPutPath() const
{
    return m_outPutPath;
}

void ListJobsOptions::setOutPutPath(::ecore::EString const& _outPutPath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_outPutPath = m_outPutPath;
#endif
    m_outPutPath = _outPutPath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobsOptions__outPutPath(),
                _old_outPutPath,
                m_outPutPath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListJobsOptions::getErrorPath() const
{
    return m_errorPath;
}

void ListJobsOptions::setErrorPath(::ecore::EString const& _errorPath)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobsOptions__errorPath(),
                _old_errorPath,
                m_errorPath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListJobsOptions::getQueue() const
{
    return m_queue;
}

void ListJobsOptions::setQueue(::ecore::EString const& _queue)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_queue = m_queue;
#endif
    m_queue = _queue;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobsOptions__queue(),
                _old_queue,
                m_queue
        );
        eNotify(&notification);
    }
#endif
}

// References

