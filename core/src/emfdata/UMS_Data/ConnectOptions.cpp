// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ConnectOptions.cpp
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

#include "ConnectOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
ConnectOptions::ConnectOptions() :
    m_closePolicy(0), m_sessionInactivityDelay(0)
{

    /*PROTECTED REGION ID(ConnectOptionsImpl__ConnectOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ConnectOptions::~ConnectOptions()
{
}

/*PROTECTED REGION ID(ConnectOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::UMS_Data::SessionCloseType ConnectOptions::getClosePolicy() const
{
    return m_closePolicy;
}

void ConnectOptions::setClosePolicy(::UMS_Data::SessionCloseType _closePolicy)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getConnectOptions__closePolicy(),
                _old_closePolicy,
                m_closePolicy
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt ConnectOptions::getSessionInactivityDelay() const
{
    return m_sessionInactivityDelay;
}

void ConnectOptions::setSessionInactivityDelay(
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getConnectOptions__sessionInactivityDelay(),
                _old_sessionInactivityDelay,
                m_sessionInactivityDelay
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ConnectOptions::getSubstituteUserId() const
{
    return m_substituteUserId;
}

void ConnectOptions::setSubstituteUserId(
        ::ecore::EString const& _substituteUserId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_substituteUserId = m_substituteUserId;
#endif
    m_substituteUserId = _substituteUserId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getConnectOptions__substituteUserId(),
                _old_substituteUserId,
                m_substituteUserId
        );
        eNotify(&notification);
    }
#endif
}

// References

