// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/Threshold.hpp
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
 * \file Threshold.hpp
 * \brief The Threshold class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef IMS_DATA_THRESHOLD_HPP
#define IMS_DATA_THRESHOLD_HPP

#include <IMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
/*PROTECTED REGION ID(Threshold_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace IMS_Data
{

    /**
     * \class Threshold
     * \brief Implementation of the Threshold class
     */
    class VISHNU_API_LIB Threshold: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Threshold
         */
        Threshold();
        /**
         * \brief The destructor for Threshold
         */
        virtual ~Threshold();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the value
         * \return The value attribute value
         **/
        ::ecore::ELong getValue() const;
        /**
         * \brief To set the value
         * \param _value The value value
         **/
        void setValue(::ecore::ELong _value);

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
         * \brief To get the type
         * \return The type attribute value
         **/
        ::IMS_Data::MetricType getType() const;
        /**
         * \brief To set the type
         * \param _type The type value
         **/
        void setType(::IMS_Data::MetricType _type);

        /**
         * \brief To get the handler
         * \return The handler attribute value
         **/
        ::ecore::EString const& getHandler() const;
        /**
         * \brief To set the handler
         * \param _handler The handler value
         **/
        void setHandler(::ecore::EString const& _handler);

        // References


        /*PROTECTED REGION ID(Threshold) START*/
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

        /*PROTECTED REGION ID(ThresholdImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::ELong m_value;

        ::ecore::EString m_machineId;

        ::IMS_Data::MetricType m_type;

        ::ecore::EString m_handler;

        // References

    };

} // IMS_Data

#endif // IMS_DATA_THRESHOLD_HPP
