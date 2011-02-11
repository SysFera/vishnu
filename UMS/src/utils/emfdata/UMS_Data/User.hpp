// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/User.hpp
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

#ifndef UMS_DATA_USER_HPP
#define UMS_DATA_USER_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(User_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    class User: public virtual ::ecore::EObject

    {
    public:
        User();

        virtual ~User();

        virtual void _initialize();

        // Operations


        // Attributes
        ::ecore::EString const& getUserId() const;
        void setUserId(::ecore::EString const& _userId);

        ::ecore::EString const& getPassword() const;
        void setPassword(::ecore::EString const& _password);

        ::ecore::EString const& getFirstname() const;
        void setFirstname(::ecore::EString const& _firstname);

        ::ecore::EString const& getLastname() const;
        void setLastname(::ecore::EString const& _lastname);

        ::UMS_Data::PrivilegeType getPrivilege() const;
        void setPrivilege(::UMS_Data::PrivilegeType _privilege);

        ::ecore::EString const& getEmail() const;
        void setEmail(::ecore::EString const& _email);

        ::UMS_Data::StatusType getStatus() const;
        void setStatus(::UMS_Data::StatusType _status);

        // References


        /*PROTECTED REGION ID(User) START*/
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

        /*PROTECTED REGION ID(UserImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_userId;

        ::ecore::EString m_password;

        ::ecore::EString m_firstname;

        ::ecore::EString m_lastname;

        ::UMS_Data::PrivilegeType m_privilege;

        ::ecore::EString m_email;

        ::UMS_Data::StatusType m_status;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_USER_HPP
