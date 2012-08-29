// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/AddWorkOptions.cpp
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

#include "AddWorkOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
AddWorkOptions::AddWorkOptions() :
    m_applicationId(""), m_subject(""), m_priority(-1), m_estimatedHour(-1),
            m_projectId(""), m_machineId(""), m_nbCPU(-1)
{

    /*PROTECTED REGION ID(AddWorkOptionsImpl__AddWorkOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

AddWorkOptions::~AddWorkOptions()
{
}

/*PROTECTED REGION ID(AddWorkOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& AddWorkOptions::getApplicationId() const
{
    return m_applicationId;
}

void AddWorkOptions::setApplicationId(::ecore::EString const& _applicationId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_applicationId = m_applicationId;
#endif
    m_applicationId = _applicationId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getAddWorkOptions__applicationId(),
                _old_applicationId,
                m_applicationId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& AddWorkOptions::getSubject() const
{
    return m_subject;
}

void AddWorkOptions::setSubject(::ecore::EString const& _subject)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getAddWorkOptions__subject(),
                _old_subject,
                m_subject
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt AddWorkOptions::getPriority() const
{
    return m_priority;
}

void AddWorkOptions::setPriority(::ecore::EInt _priority)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getAddWorkOptions__priority(),
                _old_priority,
                m_priority
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& AddWorkOptions::getOwner() const
{
    return m_owner;
}

void AddWorkOptions::setOwner(::ecore::EString const& _owner)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getAddWorkOptions__owner(),
                _old_owner,
                m_owner
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong AddWorkOptions::getEstimatedHour() const
{
    return m_estimatedHour;
}

void AddWorkOptions::setEstimatedHour(::ecore::ELong _estimatedHour)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getAddWorkOptions__estimatedHour(),
                _old_estimatedHour,
                m_estimatedHour
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& AddWorkOptions::getDescription() const
{
    return m_description;
}

void AddWorkOptions::setDescription(::ecore::EString const& _description)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getAddWorkOptions__description(),
                _old_description,
                m_description
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& AddWorkOptions::getProjectId() const
{
    return m_projectId;
}

void AddWorkOptions::setProjectId(::ecore::EString const& _projectId)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getAddWorkOptions__projectId(),
                _old_projectId,
                m_projectId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& AddWorkOptions::getMachineId() const
{
    return m_machineId;
}

void AddWorkOptions::setMachineId(::ecore::EString const& _machineId)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getAddWorkOptions__machineId(),
                _old_machineId,
                m_machineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt AddWorkOptions::getNbCPU() const
{
    return m_nbCPU;
}

void AddWorkOptions::setNbCPU(::ecore::EInt _nbCPU)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getAddWorkOptions__nbCPU(),
                _old_nbCPU,
                m_nbCPU
        );
        eNotify(&notification);
    }
#endif
}

// References

