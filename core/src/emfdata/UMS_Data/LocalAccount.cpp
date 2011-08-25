// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/LocalAccount.cpp
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

#include "LocalAccount.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
LocalAccount::LocalAccount() :
    m_userId(""), m_acLogin(""), m_homeDirectory("")
{

    /*PROTECTED REGION ID(LocalAccountImpl__LocalAccountImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

LocalAccount::~LocalAccount()
{
}

/*PROTECTED REGION ID(LocalAccount.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& LocalAccount::getUserId() const
{
    return m_userId;
}

void LocalAccount::setUserId(::ecore::EString const& _userId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getLocalAccount__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& LocalAccount::getMachineId() const
{
    return m_machineId;
}

void LocalAccount::setMachineId(::ecore::EString const& _machineId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getLocalAccount__machineId(),
                _old_machineId,
                m_machineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& LocalAccount::getAcLogin() const
{
    return m_acLogin;
}

void LocalAccount::setAcLogin(::ecore::EString const& _acLogin)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_acLogin = m_acLogin;
#endif
    m_acLogin = _acLogin;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getLocalAccount__acLogin(),
                _old_acLogin,
                m_acLogin
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& LocalAccount::getSshKeyPath() const
{
    return m_sshKeyPath;
}

void LocalAccount::setSshKeyPath(::ecore::EString const& _sshKeyPath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_sshKeyPath = m_sshKeyPath;
#endif
    m_sshKeyPath = _sshKeyPath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getLocalAccount__sshKeyPath(),
                _old_sshKeyPath,
                m_sshKeyPath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& LocalAccount::getHomeDirectory() const
{
    return m_homeDirectory;
}

void LocalAccount::setHomeDirectory(::ecore::EString const& _homeDirectory)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_homeDirectory = m_homeDirectory;
#endif
    m_homeDirectory = _homeDirectory;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getLocalAccount__homeDirectory(),
                _old_homeDirectory,
                m_homeDirectory
        );
        eNotify(&notification);
    }
#endif
}

// References

