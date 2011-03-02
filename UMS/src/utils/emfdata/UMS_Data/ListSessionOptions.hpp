// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListSessionOptions.hpp
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
 * \file ListSessionOptions.hpp
 * \brief The ListSessionOptions class
 * \author Generated file
 * \date 20/02/2011
 */

#ifndef UMS_DATA_LISTSESSIONOPTIONS_HPP
#define UMS_DATA_LISTSESSIONOPTIONS_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ListSessionOptions_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class ListSessionOptions
     * \brief Implementation of the ListSessionOptions class
     */
    class ListSessionOptions: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ListSessionOptions
         */
        ListSessionOptions();
        /**
         * \brief The destructor for ListSessionOptions
         */
        virtual ~ListSessionOptions();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the status
         * \return The status attribute value
         **/
        ::UMS_Data::StatusType getStatus() const;
        /**
         * \brief To set the status
         * \param _status The status value
         **/
        void setStatus(::UMS_Data::StatusType _status);

        /**
         * \brief To get the sessionClosePolicy
         * \return The sessionClosePolicy attribute value
         **/
        ::UMS_Data::SessionCloseType getSessionClosePolicy() const;
        /**
         * \brief To set the sessionClosePolicy
         * \param _sessionClosePolicy The sessionClosePolicy value
         **/
        void setSessionClosePolicy(
                ::UMS_Data::SessionCloseType _sessionClosePolicy);

        /**
         * \brief To get the sessionInactivityDelay
         * \return The sessionInactivityDelay attribute value
         **/
        ::ecore::EInt getSessionInactivityDelay() const;
        /**
         * \brief To set the sessionInactivityDelay
         * \param _sessionInactivityDelay The sessionInactivityDelay value
         **/
        void setSessionInactivityDelay(::ecore::EInt _sessionInactivityDelay);

        /**
         * \brief To get the machineId
         * \return The machineId attribute value
         **/
        ::ecore::EString const& getMachineId() const;
        /**
         * \brief To set the machineId
         * \param _machineId The machineId value
         **/
        void setMachineId(::ecore::EString const& _machineId);

        /**
         * \brief To get the adminListOption
         * \return The adminListOption attribute value
         **/
        ::ecore::EBoolean isAdminListOption() const;
        /**
         * \brief To set the adminListOption
         * \param _adminListOption The adminListOption value
         **/
        void setAdminListOption(::ecore::EBoolean _adminListOption);

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
         * \brief To get the sessionId
         * \return The sessionId attribute value
         **/
        ::ecore::EString const& getSessionId() const;
        /**
         * \brief To set the sessionId
         * \param _sessionId The sessionId value
         **/
        void setSessionId(::ecore::EString const& _sessionId);

        /**
         * \brief To get the startDateOption
         * \return The startDateOption attribute value
         **/
        ::ecore::ELong getStartDateOption() const;
        /**
         * \brief To set the startDateOption
         * \param _startDateOption The startDateOption value
         **/
        void setStartDateOption(::ecore::ELong _startDateOption);

        /**
         * \brief To get the endDateOption
         * \return The endDateOption attribute value
         **/
        ::ecore::ELong getEndDateOption() const;
        /**
         * \brief To set the endDateOption
         * \param _endDateOption The endDateOption value
         **/
        void setEndDateOption(::ecore::ELong _endDateOption);

        // References


        /*PROTECTED REGION ID(ListSessionOptions) START*/
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

        /*PROTECTED REGION ID(ListSessionOptionsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::UMS_Data::StatusType m_status;

        ::UMS_Data::SessionCloseType m_sessionClosePolicy;

        ::ecore::EInt m_sessionInactivityDelay;

        ::ecore::EString m_machineId;

        ::ecore::EBoolean m_adminListOption;

        ::ecore::EString m_userId;

        ::ecore::EString m_sessionId;

        ::ecore::ELong m_startDateOption;

        ::ecore::ELong m_endDateOption;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_LISTSESSIONOPTIONS_HPP
