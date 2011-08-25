// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/Process.cpp
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

#include "Process.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "IMS_Data/IMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::IMS_Data;

// Default constructor
Process::Process() :
    m_processName(""), m_machineId(""), m_dietId(""), m_state(0),
            m_timestamp(0), m_script("")
{

    /*PROTECTED REGION ID(ProcessImpl__ProcessImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Process::~Process()
{
}

/*PROTECTED REGION ID(Process.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& Process::getProcessName() const
{
    return m_processName;
}

void Process::setProcessName(::ecore::EString const& _processName)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_processName = m_processName;
#endif
    m_processName = _processName;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getProcess__processName(),
                _old_processName,
                m_processName
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Process::getMachineId() const
{
    return m_machineId;
}

void Process::setMachineId(::ecore::EString const& _machineId)
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
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getProcess__machineId(),
                _old_machineId,
                m_machineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Process::getDietId() const
{
    return m_dietId;
}

void Process::setDietId(::ecore::EString const& _dietId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_dietId = m_dietId;
#endif
    m_dietId = _dietId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getProcess__dietId(),
                _old_dietId,
                m_dietId
        );
        eNotify(&notification);
    }
#endif
}

::IMS_Data::ProcessState Process::getState() const
{
    return m_state;
}

void Process::setState(::IMS_Data::ProcessState _state)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::IMS_Data::ProcessState _old_state = m_state;
#endif
    m_state = _state;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getProcess__state(),
                _old_state,
                m_state
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Process::getTimestamp() const
{
    return m_timestamp;
}

void Process::setTimestamp(::ecore::ELong _timestamp)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_timestamp = m_timestamp;
#endif
    m_timestamp = _timestamp;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getProcess__timestamp(),
                _old_timestamp,
                m_timestamp
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Process::getScript() const
{
    return m_script;
}

void Process::setScript(::ecore::EString const& _script)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_script = m_script;
#endif
    m_script = _script;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getProcess__script(),
                _old_script,
                m_script
        );
        eNotify(&notification);
    }
#endif
}

// References

