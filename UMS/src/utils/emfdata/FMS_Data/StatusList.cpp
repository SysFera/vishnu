// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/StatusList.cpp
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

#include "StatusList.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "FMS_Data/FMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::FMS_Data;

// Default constructor
StatusList::StatusList()
{

    /*PROTECTED REGION ID(StatusListImpl__StatusListImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

StatusList::~StatusList()
{
}

/*PROTECTED REGION ID(StatusList.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

std::vector< ::FMS_Data::Status > const& StatusList::getStatus() const
{
    return m_status;
}

void StatusList::setStatus(std::vector< ::FMS_Data::Status > _status)
{
#ifdef ECORECPP_NOTIFICATION_API
    std::vector< ::FMS_Data::Status > _old_status = m_status;
#endif
    m_status = _status;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getStatusList__status(),
                _old_status,
                m_status
        );
        eNotify(&notification);
    }
#endif
}

void StatusList::addStatus(::FMS_Data::Status const& _new_element_in_status)
{
    m_status.push_back(_new_element_in_status);
}

void StatusList::setStatusAt(size_t _position,
        ::FMS_Data::Status const& _new_element_in_status)
{
    if (_position >= m_status.size())
        m_status.resize(_position + 1, 0);

    m_status[_position] = _new_element_in_status;
}

void StatusList::deleteStatusAt(size_t _position)
{
    if (_position >= m_status.size())
        throw "Attribute counter out of bounds!";

    // TODO: This is not actually quite true
    if (_position == m_status.size() - 1)
        m_status.resize(_position);
}

// References

