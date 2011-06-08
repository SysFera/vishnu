// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/FileTransfer.cpp
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

#include "FileTransfer.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "FMS_Data/FMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::FMS_Data;

// Default constructor
FileTransfer::FileTransfer() :
    m_status(4), m_start_time(0), m_trCommand(2)
{

    /*PROTECTED REGION ID(FileTransferImpl__FileTransferImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

FileTransfer::~FileTransfer()
{
}

/*PROTECTED REGION ID(FileTransfer.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& FileTransfer::getTransferId() const
{
    return m_transferId;
}

void FileTransfer::setTransferId(::ecore::EString const& _transferId)
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
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__transferId(),
                _old_transferId,
                m_transferId
        );
        eNotify(&notification);
    }
#endif
}

::FMS_Data::Status FileTransfer::getStatus() const
{
    return m_status;
}

void FileTransfer::setStatus(::FMS_Data::Status _status)
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
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__status(),
                _old_status,
                m_status
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& FileTransfer::getUserId() const
{
    return m_userId;
}

void FileTransfer::setUserId(::ecore::EString const& _userId)
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
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__userId(),
                _old_userId,
                m_userId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& FileTransfer::getClientMachineId() const
{
    return m_clientMachineId;
}

void FileTransfer::setClientMachineId(::ecore::EString const& _clientMachineId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_clientMachineId = m_clientMachineId;
#endif
    m_clientMachineId = _clientMachineId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__clientMachineId(),
                _old_clientMachineId,
                m_clientMachineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& FileTransfer::getSourceMachineId() const
{
    return m_sourceMachineId;
}

void FileTransfer::setSourceMachineId(::ecore::EString const& _sourceMachineId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_sourceMachineId = m_sourceMachineId;
#endif
    m_sourceMachineId = _sourceMachineId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__sourceMachineId(),
                _old_sourceMachineId,
                m_sourceMachineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& FileTransfer::getDestinationMachineId() const
{
    return m_destinationMachineId;
}

void FileTransfer::setDestinationMachineId(
        ::ecore::EString const& _destinationMachineId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_destinationMachineId = m_destinationMachineId;
#endif
    m_destinationMachineId = _destinationMachineId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__destinationMachineId(),
                _old_destinationMachineId,
                m_destinationMachineId
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& FileTransfer::getSourceFilePath() const
{
    return m_sourceFilePath;
}

void FileTransfer::setSourceFilePath(::ecore::EString const& _sourceFilePath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_sourceFilePath = m_sourceFilePath;
#endif
    m_sourceFilePath = _sourceFilePath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__sourceFilePath(),
                _old_sourceFilePath,
                m_sourceFilePath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& FileTransfer::getDestinationFilePath() const
{
    return m_destinationFilePath;
}

void FileTransfer::setDestinationFilePath(
        ::ecore::EString const& _destinationFilePath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_destinationFilePath = m_destinationFilePath;
#endif
    m_destinationFilePath = _destinationFilePath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__destinationFilePath(),
                _old_destinationFilePath,
                m_destinationFilePath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong FileTransfer::getSize() const
{
    return m_size;
}

void FileTransfer::setSize(::ecore::ELong _size)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_size = m_size;
#endif
    m_size = _size;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__size(),
                _old_size,
                m_size
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong FileTransfer::getStart_time() const
{
    return m_start_time;
}

void FileTransfer::setStart_time(::ecore::ELong _start_time)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_start_time = m_start_time;
#endif
    m_start_time = _start_time;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__start_time(),
                _old_start_time,
                m_start_time
        );
        eNotify(&notification);
    }
#endif
}

::FMS_Data::TransferCommand FileTransfer::getTrCommand() const
{
    return m_trCommand;
}

void FileTransfer::setTrCommand(::FMS_Data::TransferCommand _trCommand)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::FMS_Data::TransferCommand _old_trCommand = m_trCommand;
#endif
    m_trCommand = _trCommand;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__trCommand(),
                _old_trCommand,
                m_trCommand
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& FileTransfer::getErrorMsg() const
{
    return m_errorMsg;
}

void FileTransfer::setErrorMsg(::ecore::EString const& _errorMsg)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_errorMsg = m_errorMsg;
#endif
    m_errorMsg = _errorMsg;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileTransfer__errorMsg(),
                _old_errorMsg,
                m_errorMsg
        );
        eNotify(&notification);
    }
#endif
}

// References

