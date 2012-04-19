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
#include <TMS_Data/LoadCriterion.hpp>
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
    if (m_criterion)
    {
        m_criterion->_initialize();
        m_criterion->_setEContainer(
                this,
                ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__criterion());
    }

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
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
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
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__MAILNOTIFICATION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_mailNotification);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__MAILNOTIFYUSER:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_mailNotifyUser);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__GROUP:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_group);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__WORKINGDIR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_workingDir);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__CPUTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_cpuTime);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__SELECTQUEUEAUTOM:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::toAny(_any,
                m_selectQueueAutom);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__CRITERION:
    {
        _any = static_cast< ::ecore::EObject* > (m_criterion);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__FILE:
    {
        std::vector < ::ecorecpp::mapping::any > _anys(m_file.size());
        for (size_t _i = 0; _i < m_file.size(); _i++)
            ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(
                    _anys[_i], m_file[_i]);
        _any = _anys;
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__LISTFILES:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_listFiles);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__PARAM:
    {
        std::vector < ::ecorecpp::mapping::any > _anys(m_param.size());
        for (size_t _i = 0; _i < m_param.size(); _i++)
            ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(
                    _anys[_i], m_param[_i]);
        _any = _anys;
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__LISTPARAMS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_listParams);
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
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
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
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__MAILNOTIFICATION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_mailNotification);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__MAILNOTIFYUSER:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_mailNotifyUser);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__GROUP:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_group);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__WORKINGDIR:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_workingDir);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__CPUTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_cpuTime);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__SELECTQUEUEAUTOM:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::fromAny(
                _newValue, m_selectQueueAutom);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__CRITERION:
    {
        ::ecore::EObject_ptr _t0 = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EObject_ptr >(_newValue);
        ::TMS_Data::LoadCriterion_ptr _t1 =
                dynamic_cast< ::TMS_Data::LoadCriterion_ptr > (_t0);
        ::TMS_Data::SubmitOptions::setCriterion(_t1);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__FILE:
    {
        ::ecore::EString _t0;
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                _t0);
        ::TMS_Data::SubmitOptions::addFile(_t0);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__LISTFILES:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_listFiles);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__PARAM:
    {
        ::ecore::EString _t0;
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                _t0);
        ::TMS_Data::SubmitOptions::addParam(_t0);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__LISTPARAMS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_listParams);
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
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_nbNodesAndCpuPerNode);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__OUTPUTPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_outputPath);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__ERRORPATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_errorPath);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__MAILNOTIFICATION:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_mailNotification);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__MAILNOTIFYUSER:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_mailNotifyUser);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__GROUP:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_group);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__WORKINGDIR:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_workingDir);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__CPUTIME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_cpuTime);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__SELECTQUEUEAUTOM:
        return m_selectQueueAutom != false;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__CRITERION:
        return m_criterion;
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__FILE:
        return m_file.size();
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__LISTFILES:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_listFiles);
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__PARAM:
        return m_param.size();
    case ::TMS_Data::TMS_DataPackage::SUBMITOPTIONS__LISTPARAMS:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_listParams);

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

