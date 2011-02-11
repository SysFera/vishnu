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
        ::ecore::EString const& getSessionId() const;
        void setSessionId(::ecore::EString const& _sessionId);

        ::ecore::EString const& getUserId() const;
        void setUserId(::ecore::EString const& _userId);

        ::ecore::EString const& getSessionKey() const;
        void setSessionKey(::ecore::EString const& _sessionKey);

        ::ecore::ELong getDateLastConnect() const;
        void setDateLastConnect(::ecore::ELong _dateLastConnect);

        ::ecore::ELong getDateCreation() const;
        void setDateCreation(::ecore::ELong _dateCreation);

        ::ecore::ELong getDateClosure() const;
        void setDateClosure(::ecore::ELong _dateClosure);

        ::UMS_Data::StatusType getStatus() const;
        void setStatus(::UMS_Data::StatusType _status);

        ::UMS_Data::SessionCloseType getClosePolicy() const;
        void setClosePolicy(::UMS_Data::SessionCloseType _closePolicy);

        ::ecore::ELong getTimeout() const;
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
