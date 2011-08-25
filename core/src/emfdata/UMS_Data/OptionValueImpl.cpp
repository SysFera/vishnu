// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/OptionValueImpl.cpp
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

#include "OptionValue.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(OptionValueImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void OptionValue::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(OptionValueImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject OptionValue::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::OPTIONVALUE__OPTIONNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_optionName);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::OPTIONVALUE__VALUE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_value);
    }
        return _any;

    }
    throw "Error";
}

void OptionValue::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::OPTIONVALUE__OPTIONNAME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_optionName);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::OPTIONVALUE__VALUE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_value);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean OptionValue::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::OPTIONVALUE__OPTIONNAME:
        return m_optionName != "";
    case ::UMS_Data::UMS_DataPackage::OPTIONVALUE__VALUE:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_value);

    }
    throw "Error";
}

void OptionValue::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr OptionValue::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getOptionValue();
    return _eclass;
}

