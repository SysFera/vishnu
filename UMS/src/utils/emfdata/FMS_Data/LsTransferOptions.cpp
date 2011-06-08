// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/LsTransferOptions.cpp
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

#include "LsTransferOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "FMS_Data/FMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::FMS_Data;

// Default constructor
LsTransferOptions::LsTransferOptions() :
    m_status(4)
{

    /*PROTECTED REGION ID(LsTransferOptionsImpl__LsTransferOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

LsTransferOptions::~LsTransferOptions()
{
}

/*PROTECTED REGION ID(LsTransferOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& LsTransferOptions::getTransferId() const
{
    return m_transferId;
}

void LsTransferOptions::setTransferId(::ecore::EString const& _transferId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_transferId = m_transferId;
#endif
    m_transferId = _transferId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getLsTransferOptions__transferId(),
                _old_transferId,
                m_transferId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& LsTransferOptions::getFromMachineId() const
{
    return m_fromMachineId;
}

void LsTransferOptions::setFromMachineId(::ecore::EString const& _fromMachineId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_fromMachineId = m_fromMachineId;
#endif
    m_fromMachineId = _fromMachineId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getLsTransferOptions__fromMachineId(),
                _old_fromMachineId,
                m_fromMachineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& LsTransferOptions::getUserId() const
{
    return m_userId;
}

void LsTransferOptions::setUserId(::ecore::EString const& _userId)
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
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getLsTransferOptions__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::FMS_Data::Status LsTransferOptions::getStatus() const
{
    return m_status;
}

void LsTransferOptions::setStatus(::FMS_Data::Status _status)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::FMS_Data::Status _old_status = m_status;
#endif
    m_status = _status;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getLsTransferOptions__status(),
                _old_status,
                m_status
        );
        eNotify(&notification);
    }
#endif
}

// References

