// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/LoadCriterionImpl.cpp
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

#include "LoadCriterion.hpp"
#include <TMS_Data/TMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::TMS_Data;

/*PROTECTED REGION ID(LoadCriterionImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void LoadCriterion::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(LoadCriterionImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject LoadCriterion::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::LOADCRITERION__LOADTYPE:
    {
        ::ecorecpp::mapping::any_traits< ::TMS_Data::LoadType >::toAny(_any,
                m_loadType);
    }
        return _any;

    }
    throw "Error";
}

void LoadCriterion::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::LOADCRITERION__LOADTYPE:
    {
        ::ecorecpp::mapping::any_traits< ::TMS_Data::LoadType >::fromAny(
                _newValue, m_loadType);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean LoadCriterion::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::TMS_Data::TMS_DataPackage::LOADCRITERION__LOADTYPE:
        return m_loadType != 1;

    }
    throw "Error";
}

void LoadCriterion::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr LoadCriterion::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (::TMS_Data::TMS_DataPackage::_instance())->getLoadCriterion();
    return _eclass;
}

