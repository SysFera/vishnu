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

/**
 * \file Machine.hpp
 * \brief The Machine class
 * \author Generated file
 * \date 20/02/2011
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

    /**
     * \class Machine
     * \brief Implementation of the Machine class
     */
    class Machine: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Machine
         */
        Machine();
        /**
         * \brief The destructor for Machine
         */
        virtual ~Machine();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
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
         * \brief To get the site
         * \return The site attribute value
         **/
        ::ecore::EString const& getSite() const;
        /**
         * \brief To set the site
         * \param _site The site value
         **/
        void setSite(::ecore::EString const& _site);

        /**
         * \brief To get the machineDescription
         * \return The machineDescription attribute value
         **/
        ::ecore::EString const& getMachineDescription() const;
        /**
         * \brief To set the machineDescription
         * \param _machineDescription The machineDescription value
         **/
        void setMachineDescription(::ecore::EString const& _machineDescription);

        /**
         * \brief To get the language
         * \return The language attribute value
         **/
        ::ecore::EString const& getLanguage() const;
        /**
         * \brief To set the language
         * \param _language The language value
         **/
        void setLanguage(::ecore::EString const& _language);

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
         * \brief To get the sshPublicKey
         * \return The sshPublicKey attribute value
         **/
        ::ecore::EString const& getSshPublicKey() const;
        /**
         * \brief To set the sshPublicKey
         * \param _sshPublicKey The sshPublicKey value
         **/
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
