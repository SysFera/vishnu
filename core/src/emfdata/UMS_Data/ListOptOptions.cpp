// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListOptOptions.cpp
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

#include "ListOptOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
ListOptOptions::ListOptOptions() :
    m_listAllDeftValue(false), m_optionName("")
{

    /*PROTECTED REGION ID(ListOptOptionsImpl__ListOptOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListOptOptions::~ListOptOptions()
{
}

/*PROTECTED REGION ID(ListOptOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EBoolean ListOptOptions::isListAllDeftValue() const
{
    return m_listAllDeftValue;
}

void ListOptOptions::setListAllDeftValue(::ecore::EBoolean _listAllDeftValue)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EBoolean _old_listAllDeftValue = m_listAllDeftValue;
#endif
    m_listAllDeftValue = _listAllDeftValue;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListOptOptions__listAllDeftValue(),
                _old_listAllDeftValue,
                m_listAllDeftValue
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListOptOptions::getUserId() const
{
    return m_userId;
}

void ListOptOptions::setUserId(::ecore::EString const& _userId)
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
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListOptOptions__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& ListOptOptions::getOptionName() const
{
    return m_optionName;
}

void ListOptOptions::setOptionName(::ecore::EString const& _optionName)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_optionName = m_optionName;
#endif
    m_optionName = _optionName;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getListOptOptions__optionName(),
                _old_optionName,
                m_optionName
        );
        eNotify(&notification);
    }
#endif
}

// References

