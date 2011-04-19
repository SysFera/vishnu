// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/SubmitOptionsImpl.cpp
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

#include "SubmitOptions.hpp"
#include <TMS_Data/TMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::TMS_Data;

/*PROTECTED REGION ID(SubmitOptionsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void SubmitOptions::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(SubmitOptionsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject SubmitOptions::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__NAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_name);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__QUEUE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_queue);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__WALLTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any,
                m_wallTime);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__MEMORY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any, m_memory);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__NBCPU:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any, m_nbCpu);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__NBNODESANDCPUPERNODE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any,
                m_nbNodesAndCpuPerNode);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__OUTPUTPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_outputPath);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__ERRORPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_errorPath);
    }
        return _any;

    }
    throw "Error";
}

void SubmitOptions::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__NAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_name);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__QUEUE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_queue);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__WALLTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_wallTime);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__MEMORY:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_memory);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__NBCPU:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_nbCpu);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__NBNODESANDCPUPERNODE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_nbNodesAndCpuPerNode);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__OUTPUTPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_outputPath);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__ERRORPATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_errorPath);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean SubmitOptions::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__NAME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_name);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__QUEUE:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_queue);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__WALLTIME:
        return m_wallTime != -1;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__MEMORY:
        return m_memory != -1;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__NBCPU:
        return m_nbCpu != -1;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__NBNODESANDCPUPERNODE:
        return m_nbNodesAndCpuPerNode != -1;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__OUTPUTPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_outputPath);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__ERRORPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_errorPath);

    }
    throw "Error";
}

void SubmitOptions::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr SubmitOptions::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (::TMS_Data::TMS_DataPackage::_instance())->getSubmitOptions();
    return _eclass;
}

