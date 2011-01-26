// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/User.cpp
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

#include "User.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
User::User() :
    m_userId(""), m_privilege(-1)
{

    /*PROTECTED REGION ID(UserImpl__UserImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

User::~User()
{
}

/*PROTECTED REGION ID(User.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& User::getUserId() const
{
    return m_userId;
}

void User::setUserId(::ecore::EString const& _userId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getUser__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& User::getPassword() const
{
    return m_password;
}

void User::setPassword(::ecore::EString const& _password)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_password = m_password;
#endif
    m_password = _password;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getUser__password(),
                _old_password,
                m_password
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& User::getFirstname() const
{
    return m_firstname;
}

void User::setFirstname(::ecore::EString const& _firstname)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_firstname = m_firstname;
#endif
    m_firstname = _firstname;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getUser__firstname(),
                _old_firstname,
                m_firstname
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& User::getLastname() const
{
    return m_lastname;
}

void User::setLastname(::ecore::EString const& _lastname)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_lastname = m_lastname;
#endif
    m_lastname = _lastname;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getUser__lastname(),
                _old_lastname,
                m_lastname
        );
        eNotify(&notification);
    }
#endif
}

::UMS_Data::PrivilegeType User::getPrivilege() const
{
    return m_privilege;
}

void User::setPrivilege(::UMS_Data::PrivilegeType _privilege)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::UMS_Data::PrivilegeType _old_privilege = m_privilege;
#endif
    m_privilege = _privilege;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getUser__privilege(),
                _old_privilege,
                m_privilege
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& User::getEmail() const
{
    return m_email;
}

void User::setEmail(::ecore::EString const& _email)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_email = m_email;
#endif
    m_email = _email;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getUser__email(),
                _old_email,
                m_email
        );
        eNotify(&notification);
    }
#endif
}

// References

