// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/SysInfoOp.cpp
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

#include "SysInfoOp.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "IMS_Data/IMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::IMS_Data;

// Default constructor
SysInfoOp::SysInfoOp() :
    m_machineId("")
{

    /*PROTECTED REGION ID(SysInfoOpImpl__SysInfoOpImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

SysInfoOp::~SysInfoOp()
{
}

/*PROTECTED REGION ID(SysInfoOp.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& SysInfoOp::getMachineId() const
{
    return m_machineId;
}

void SysInfoOp::setMachineId(::ecore::EString const& _machineId)
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
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getSysInfoOp__machineId(),
                _old_machineId,
                m_machineId
        );
        eNotify(&notification);
    }
#endif
}

// References

