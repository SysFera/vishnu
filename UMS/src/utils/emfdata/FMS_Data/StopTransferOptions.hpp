// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/StopTransferOptions.hpp
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
 * \file StopTransferOptions.hpp
 * \brief The StopTransferOptions class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef FMS_DATA_STOPTRANSFEROPTIONS_HPP
#define FMS_DATA_STOPTRANSFEROPTIONS_HPP

#include <FMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(StopTransferOptions_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace FMS_Data
{

    /**
     * \class StopTransferOptions
     * \brief Implementation of the StopTransferOptions class
     */
    class StopTransferOptions: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for StopTransferOptions
         */
        StopTransferOptions();
        /**
         * \brief The destructor for StopTransferOptions
         */
        virtual ~StopTransferOptions();

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
         * \brief To get the fromMachineId
         * \return The fromMachineId attribute value
         **/
        ::ecore::EString const& getFromMachineId() const;
        /**
         * \brief To set the fromMachineId
         * \param _fromMachineId The fromMachineId value
         **/
        void setFromMachineId(::ecore::EString const& _fromMachineId);

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

        // References


        /*PROTECTED REGION ID(StopTransferOptions) START*/
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

        /*PROTECTED REGION ID(StopTransferOptionsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_transferId;

        ::ecore::EString m_fromMachineId;

        ::ecore::EString m_userId;

        // References

    };

} // FMS_Data

#endif // FMS_DATA_STOPTRANSFEROPTIONS_HPP
