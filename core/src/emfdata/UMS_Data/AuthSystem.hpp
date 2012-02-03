// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/AuthSystem.hpp
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
 * \file AuthSystem.hpp
 * \brief The AuthSystem class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef UMS_DATA_AUTHSYSTEM_HPP
#define UMS_DATA_AUTHSYSTEM_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(AuthSystem_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class AuthSystem
     * \brief Implementation of the AuthSystem class
     */
    class AuthSystem: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for AuthSystem
         */
        AuthSystem();
        /**
         * \brief The destructor for AuthSystem
         */
        virtual ~AuthSystem();

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
         * \brief To get the name
         * \return The name attribute value
         **/
        ::ecore::EString const& getName() const;
        /**
         * \brief To set the name
         * \param _name The name value
         **/
        void setName(::ecore::EString const& _name);

        /**
         * \brief To get the URI
         * \return The URI attribute value
         **/
        ::ecore::EString const& getURI() const;
        /**
         * \brief To set the URI
         * \param _URI The URI value
         **/
        void setURI(::ecore::EString const& _URI);

        /**
         * \brief To get the authLogin
         * \return The authLogin attribute value
         **/
        ::ecore::EString const& getAuthLogin() const;
        /**
         * \brief To set the authLogin
         * \param _authLogin The authLogin value
         **/
        void setAuthLogin(::ecore::EString const& _authLogin);

        /**
         * \brief To get the authPassword
         * \return The authPassword attribute value
         **/
        ::ecore::EString const& getAuthPassword() const;
        /**
         * \brief To set the authPassword
         * \param _authPassword The authPassword value
         **/
        void setAuthPassword(::ecore::EString const& _authPassword);

        /**
         * \brief To get the userPasswordEncryption
         * \return The userPasswordEncryption attribute value
         **/
        ::UMS_Data::EncryptionMethod getUserPasswordEncryption() const;
        /**
         * \brief To set the userPasswordEncryption
         * \param _userPasswordEncryption The userPasswordEncryption value
         **/
        void setUserPasswordEncryption(
                ::UMS_Data::EncryptionMethod _userPasswordEncryption);

        /**
         * \brief To get the type
         * \return The type attribute value
         **/
        ::UMS_Data::AuthType getType() const;
        /**
         * \brief To set the type
         * \param _type The type value
         **/
        void setType(::UMS_Data::AuthType _type);

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

        // References
        /**
         * \brief To get the options
         * \return A reference to options
         **/
        ::UMS_Data::AuthSystemOptions_ptr getOptions();
        /**
         * \brief To set the options reference
         * \param _options A reference to a options
         **/
        void setOptions(::UMS_Data::AuthSystemOptions_ptr _options);

        /*PROTECTED REGION ID(AuthSystem) START*/
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

        /*PROTECTED REGION ID(AuthSystemImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_authSystemId;

        ::ecore::EString m_name;

        ::ecore::EString m_URI;

        ::ecore::EString m_authLogin;

        ::ecore::EString m_authPassword;

        ::UMS_Data::EncryptionMethod m_userPasswordEncryption;

        ::UMS_Data::AuthType m_type;

        ::UMS_Data::StatusType m_status;

        // References

        ::UMS_Data::AuthSystemOptions_ptr m_options;

    };

} // UMS_Data

#endif // UMS_DATA_AUTHSYSTEM_HPP
