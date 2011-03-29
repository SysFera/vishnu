// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListProgressionImpl.cpp
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

#include "ListProgression.hpp"
#include <TMS_Data/TMS_DataPackage.hpp>
#include <TMS_Data/Progression.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::TMS_Data;

/*PROTECTED REGION ID(ListProgressionImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void ListProgression::_initialize()
{
    // Supertypes

    // Rerefences
    for (size_t i = 0; i < m_progress->size(); i++)
    {
        (*m_progress)[i]->_initialize();
        (*m_progress)[i]->_setEContainer(
                this,
                ::TMS_Data::TMS_DataPackage::_instance()->getListProgression__progress());
    }

    /*PROTECTED REGION ID(ListProgressionImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject ListProgression::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::LISTPROGRESSION__NBJOBS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any, m_nbJobs);
    }
        return _any;
    case ::TMS_Data::TMS_DataPackage::LISTPROGRESSION__PROGRESS:
    {
        _any = m_progress->asEListOf< ::ecore::EObject > ();
    }
        return _any;

    }
    throw "Error";
}

void ListProgression::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::LISTPROGRESSION__NBJOBS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_nbJobs);
    }
        return;
    case ::TMS_Data::TMS_DataPackage::LISTPROGRESSION__PROGRESS:
    {
        ::ecorecpp::mapping::EList_ptr _t0 =
                ::ecorecpp::mapping::any::any_cast<
                        ::ecorecpp::mapping::EList_ptr >(_newValue);
        ::TMS_Data::ListProgression::getProgress().clear();
        ::TMS_Data::ListProgression::getProgress().insert_all(*_t0);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean ListProgression::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::LISTPROGRESSION__NBJOBS:
        return ::ecorecpp::mapping::set_traits< ::ecore::EInt >::is_set(
                m_nbJobs);
    case ::TMS_Data::TMS_DataPackage::LISTPROGRESSION__PROGRESS:
        return m_progress && m_progress->size();

    }
    throw "Error";
}

void ListProgression::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr ListProgression::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (::TMS_Data::TMS_DataPackage::_instance())->getListProgression();
    return _eclass;
}

