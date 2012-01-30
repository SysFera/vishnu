// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListAuthSysOptions.cpp
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

#include "ListAuthSysOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
ListAuthSysOptions::ListAuthSysOptions() :
    m_listAllAuthSystems(false), m_authSystemId("")
{

    /*PROTECTED REGION ID(ListAuthSysOptionsImpl__ListAuthSysOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListAuthSysOptions::~ListAuthSysOptions()
{
}

/*PROTECTED REGION ID(ListAuthSysOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EBoolean ListAuthSysOptions::isListAllAuthSystems() const
{
    return m_listAllAuthSystems;
}

void ListAuthSysOptions::setListAllAuthSystems(
        ::ecore::EBoolean _listAllAuthSystems)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EBoolean _old_listAllAuthSystems = m_listAllAuthSystems;
#endif
    m_listAllAuthSystems = _listAllAuthSystems;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListAuthSysOptions__listAllAuthSystems(),
                _old_listAllAuthSystems,
                m_listAllAuthSystems
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListAuthSysOptions::getUserId() const
{
    return m_userId;
}

void ListAuthSysOptions::setUserId(::ecore::EString const& _userId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListAuthSysOptions__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListAuthSysOptions::getAuthSystemId() const
{
    return m_authSystemId;
}

void ListAuthSysOptions::setAuthSystemId(::ecore::EString const& _authSystemId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListAuthSysOptions__authSystemId(),
                _old_authSystemId,
                m_authSystemId
        );
        eNotify(&notification);
    }
#endif
}

// References

