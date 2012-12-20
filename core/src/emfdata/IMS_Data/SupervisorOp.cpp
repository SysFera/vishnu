// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/SupervisorOp.cpp
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

#include "SupervisorOp.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "IMS_Data/IMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::IMS_Data;

// Default constructor
SupervisorOp::SupervisorOp() :
    m_URI(""), m_name("")
{

    /*PROTECTED REGION ID(SupervisorOpImpl__SupervisorOpImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

SupervisorOp::~SupervisorOp()
{
}

/*PROTECTED REGION ID(SupervisorOp.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& SupervisorOp::getURI() const
{
    return m_URI;
}

void SupervisorOp::setURI(::ecore::EString const& _URI)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_URI = m_URI;
#endif
    m_URI = _URI;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getSupervisorOp__URI(),
                _old_URI,
                m_URI
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SupervisorOp::getName() const
{
    return m_name;
}

void SupervisorOp::setName(::ecore::EString const& _name)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_name = m_name;
#endif
    m_name = _name;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getSupervisorOp__name(),
                _old_name,
                m_name
        );
        eNotify(&notification);
    }
#endif
}

// References

