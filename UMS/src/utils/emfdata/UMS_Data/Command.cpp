// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/Command.cpp
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

#include "Command.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
Command::Command() :
    m_commandId(""), m_sessionId(""), m_machineId(""), m_cmdStartTime(-1),
            m_cmdEndTime(-1)
{

    /*PROTECTED REGION ID(CommandImpl__CommandImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Command::~Command()
{
}

/*PROTECTED REGION ID(Command.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& Command::getCommandId() const
{
    return m_commandId;
}

void Command::setCommandId(::ecore::EString const& _commandId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_commandId = m_commandId;
#endif
    m_commandId = _commandId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getCommand__commandId(),
                _old_commandId,
                m_commandId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Command::getSessionId() const
{
    return m_sessionId;
}

void Command::setSessionId(::ecore::EString const& _sessionId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getCommand__sessionId(),
                _old_sessionId,
                m_sessionId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Command::getMachineId() const
{
    return m_machineId;
}

void Command::setMachineId(::ecore::EString const& _machineId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getCommand__machineId(),
                _old_machineId,
                m_machineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Command::getCmdDescription() const
{
    return m_cmdDescription;
}

void Command::setCmdDescription(::ecore::EString const& _cmdDescription)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_cmdDescription = m_cmdDescription;
#endif
    m_cmdDescription = _cmdDescription;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getCommand__cmdDescription(),
                _old_cmdDescription,
                m_cmdDescription
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Command::getCmdStartTime() const
{
    return m_cmdStartTime;
}

void Command::setCmdStartTime(::ecore::ELong _cmdStartTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_cmdStartTime = m_cmdStartTime;
#endif
    m_cmdStartTime = _cmdStartTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getCommand__cmdStartTime(),
                _old_cmdStartTime,
                m_cmdStartTime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Command::getCmdEndTime() const
{
    return m_cmdEndTime;
}

void Command::setCmdEndTime(::ecore::ELong _cmdEndTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_cmdEndTime = m_cmdEndTime;
#endif
    m_cmdEndTime = _cmdEndTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getCommand__cmdEndTime(),
                _old_cmdEndTime,
                m_cmdEndTime
        );
        eNotify(&notification);
    }
#endif
}

// References

