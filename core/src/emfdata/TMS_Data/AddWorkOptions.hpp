// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/AddWorkOptions.hpp
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
 * \file AddWorkOptions.hpp
 * \brief The AddWorkOptions class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef TMS_DATA_ADDWORKOPTIONS_HPP
#define TMS_DATA_ADDWORKOPTIONS_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(AddWorkOptions_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    /**
     * \class AddWorkOptions
     * \brief Implementation of the AddWorkOptions class
     */
    class AddWorkOptions: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for AddWorkOptions
         */
        AddWorkOptions();
        /**
         * \brief The destructor for AddWorkOptions
         */
        virtual ~AddWorkOptions();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the applicationId
         * \return The applicationId attribute value
         **/
        ::ecore::EString const& getApplicationId() const;
        /**
         * \brief To set the applicationId
         * \param _applicationId The applicationId value
         **/
        void setApplicationId(::ecore::EString const& _applicationId);

        /**
         * \brief To get the subject
         * \return The subject attribute value
         **/
        ::ecore::EString const& getSubject() const;
        /**
         * \brief To set the subject
         * \param _subject The subject value
         **/
        void setSubject(::ecore::EString const& _subject);

        /**
         * \brief To get the priority
         * \return The priority attribute value
         **/
        ::ecore::EInt getPriority() const;
        /**
         * \brief To set the priority
         * \param _priority The priority value
         **/
        void setPriority(::ecore::EInt _priority);

        /**
         * \brief To get the owner
         * \return The owner attribute value
         **/
        ::ecore::EString const& getOwner() const;
        /**
         * \brief To set the owner
         * \param _owner The owner value
         **/
        void setOwner(::ecore::EString const& _owner);

        /**
         * \brief To get the estimatedHour
         * \return The estimatedHour attribute value
         **/
        ::ecore::ELong getEstimatedHour() const;
        /**
         * \brief To set the estimatedHour
         * \param _estimatedHour The estimatedHour value
         **/
        void setEstimatedHour(::ecore::ELong _estimatedHour);

        /**
         * \brief To get the description
         * \return The description attribute value
         **/
        ::ecore::EString const& getDescription() const;
        /**
         * \brief To set the description
         * \param _description The description value
         **/
        void setDescription(::ecore::EString const& _description);

        /**
         * \brief To get the projectId
         * \return The projectId attribute value
         **/
        ::ecore::EString const& getProjectId() const;
        /**
         * \brief To set the projectId
         * \param _projectId The projectId value
         **/
        void setProjectId(::ecore::EString const& _projectId);

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
         * \brief To get the nbCPU
         * \return The nbCPU attribute value
         **/
        ::ecore::EInt getNbCPU() const;
        /**
         * \brief To set the nbCPU
         * \param _nbCPU The nbCPU value
         **/
        void setNbCPU(::ecore::EInt _nbCPU);

        // References


        /*PROTECTED REGION ID(AddWorkOptions) START*/
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

        /*PROTECTED REGION ID(AddWorkOptionsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_applicationId;

        ::ecore::EString m_subject;

        ::ecore::EInt m_priority;

        ::ecore::EString m_owner;

        ::ecore::ELong m_estimatedHour;

        ::ecore::EString m_description;

        ::ecore::EString m_projectId;

        ::ecore::EString m_machineId;

        ::ecore::EInt m_nbCPU;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_ADDWORKOPTIONS_HPP
