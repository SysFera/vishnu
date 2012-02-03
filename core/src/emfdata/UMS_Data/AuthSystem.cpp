// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/AuthSystem.cpp
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

#include "AuthSystem.hpp"
#include <UMS_Data/AuthSystemOptions.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
AuthSystem::AuthSystem() :
    m_authSystemId(""), m_name(""), m_URI(""), m_authLogin(""), m_authPassword(
            ""), m_userPasswordEncryption(0), m_type(0), m_status(1),
            m_options(0)
{

    /*PROTECTED REGION ID(AuthSystemImpl__AuthSystemImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

AuthSystem::~AuthSystem()
{
}

/*PROTECTED REGION ID(AuthSystem.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& AuthSystem::getAuthSystemId() const
{
    return m_authSystemId;
}

void AuthSystem::setAuthSystemId(::ecore::EString const& _authSystemId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_authSystemId = m_authSystemId;
#endif
    m_authSystemId = _authSystemId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getAuthSystem__authSystemId(),
                _old_authSystemId,
                m_authSystemId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& AuthSystem::getName() const
{
    return m_name;
}

void AuthSystem::setName(::ecore::EString const& _name)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getAuthSystem__name(),
                _old_name,
                m_name
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& AuthSystem::getURI() const
{
    return m_URI;
}

void AuthSystem::setURI(::ecore::EString const& _URI)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_URI = m_URI;
#endif
    m_URI = _URI;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getAuthSystem__URI(),
                _old_URI,
                m_URI
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& AuthSystem::getAuthLogin() const
{
    return m_authLogin;
}

void AuthSystem::setAuthLogin(::ecore::EString const& _authLogin)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_authLogin = m_authLogin;
#endif
    m_authLogin = _authLogin;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getAuthSystem__authLogin(),
                _old_authLogin,
                m_authLogin
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& AuthSystem::getAuthPassword() const
{
    return m_authPassword;
}

void AuthSystem::setAuthPassword(::ecore::EString const& _authPassword)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_authPassword = m_authPassword;
#endif
    m_authPassword = _authPassword;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getAuthSystem__authPassword(),
                _old_authPassword,
                m_authPassword
        );
        eNotify(&notification);
    }
#endif
}

::UMS_Data::EncryptionMethod AuthSystem::getUserPasswordEncryption() const
{
    return m_userPasswordEncryption;
}

void AuthSystem::setUserPasswordEncryption(
        ::UMS_Data::EncryptionMethod _userPasswordEncryption)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::UMS_Data::EncryptionMethod _old_userPasswordEncryption = m_userPasswordEncryption;
#endif
    m_userPasswordEncryption = _userPasswordEncryption;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getAuthSystem__userPasswordEncryption(),
                _old_userPasswordEncryption,
                m_userPasswordEncryption
        );
        eNotify(&notification);
    }
#endif
}

::UMS_Data::AuthType AuthSystem::getType() const
{
    return m_type;
}

void AuthSystem::setType(::UMS_Data::AuthType _type)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::UMS_Data::AuthType _old_type = m_type;
#endif
    m_type = _type;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getAuthSystem__type(),
                _old_type,
                m_type
        );
        eNotify(&notification);
    }
#endif
}

::UMS_Data::StatusType AuthSystem::getStatus() const
{
    return m_status;
}

void AuthSystem::setStatus(::UMS_Data::StatusType _status)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getAuthSystem__status(),
                _old_status,
                m_status
        );
        eNotify(&notification);
    }
#endif
}

// References
::UMS_Data::AuthSystemOptions_ptr AuthSystem::getOptions()
{
    return m_options;
}

void AuthSystem::setOptions(::UMS_Data::AuthSystemOptions_ptr _options)
{
    ::UMS_Data::AuthSystemOptions_ptr _old_options = m_options;

    m_options = _options;

#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getAuthSystem__options(),
                _old_options,
                m_options
        );
        eNotify(&notification);
    }
#endif

}

