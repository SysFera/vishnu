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

    class ListSessionOptions: public virtual ::ecore::EObject

    {
    public:
        ListSessionOptions();

        virtual ~ListSessionOptions();

        virtual void _initialize();

        // Operations


        // Attributes
        ::UMS_Data::StatusType getStatus() const;
        void setStatus(::UMS_Data::StatusType _status);

        ::UMS_Data::SessionCloseType getSessionClosePolicy() const;
        void setSessionClosePolicy(
                ::UMS_Data::SessionCloseType _sessionClosePolicy);

        ::ecore::EInt getSessionInactivityDelay() const;
        void setSessionInactivityDelay(::ecore::EInt _sessionInactivityDelay);

        ::ecore::EString const& getMachineId() const;
        void setMachineId(::ecore::EString const& _machineId);

        ::ecore::EBoolean isAdminListOption() const;
        void setAdminListOption(::ecore::EBoolean _adminListOption);

        ::ecore::EString const& getUserId() const;
        void setUserId(::ecore::EString const& _userId);

        ::ecore::EString const& getSessionId() const;
        void setSessionId(::ecore::EString const& _sessionId);

        ::ecore::ELong getStartDateOption() const;
        void setStartDateOption(::ecore::ELong _startDateOption);

        ::ecore::ELong getEndDateOption() const;
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
