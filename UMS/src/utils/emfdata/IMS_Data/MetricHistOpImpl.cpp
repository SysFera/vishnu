// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/MetricHistOpImpl.cpp
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

#include "MetricHistOp.hpp"
#include <IMS_Data/IMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::IMS_Data;

/*PROTECTED REGION ID(MetricHistOpImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void MetricHistOp::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(MetricHistOpImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject MetricHistOp::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::METRICHISTOP__STARTTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_startTime);
    }
        return _any;
    case ::IMS_Data::IMS_DataPackage::METRICHISTOP__ENDTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_endTime);
    }
        return _any;
    case ::IMS_Data::IMS_DataPackage::METRICHISTOP__TYPE:
    {
        ::ecorecpp::mapping::any_traits< ::IMS_Data::MetricType >::toAny(_any,
                m_type);
    }
        return _any;

    }
    throw "Error";
}

void MetricHistOp::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::METRICHISTOP__STARTTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_startTime);
    }
        return;
    case ::IMS_Data::IMS_DataPackage::METRICHISTOP__ENDTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_endTime);
    }
        return;
    case ::IMS_Data::IMS_DataPackage::METRICHISTOP__TYPE:
    {
        ::ecorecpp::mapping::any_traits< ::IMS_Data::MetricType >::fromAny(
                _newValue, m_type);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean MetricHistOp::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::METRICHISTOP__STARTTIME:
        return m_startTime != 0;
    case ::IMS_Data::IMS_DataPackage::METRICHISTOP__ENDTIME:
        return m_endTime != 0;
    case ::IMS_Data::IMS_DataPackage::METRICHISTOP__TYPE:
        return m_type != 0;

    }
    throw "Error";
}

void MetricHistOp::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr MetricHistOp::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::IMS_Data::IMS_DataPackage_ptr > (::IMS_Data::IMS_DataPackage::_instance())->getMetricHistOp();
    return _eclass;
}

