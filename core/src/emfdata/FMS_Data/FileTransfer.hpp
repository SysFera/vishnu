// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/FileTransfer.hpp
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

/**
 * \file FileTransfer.hpp
 * \brief The FileTransfer class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef FMS_DATA_FILETRANSFER_HPP
#define FMS_DATA_FILETRANSFER_HPP

#include <FMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
/*PROTECTED REGION ID(FileTransfer_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace FMS_Data
{

    /**
     * \class FileTransfer
     * \brief Implementation of the FileTransfer class
     */
    class VISHNU_API_LIB FileTransfer: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for FileTransfer
         */
        FileTransfer();
        /**
         * \brief The destructor for FileTransfer
         */
        virtual ~FileTransfer();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the transferId
         * \return The transferId attribute value
         **/
        ::ecore::EString const& getTransferId() const;
        /**
         * \brief To set the transferId
         * \param _transferId The transferId value
         **/
        void setTransferId(::ecore::EString const& _transferId);

        /**
         * \brief To get the status
         * \return The status attribute value
         **/
        ::FMS_Data::Status getStatus() const;
        /**
         * \brief To set the status
         * \param _status The status value
         **/
        void setStatus(::FMS_Data::Status _status);

        /**
         * \brief To get the userId
         * \return The userId attribute value
         **/
        ::ecore::EString const& getUserId() const;
        /**
         * \brief To set the userId
         * \param _userId The userId value
         **/
        void setUserId(::ecore::EString const& _userId);

        /**
         * \brief To get the clientMachineId
         * \return The clientMachineId attribute value
         **/
        ::ecore::EString const& getClientMachineId() const;
        /**
         * \brief To set the clientMachineId
         * \param _clientMachineId The clientMachineId value
         **/
        void setClientMachineId(::ecore::EString const& _clientMachineId);

        /**
         * \brief To get the sourceMachineId
         * \return The sourceMachineId attribute value
         **/
        ::ecore::EString const& getSourceMachineId() const;
        /**
         * \brief To set the sourceMachineId
         * \param _sourceMachineId The sourceMachineId value
         **/
        void setSourceMachineId(::ecore::EString const& _sourceMachineId);

        /**
         * \brief To get the destinationMachineId
         * \return The destinationMachineId attribute value
         **/
        ::ecore::EString const& getDestinationMachineId() const;
        /**
         * \brief To set the destinationMachineId
         * \param _destinationMachineId The destinationMachineId value
         **/
        void setDestinationMachineId(
                ::ecore::EString const& _destinationMachineId);

        /**
         * \brief To get the sourceFilePath
         * \return The sourceFilePath attribute value
         **/
        ::ecore::EString const& getSourceFilePath() const;
        /**
         * \brief To set the sourceFilePath
         * \param _sourceFilePath The sourceFilePath value
         **/
        void setSourceFilePath(::ecore::EString const& _sourceFilePath);

        /**
         * \brief To get the destinationFilePath
         * \return The destinationFilePath attribute value
         **/
        ::ecore::EString const& getDestinationFilePath() const;
        /**
         * \brief To set the destinationFilePath
         * \param _destinationFilePath The destinationFilePath value
         **/
        void setDestinationFilePath(
                ::ecore::EString const& _destinationFilePath);

        /**
         * \brief To get the size
         * \return The size attribute value
         **/
        ::ecore::ELong getSize() const;
        /**
         * \brief To set the size
         * \param _size The size value
         **/
        void setSize(::ecore::ELong _size);

        /**
         * \brief To get the start_time
         * \return The start_time attribute value
         **/
        ::ecore::ELong getStart_time() const;
        /**
         * \brief To set the start_time
         * \param _start_time The start_time value
         **/
        void setStart_time(::ecore::ELong _start_time);

        /**
         * \brief To get the trCommand
         * \return The trCommand attribute value
         **/
        ::FMS_Data::TransferCommand getTrCommand() const;
        /**
         * \brief To set the trCommand
         * \param _trCommand The trCommand value
         **/
        void setTrCommand(::FMS_Data::TransferCommand _trCommand);

        /**
         * \brief To get the errorMsg
         * \return The errorMsg attribute value
         **/
        ::ecore::EString const& getErrorMsg() const;
        /**
         * \brief To set the errorMsg
         * \param _errorMsg The errorMsg value
         **/
        void setErrorMsg(::ecore::EString const& _errorMsg);

        // References


        /*PROTECTED REGION ID(FileTransfer) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

        // EObjectImpl
        virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID,
                ::ecore::EBoolean _resolve);
        virtual void eSet(::ecore::EInt _featureID,
                ::ecore::EJavaObject const& _newValue);
        virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID);
        virtual void eUnset(::ecore::EInt _featureID);
        virtual ::ecore::EClass_ptr _eClass();

        /*PROTECTED REGION ID(FileTransferImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_transferId;

        ::FMS_Data::Status m_status;

        ::ecore::EString m_userId;

        ::ecore::EString m_clientMachineId;

        ::ecore::EString m_sourceMachineId;

        ::ecore::EString m_destinationMachineId;

        ::ecore::EString m_sourceFilePath;

        ::ecore::EString m_destinationFilePath;

        ::ecore::ELong m_size;

        ::ecore::ELong m_start_time;

        ::FMS_Data::TransferCommand m_trCommand;

        ::ecore::EString m_errorMsg;

        // References

    };

} // FMS_Data

#endif // FMS_DATA_FILETRANSFER_HPP
