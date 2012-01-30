// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/AuthAccount.hpp
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
 * \file AuthAccount.hpp
 * \brief The AuthAccount class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef UMS_DATA_AUTHACCOUNT_HPP
#define UMS_DATA_AUTHACCOUNT_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(AuthAccount_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class AuthAccount
     * \brief Implementation of the AuthAccount class
     */
    class AuthAccount: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for AuthAccount
         */
        AuthAccount();
        /**
         * \brief The destructor for AuthAccount
         */
        virtual ~AuthAccount();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the authSystemId
         * \return The authSystemId attribute value
         **/
        ::ecore::EString const& getAuthSystemId() const;
        /**
         * \brief To set the authSystemId
         * \param _authSystemId The authSystemId value
         **/
        void setAuthSystemId(::ecore::EString const& _authSystemId);

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
         * \brief To get the acLogin
         * \return The acLogin attribute value
         **/
        ::ecore::EString const& getAcLogin() const;
        /**
         * \brief To set the acLogin
         * \param _acLogin The acLogin value
         **/
        void setAcLogin(::ecore::EString const& _acLogin);

        // References


        /*PROTECTED REGION ID(AuthAccount) START*/
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

        /*PROTECTED REGION ID(AuthAccountImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_authSystemId;

        ::ecore::EString m_userId;

        ::ecore::EString m_acLogin;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_AUTHACCOUNT_HPP
