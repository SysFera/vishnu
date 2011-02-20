// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/Machine.hpp
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

#ifndef UMS_DATA_MACHINE_HPP
#define UMS_DATA_MACHINE_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Machine_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    class Machine: public virtual ::ecore::EObject

    {
    public:
        Machine();

        virtual ~Machine();

        virtual void _initialize();

        // Operations


        // Attributes
        ::ecore::EString const& getMachineId() const;
        void setMachineId(::ecore::EString const& _machineId);

        ::ecore::EString const& getName() const;
        void setName(::ecore::EString const& _name);

        ::ecore::EString const& getSite() const;
        void setSite(::ecore::EString const& _site);

        ::ecore::EString const& getMachineDescription() const;
        void setMachineDescription(::ecore::EString const& _machineDescription);

        ::ecore::EString const& getLanguage() const;
        void setLanguage(::ecore::EString const& _language);

        ::UMS_Data::StatusType getStatus() const;
        void setStatus(::UMS_Data::StatusType _status);

        ::ecore::EString const& getSshPublicKey() const;
        void setSshPublicKey(::ecore::EString const& _sshPublicKey);

        // References


        /*PROTECTED REGION ID(Machine) START*/
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

        /*PROTECTED REGION ID(MachineImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_machineId;

        ::ecore::EString m_name;

        ::ecore::EString m_site;

        ::ecore::EString m_machineDescription;

        ::ecore::EString m_language;

        ::UMS_Data::StatusType m_status;

        ::ecore::EString m_sshPublicKey;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_MACHINE_HPP
