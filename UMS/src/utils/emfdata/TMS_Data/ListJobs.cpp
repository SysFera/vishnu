// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListJobs.cpp
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

#include "ListJobs.hpp"
#include <TMS_Data/Job.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
ListJobs::ListJobs()
{

    m_jobs.reset(new ::ecorecpp::mapping::ReferenceEListImpl< ::TMS_Data::Job,
            -1, true, false >(this, NULL));

    /*PROTECTED REGION ID(ListJobsImpl__ListJobsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListJobs::~ListJobs()
{
}

/*PROTECTED REGION ID(ListJobs.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::ELong ListJobs::getNbJobs() const
{
    return m_nbJobs;
}

void ListJobs::setNbJobs(::ecore::ELong _nbJobs)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_nbJobs = m_nbJobs;
#endif
    m_nbJobs = _nbJobs;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobs__nbJobs(),
                _old_nbJobs,
                m_nbJobs
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong ListJobs::getNbRunningJobs() const
{
    return m_nbRunningJobs;
}

void ListJobs::setNbRunningJobs(::ecore::ELong _nbRunningJobs)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_nbRunningJobs = m_nbRunningJobs;
#endif
    m_nbRunningJobs = _nbRunningJobs;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobs__nbRunningJobs(),
                _old_nbRunningJobs,
                m_nbRunningJobs
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong ListJobs::getNbWaitingJobs() const
{
    return m_nbWaitingJobs;
}

void ListJobs::setNbWaitingJobs(::ecore::ELong _nbWaitingJobs)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_nbWaitingJobs = m_nbWaitingJobs;
#endif
    m_nbWaitingJobs = _nbWaitingJobs;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobs__nbWaitingJobs(),
                _old_nbWaitingJobs,
                m_nbWaitingJobs
        );
        eNotify(&notification);
    }
#endif
}

// References
::ecorecpp::mapping::EList< ::TMS_Data::Job >& ListJobs::getJobs()
{
    return *m_jobs;
}

