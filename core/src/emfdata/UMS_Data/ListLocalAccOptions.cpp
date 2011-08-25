// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListLocalAccOptions.cpp
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

#include "ListLocalAccOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
ListLocalAccOptions::ListLocalAccOptions() :
    m_adminListOption(false), m_machineId("")
{

    /*PROTECTED REGION ID(ListLocalAccOptionsImpl__ListLocalAccOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListLocalAccOptions::~ListLocalAccOptions()
{
}

/*PROTECTED REGION ID(ListLocalAccOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EBoolean ListLocalAccOptions::isAdminListOption() const
{
    return m_adminListOption;
}

void ListLocalAccOptions::setAdminListOption(::ecore::EBoolean _adminListOption)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListLocalAccOptions__adminListOption(),
                _old_adminListOption,
                m_adminListOption
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListLocalAccOptions::getUserId() const
{
    return m_userId;
}

void ListLocalAccOptions::setUserId(::ecore::EString const& _userId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListLocalAccOptions__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListLocalAccOptions::getMachineId() const
{
    return m_machineId;
}

void ListLocalAccOptions::setMachineId(::ecore::EString const& _machineId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListLocalAccOptions__machineId(),
                _old_machineId,
                m_machineId
        );
        eNotify(&notification);
    }
#endif
}

// References

