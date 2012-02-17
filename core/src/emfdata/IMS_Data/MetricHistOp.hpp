// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/MetricHistOp.hpp
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
 * \file MetricHistOp.hpp
 * \brief The MetricHistOp class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef IMS_DATA_METRICHISTOP_HPP
#define IMS_DATA_METRICHISTOP_HPP

#include <IMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
/*PROTECTED REGION ID(MetricHistOp_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace IMS_Data
{

    /**
     * \class MetricHistOp
     * \brief Implementation of the MetricHistOp class
     */
    class VISHNU_API_LIB MetricHistOp: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for MetricHistOp
         */
        MetricHistOp();
        /**
         * \brief The destructor for MetricHistOp
         */
        virtual ~MetricHistOp();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the startTime
         * \return The startTime attribute value
         **/
        ::ecore::ELong getStartTime() const;
        /**
         * \brief To set the startTime
         * \param _startTime The startTime value
         **/
        void setStartTime(::ecore::ELong _startTime);

        /**
         * \brief To get the endTime
         * \return The endTime attribute value
         **/
        ::ecore::ELong getEndTime() const;
        /**
         * \brief To set the endTime
         * \param _endTime The endTime value
         **/
        void setEndTime(::ecore::ELong _endTime);

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

        // References


        /*PROTECTED REGION ID(MetricHistOp) START*/
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

        /*PROTECTED REGION ID(MetricHistOpImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::ELong m_startTime;

        ::ecore::ELong m_endTime;

        ::IMS_Data::MetricType m_type;

        // References

    };

} // IMS_Data

#endif // IMS_DATA_METRICHISTOP_HPP
