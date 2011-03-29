// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListJobResultsImpl.cpp
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

#include "ListJobResults.hpp"
#include <TMS_Data/TMS_DataPackage.hpp>
#include <TMS_Data/JobResult.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::TMS_Data;

/*PROTECTED REGION ID(ListJobResultsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void ListJobResults::_initialize()
{
    // Supertypes

    // Rerefences
    for (size_t i = 0; i < m_Results->size(); i++)
    {
        (*m_Results)[i]->_initialize();
        (*m_Results)[i]->_setEContainer(
                this,
                ::TMS_Data::TMS_DataPackage::_instance()->getListJobResults__Results());
    }

    /*PROTECTED REGION ID(ListJobResultsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject ListJobResults::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::LISTJOBRESULTS__NBJOBS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_nbJobs);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::LISTJOBRESULTS__RESULTS:
    {
        _any = m_Results->asEListOf< ::ecore::EObject > ();
    }
        return _any;

    }
    throw "Error";
}

void ListJobResults::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::LISTJOBRESULTS__NBJOBS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_nbJobs);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::LISTJOBRESULTS__RESULTS:
    {
        ::ecorecpp::mapping::EList_ptr _t0 =
                ::ecorecpp::mapping::any::any_cast<
                        ::ecorecpp::mapping::EList_ptr >(_newValue);
        ::TMS_Data::ListJobResults::getResults().clear();
        ::TMS_Data::ListJobResults::getResults().insert_all(*_t0);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean ListJobResults::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::LISTJOBRESULTS__NBJOBS:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_nbJobs);
    case ::TMS_Data::TMS_DataPackage::LISTJOBRESULTS__RESULTS:
        return m_Results && m_Results->size();

    }
    throw "Error";
}

void ListJobResults::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr ListJobResults::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (::TMS_Data::TMS_DataPackage::_instance())->getListJobResults();
    return _eclass;
}

