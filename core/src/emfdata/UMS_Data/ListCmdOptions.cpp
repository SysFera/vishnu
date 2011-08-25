// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListCmdOptions.cpp
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

#include "ListCmdOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
ListCmdOptions::ListCmdOptions() :
    m_adminListOption(false), m_sessionId(""), m_startDateOption(-1),
            m_endDateOption(-1)
{

    /*PROTECTED REGION ID(ListCmdOptionsImpl__ListCmdOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListCmdOptions::~ListCmdOptions()
{
}

/*PROTECTED REGION ID(ListCmdOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EBoolean ListCmdOptions::isAdminListOption() const
{
    return m_adminListOption;
}

void ListCmdOptions::setAdminListOption(::ecore::EBoolean _adminListOption)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListCmdOptions__adminListOption(),
                _old_adminListOption,
                m_adminListOption
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListCmdOptions::getUserId() const
{
    return m_userId;
}

void ListCmdOptions::setUserId(::ecore::EString const& _userId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListCmdOptions__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListCmdOptions::getSessionId() const
{
    return m_sessionId;
}

void ListCmdOptions::setSessionId(::ecore::EString const& _sessionId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListCmdOptions__sessionId(),
                _old_sessionId,
                m_sessionId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong ListCmdOptions::getStartDateOption() const
{
    return m_startDateOption;
}

void ListCmdOptions::setStartDateOption(::ecore::ELong _startDateOption)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListCmdOptions__startDateOption(),
                _old_startDateOption,
                m_startDateOption
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong ListCmdOptions::getEndDateOption() const
{
    return m_endDateOption;
}

void ListCmdOptions::setEndDateOption(::ecore::ELong _endDateOption)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListCmdOptions__endDateOption(),
                _old_endDateOption,
                m_endDateOption
        );
        eNotify(&notification);
    }
#endif
}

// References

