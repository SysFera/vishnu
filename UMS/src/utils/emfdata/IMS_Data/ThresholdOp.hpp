// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/ThresholdOp.hpp
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
 * \file ThresholdOp.hpp
 * \brief The ThresholdOp class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef IMS_DATA_THRESHOLDOP_HPP
#define IMS_DATA_THRESHOLDOP_HPP

#include <IMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ThresholdOp_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace IMS_Data
{

    /**
     * \class ThresholdOp
     * \brief Implementation of the ThresholdOp class
     */
    class ThresholdOp: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ThresholdOp
         */
        ThresholdOp();
        /**
         * \brief The destructor for ThresholdOp
         */
        virtual ~ThresholdOp();

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
         * \brief To get the metricType
         * \return The metricType attribute value
         **/
        ::IMS_Data::MetricType getMetricType() const;
        /**
         * \brief To set the metricType
         * \param _metricType The metricType value
         **/
        void setMetricType(::IMS_Data::MetricType _metricType);

        // References


        /*PROTECTED REGION ID(ThresholdOp) START*/
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

        /*PROTECTED REGION ID(ThresholdOpImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_machineId;

        ::IMS_Data::MetricType m_metricType;

        // References

    };

} // IMS_Data

#endif // IMS_DATA_THRESHOLDOP_HPP
