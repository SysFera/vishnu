// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/LsDirOptionsImpl.cpp
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

#include "LsDirOptions.hpp"
#include <FMS_Data/FMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::FMS_Data;

/*PROTECTED REGION ID(LsDirOptionsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void LsDirOptions::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(LsDirOptionsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject LsDirOptions::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::LSDIROPTIONS__LONGFORMAT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::toAny(_any,
                m_longFormat);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::LSDIROPTIONS__ALLFILES:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::toAny(_any,
                m_allFiles);
    }
        return _any;

    }
    throw "Error";
}

void LsDirOptions::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::LSDIROPTIONS__LONGFORMAT:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::fromAny(
                _newValue, m_longFormat);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::LSDIROPTIONS__ALLFILES:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::fromAny(
                _newValue, m_allFiles);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean LsDirOptions::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::LSDIROPTIONS__LONGFORMAT:
        return m_longFormat != false;
    case ::FMS_Data::FMS_DataPackage::LSDIROPTIONS__ALLFILES:
        return m_allFiles != false;

    }
    throw "Error";
}

void LsDirOptions::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr LsDirOptions::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::FMS_Data::FMS_DataPackage_ptr > (::FMS_Data::FMS_DataPackage::_instance())->getLsDirOptions();
    return _eclass;
}

