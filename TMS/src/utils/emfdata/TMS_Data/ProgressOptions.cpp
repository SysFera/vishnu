// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ProgressOptions.cpp
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

#include "ProgressOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
ProgressOptions::ProgressOptions()
{

    /*PROTECTED REGION ID(ProgressOptionsImpl__ProgressOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ProgressOptions::~ProgressOptions()
{
}

/*PROTECTED REGION ID(ProgressOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& ProgressOptions::getJobId() const
{
    return m_jobId;
}

void ProgressOptions::setJobId(::ecore::EString const& _jobId)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getProgressOptions__jobId(),
                _old_jobId,
                m_jobId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ProgressOptions::getJobOwner() const
{
    return m_jobOwner;
}

void ProgressOptions::setJobOwner(::ecore::EString const& _jobOwner)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_jobOwner = m_jobOwner;
#endif
    m_jobOwner = _jobOwner;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getProgressOptions__jobOwner(),
                _old_jobOwner,
                m_jobOwner
        );
        eNotify(&notification);
    }
#endif
}

// References

