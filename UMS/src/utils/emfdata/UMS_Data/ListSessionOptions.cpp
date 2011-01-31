// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListSessionOptions.cpp
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

#include "ListSessionOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
ListSessionOptions::ListSessionOptions() :
    m_sessionListOption(1), m_sessionClosePolicy(0),
            m_sessionInactivityDelay(0), m_machineId(""), m_adminListOption(
                    false), m_sessionId(""), m_startDateOption(-1),
            m_endDateOption(-1)
{

    /*PROTECTED REGION ID(ListSessionOptionsImpl__ListSessionOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListSessionOptions::~ListSessionOptions()
{
}

/*PROTECTED REGION ID(ListSessionOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::UMS_Data::SessionStateType ListSessionOptions::getSessionListOption() const
{
    return m_sessionListOption;
}

void ListSessionOptions::setSessionListOption(
        ::UMS_Data::SessionStateType _sessionListOption)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::UMS_Data::SessionStateType _old_sessionListOption = m_sessionListOption;
#endif
    m_sessionListOption = _sessionListOption;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListSessionOptions__sessionListOption(),
                _old_sessionListOption,
                m_sessionListOption
        );
        eNotify(&notification);
    }
#endif
}

::UMS_Data::SessionCloseType ListSessionOptions::getSessionClosePolicy() const
{
    return m_sessionClosePolicy;
}

void ListSessionOptions::setSessionClosePolicy(
        ::UMS_Data::SessionCloseType _sessionClosePolicy)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::UMS_Data::SessionCloseType _old_sessionClosePolicy = m_sessionClosePolicy;
#endif
    m_sessionClosePolicy = _sessionClosePolicy;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListSessionOptions__sessionClosePolicy(),
                _old_sessionClosePolicy,
                m_sessionClosePolicy
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt ListSessionOptions::getSessionInactivityDelay() const
{
    return m_sessionInactivityDelay;
}

void ListSessionOptions::setSessionInactivityDelay(
        ::ecore::EInt _sessionInactivityDelay)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_sessionInactivityDelay = m_sessionInactivityDelay;
#endif
    m_sessionInactivityDelay = _sessionInactivityDelay;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListSessionOptions__sessionInactivityDelay(),
                _old_sessionInactivityDelay,
                m_sessionInactivityDelay
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListSessionOptions::getMachineId() const
{
    return m_machineId;
}

void ListSessionOptions::setMachineId(::ecore::EString const& _machineId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListSessionOptions__machineId(),
                _old_machineId,
                m_machineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EBoolean ListSessionOptions::isAdminListOption() const
{
    return m_adminListOption;
}

void ListSessionOptions::setAdminListOption(::ecore::EBoolean _adminListOption)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EBoolean _old_adminListOption = m_adminListOption;
#endif
    m_adminListOption = _adminListOption;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListSessionOptions__adminListOption(),
                _old_adminListOption,
                m_adminListOption
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListSessionOptions::getUserId() const
{
    return m_userId;
}

void ListSessionOptions::setUserId(::ecore::EString const& _userId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_userId = m_userId;
#endif
    m_userId = _userId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListSessionOptions__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListSessionOptions::getSessionId() const
{
    return m_sessionId;
}

void ListSessionOptions::setSessionId(::ecore::EString const& _sessionId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListSessionOptions__sessionId(),
                _old_sessionId,
                m_sessionId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong ListSessionOptions::getStartDateOption() const
{
    return m_startDateOption;
}

void ListSessionOptions::setStartDateOption(::ecore::ELong _startDateOption)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_startDateOption = m_startDateOption;
#endif
    m_startDateOption = _startDateOption;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListSessionOptions__startDateOption(),
                _old_startDateOption,
                m_startDateOption
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong ListSessionOptions::getEndDateOption() const
{
    return m_endDateOption;
}

void ListSessionOptions::setEndDateOption(::ecore::ELong _endDateOption)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_endDateOption = m_endDateOption;
#endif
    m_endDateOption = _endDateOption;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListSessionOptions__endDateOption(),
                _old_endDateOption,
                m_endDateOption
        );
        eNotify(&notification);
    }
#endif
}

// References

