// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/LocalAccount.hpp
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

#ifndef UMS_DATA_LOCALACCOUNT_HPP
#define UMS_DATA_LOCALACCOUNT_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(LocalAccount_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    class LocalAccount: public virtual ::ecore::EObject

    {
    public:
        LocalAccount();

        virtual ~LocalAccount();

        virtual void _initialize();

        // Operations


        // Attributes
        ::ecore::EString const& getUserId() const;
        void setUserId(::ecore::EString const& _userId);

        ::ecore::EString const& getMachineId() const;
        void setMachineId(::ecore::EString const& _machineId);

        ::ecore::EString const& getAcLogin() const;
        void setAcLogin(::ecore::EString const& _acLogin);

        ::ecore::EString const& getSshKeyPath() const;
        void setSshKeyPath(::ecore::EString const& _sshKeyPath);

        ::ecore::EString const& getHomeDirectory() const;
        void setHomeDirectory(::ecore::EString const& _homeDirectory);

        // References


        /*PROTECTED REGION ID(LocalAccount) START*/
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

        /*PROTECTED REGION ID(LocalAccountImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_userId;

        ::ecore::EString m_machineId;

        ::ecore::EString m_acLogin;

        ::ecore::EString m_sshKeyPath;

        ::ecore::EString m_homeDirectory;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_LOCALACCOUNT_HPP
