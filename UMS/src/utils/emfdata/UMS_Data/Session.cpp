// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/Session.cpp
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

#include "Session.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
Session::Session() :
    m_sessionId(""), m_userId(""), m_sessionKey(""), m_dateLastConnect(-1),
            m_dateCreation(-1), m_dateClosure(-1), m_state(1),
            m_closePolicy(0), m_timeout(0)
{

    /*PROTECTED REGION ID(SessionImpl__SessionImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Session::~Session()
{
}

/*PROTECTED REGION ID(Session.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& Session::getSessionId() const
{
    return m_sessionId;
}

void Session::setSessionId(::ecore::EString const& _sessionId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getSession__sessionId(),
                _old_sessionId,
                m_sessionId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Session::getUserId() const
{
    return m_userId;
}

void Session::setUserId(::ecore::EString const& _userId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getSession__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Session::getSessionKey() const
{
    return m_sessionKey;
}

void Session::setSessionKey(::ecore::EString const& _sessionKey)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_sessionKey = m_sessionKey;
#endif
    m_sessionKey = _sessionKey;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getSession__sessionKey(),
                _old_sessionKey,
                m_sessionKey
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Session::getDateLastConnect() const
{
    return m_dateLastConnect;
}

void Session::setDateLastConnect(::ecore::ELong _dateLastConnect)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_dateLastConnect = m_dateLastConnect;
#endif
    m_dateLastConnect = _dateLastConnect;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getSession__dateLastConnect(),
                _old_dateLastConnect,
                m_dateLastConnect
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Session::getDateCreation() const
{
    return m_dateCreation;
}

void Session::setDateCreation(::ecore::ELong _dateCreation)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_dateCreation = m_dateCreation;
#endif
    m_dateCreation = _dateCreation;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getSession__dateCreation(),
                _old_dateCreation,
                m_dateCreation
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Session::getDateClosure() const
{
    return m_dateClosure;
}

void Session::setDateClosure(::ecore::ELong _dateClosure)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_dateClosure = m_dateClosure;
#endif
    m_dateClosure = _dateClosure;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getSession__dateClosure(),
                _old_dateClosure,
                m_dateClosure
        );
        eNotify(&notification);
    }
#endif
}

::UMS_Data::SessionStateType Session::getState() const
{
    return m_state;
}

void Session::setState(::UMS_Data::SessionStateType _state)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::UMS_Data::SessionStateType _old_state = m_state;
#endif
    m_state = _state;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getSession__state(),
                _old_state,
                m_state
        );
        eNotify(&notification);
    }
#endif
}

::UMS_Data::SessionCloseType Session::getClosePolicy() const
{
    return m_closePolicy;
}

void Session::setClosePolicy(::UMS_Data::SessionCloseType _closePolicy)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::UMS_Data::SessionCloseType _old_closePolicy = m_closePolicy;
#endif
    m_closePolicy = _closePolicy;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getSession__closePolicy(),
                _old_closePolicy,
                m_closePolicy
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Session::getTimeout() const
{
    return m_timeout;
}

void Session::setTimeout(::ecore::ELong _timeout)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_timeout = m_timeout;
#endif
    m_timeout = _timeout;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getSession__timeout(),
                _old_timeout,
                m_timeout
        );
        eNotify(&notification);
    }
#endif
}

// References

