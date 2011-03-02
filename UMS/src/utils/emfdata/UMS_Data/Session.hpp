// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/Session.hpp
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
 * \file Session.hpp
 * \brief The Session class
 * \author Generated file
 * \date 20/02/2011
 */

#ifndef UMS_DATA_SESSION_HPP
#define UMS_DATA_SESSION_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Session_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    class Session: public virtual ::ecore::EObject

    {
    public:
        Session();

        virtual ~Session();

        virtual void _initialize();

        // Operations


        // Attributes
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
         * \brief To get the sessionKey
         * \return The sessionKey attribute value
         **/
        ::ecore::EString const& getSessionKey() const;
        /**
         * \brief To set the sessionKey
         * \param _sessionKey The sessionKey value
         **/
        void setSessionKey(::ecore::EString const& _sessionKey);

        /**
         * \brief To get the dateLastConnect
         * \return The dateLastConnect attribute value
         **/
        ::ecore::ELong getDateLastConnect() const;
        /**
         * \brief To set the dateLastConnect
         * \param _dateLastConnect The dateLastConnect value
         **/
        void setDateLastConnect(::ecore::ELong _dateLastConnect);

        /**
         * \brief To get the dateCreation
         * \return The dateCreation attribute value
         **/
        ::ecore::ELong getDateCreation() const;
        /**
         * \brief To set the dateCreation
         * \param _dateCreation The dateCreation value
         **/
        void setDateCreation(::ecore::ELong _dateCreation);

        /**
         * \brief To get the dateClosure
         * \return The dateClosure attribute value
         **/
        ::ecore::ELong getDateClosure() const;
        /**
         * \brief To set the dateClosure
         * \param _dateClosure The dateClosure value
         **/
        void setDateClosure(::ecore::ELong _dateClosure);

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
         * \brief To get the closePolicy
         * \return The closePolicy attribute value
         **/
        ::UMS_Data::SessionCloseType getClosePolicy() const;
        /**
         * \brief To set the closePolicy
         * \param _closePolicy The closePolicy value
         **/
        void setClosePolicy(::UMS_Data::SessionCloseType _closePolicy);

        /**
         * \brief To get the timeout
         * \return The timeout attribute value
         **/
        ::ecore::ELong getTimeout() const;
        /**
         * \brief To set the timeout
         * \param _timeout The timeout value
         **/
        void setTimeout(::ecore::ELong _timeout);

        // References


        /*PROTECTED REGION ID(Session) START*/
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

        /*PROTECTED REGION ID(SessionImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_sessionId;

        ::ecore::EString m_userId;

        ::ecore::EString m_sessionKey;

        ::ecore::ELong m_dateLastConnect;

        ::ecore::ELong m_dateCreation;

        ::ecore::ELong m_dateClosure;

        ::UMS_Data::StatusType m_status;

        ::UMS_Data::SessionCloseType m_closePolicy;

        ::ecore::ELong m_timeout;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_SESSION_HPP
