// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/StringList.cpp
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

#include "StringList.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "FMS_Data/FMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::FMS_Data;

// Default constructor
StringList::StringList()
{

    /*PROTECTED REGION ID(StringListImpl__StringListImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

StringList::~StringList()
{
}

/*PROTECTED REGION ID(StringList.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

std::vector< ::ecore::EString > const& StringList::getStrings() const
{
    return m_strings;
}

void StringList::setStrings(std::vector< ::ecore::EString > const& _strings)
{
#ifdef ECORECPP_NOTIFICATION_API
    std::vector< ::ecore::EString > _old_strings = m_strings;
#endif
    m_strings = _strings;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getStringList__strings(),
                _old_strings,
                m_strings
        );
        eNotify(&notification);
    }
#endif
}

void StringList::addStrings(::ecore::EString const& _new_element_in_strings)
{
    m_strings.push_back(_new_element_in_strings);
}

void StringList::setStringsAt(size_t _position,
        ::ecore::EString const& _new_element_in_strings)
{
    if (_position >= m_strings.size())
        m_strings.resize(_position + 1, 0);

    m_strings[_position] = _new_element_in_strings;
}

void StringList::deleteStringsAt(size_t _position)
{
    if (_position >= m_strings.size())
        throw "Attribute counter out of bounds!";

    // TODO: This is not actually quite true
    if (_position == m_strings.size() - 1)
        m_strings.resize(_position);
}

// References

