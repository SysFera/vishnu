// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/RestartOp.cpp
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

#include "RestartOp.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "IMS_Data/IMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::IMS_Data;

// Default constructor
RestartOp::RestartOp() :
    m_vishnuConf(""), m_sedType(0)
{

    /*PROTECTED REGION ID(RestartOpImpl__RestartOpImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

RestartOp::~RestartOp()
{
}

/*PROTECTED REGION ID(RestartOp.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& RestartOp::getVishnuConf() const
{
    return m_vishnuConf;
}

void RestartOp::setVishnuConf(::ecore::EString const& _vishnuConf)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_vishnuConf = m_vishnuConf;
#endif
    m_vishnuConf = _vishnuConf;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getRestartOp__vishnuConf(),
                _old_vishnuConf,
                m_vishnuConf
        );
        eNotify(&notification);
    }
#endif
}

::IMS_Data::SeDType RestartOp::getSedType() const
{
    return m_sedType;
}

void RestartOp::setSedType(::IMS_Data::SeDType _sedType)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::IMS_Data::SeDType _old_sedType = m_sedType;
#endif
    m_sedType = _sedType;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getRestartOp__sedType(),
                _old_sedType,
                m_sedType
        );
        eNotify(&notification);
    }
#endif
}

// References

