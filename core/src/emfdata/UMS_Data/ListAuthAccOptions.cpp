// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListAuthAccOptions.cpp
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

#include "ListAuthAccOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
ListAuthAccOptions::ListAuthAccOptions() :
    m_listAll(false), m_authSystemId("")
{

    /*PROTECTED REGION ID(ListAuthAccOptionsImpl__ListAuthAccOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListAuthAccOptions::~ListAuthAccOptions()
{
}

/*PROTECTED REGION ID(ListAuthAccOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EBoolean ListAuthAccOptions::isListAll() const
{
    return m_listAll;
}

void ListAuthAccOptions::setListAll(::ecore::EBoolean _listAll)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EBoolean _old_listAll = m_listAll;
#endif
    m_listAll = _listAll;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListAuthAccOptions__listAll(),
                _old_listAll,
                m_listAll
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListAuthAccOptions::getUserId() const
{
    return m_userId;
}

void ListAuthAccOptions::setUserId(::ecore::EString const& _userId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListAuthAccOptions__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListAuthAccOptions::getAuthSystemId() const
{
    return m_authSystemId;
}

void ListAuthAccOptions::setAuthSystemId(::ecore::EString const& _authSystemId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListAuthAccOptions__authSystemId(),
                _old_authSystemId,
                m_authSystemId
        );
        eNotify(&notification);
    }
#endif
}

// References

