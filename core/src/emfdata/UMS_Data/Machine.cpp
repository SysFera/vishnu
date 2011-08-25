// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/Machine.cpp
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

#include "Machine.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
Machine::Machine() :
    m_machineId(""), m_machineDescription(""), m_status(1)
{

    /*PROTECTED REGION ID(MachineImpl__MachineImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Machine::~Machine()
{
}

/*PROTECTED REGION ID(Machine.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& Machine::getMachineId() const
{
    return m_machineId;
}

void Machine::setMachineId(::ecore::EString const& _machineId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getMachine__machineId(),
                _old_machineId,
                m_machineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Machine::getName() const
{
    return m_name;
}

void Machine::setName(::ecore::EString const& _name)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_name = m_name;
#endif
    m_name = _name;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getMachine__name(),
                _old_name,
                m_name
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Machine::getSite() const
{
    return m_site;
}

void Machine::setSite(::ecore::EString const& _site)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_site = m_site;
#endif
    m_site = _site;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getMachine__site(),
                _old_site,
                m_site
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Machine::getMachineDescription() const
{
    return m_machineDescription;
}

void Machine::setMachineDescription(::ecore::EString const& _machineDescription)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_machineDescription = m_machineDescription;
#endif
    m_machineDescription = _machineDescription;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getMachine__machineDescription(),
                _old_machineDescription,
                m_machineDescription
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Machine::getLanguage() const
{
    return m_language;
}

void Machine::setLanguage(::ecore::EString const& _language)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_language = m_language;
#endif
    m_language = _language;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getMachine__language(),
                _old_language,
                m_language
        );
        eNotify(&notification);
    }
#endif
}

::UMS_Data::StatusType Machine::getStatus() const
{
    return m_status;
}

void Machine::setStatus(::UMS_Data::StatusType _status)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::UMS_Data::StatusType _old_status = m_status;
#endif
    m_status = _status;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getMachine__status(),
                _old_status,
                m_status
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Machine::getSshPublicKey() const
{
    return m_sshPublicKey;
}

void Machine::setSshPublicKey(::ecore::EString const& _sshPublicKey)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_sshPublicKey = m_sshPublicKey;
#endif
    m_sshPublicKey = _sshPublicKey;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getMachine__sshPublicKey(),
                _old_sshPublicKey,
                m_sshPublicKey
        );
        eNotify(&notification);
    }
#endif
}

// References

