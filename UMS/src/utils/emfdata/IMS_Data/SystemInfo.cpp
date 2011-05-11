// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/SystemInfo.cpp
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

#include "SystemInfo.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "IMS_Data/IMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::IMS_Data;

// Default constructor
SystemInfo::SystemInfo() :
    m_memory(0), m_diskSpace(0), m_machineId("")
{

    /*PROTECTED REGION ID(SystemInfoImpl__SystemInfoImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

SystemInfo::~SystemInfo()
{
}

/*PROTECTED REGION ID(SystemInfo.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::ELong SystemInfo::getMemory() const
{
    return m_memory;
}

void SystemInfo::setMemory(::ecore::ELong _memory)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_memory = m_memory;
#endif
    m_memory = _memory;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getSystemInfo__memory(),
                _old_memory,
                m_memory
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong SystemInfo::getDiskSpace() const
{
    return m_diskSpace;
}

void SystemInfo::setDiskSpace(::ecore::ELong _diskSpace)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_diskSpace = m_diskSpace;
#endif
    m_diskSpace = _diskSpace;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getSystemInfo__diskSpace(),
                _old_diskSpace,
                m_diskSpace
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SystemInfo::getMachineId() const
{
    return m_machineId;
}

void SystemInfo::setMachineId(::ecore::EString const& _machineId)
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
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getSystemInfo__machineId(),
                _old_machineId,
                m_machineId
        );
        eNotify(&notification);
    }
#endif
}

// References

