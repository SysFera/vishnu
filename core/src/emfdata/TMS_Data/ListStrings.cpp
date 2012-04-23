// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListStrings.cpp
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

#include "ListStrings.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
ListStrings::ListStrings() :
    m_string(0)
{

    /*PROTECTED REGION ID(ListStringsImpl__ListStringsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListStrings::~ListStrings()
{
}

/*PROTECTED REGION ID(ListStrings.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

std::vector< ::ecore::EString > const& ListStrings::getString() const
{
    return m_string;
}

void ListStrings::setString(std::vector< ::ecore::EString > const& _string)
{
#ifdef ECORECPP_NOTIFICATION_API
    std::vector< ::ecore::EString > _old_string = m_string;
#endif
    m_string = _string;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListStrings__string(),
                _old_string,
                m_string
        );
        eNotify(&notification);
    }
#endif
}

void ListStrings::addString(::ecore::EString const& _new_element_in_string)
{
    m_string.push_back(_new_element_in_string);
}

void ListStrings::setStringAt(size_t _position,
        ::ecore::EString const& _new_element_in_string)
{
    if (_position >= m_string.size())
        m_string.resize(_position + 1, 0);

    m_string[_position] = _new_element_in_string;
}

void ListStrings::deleteStringAt(size_t _position)
{
    if (_position >= m_string.size())
        throw "Attribute counter out of bounds!";

    // TODO: This is not actually quite true
    if (_position == m_string.size() - 1)
        m_string.resize(_position);
}

// References

