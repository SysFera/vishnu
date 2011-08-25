// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/ExportOpImpl.cpp
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

#include "ExportOp.hpp"
#include <IMS_Data/IMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::IMS_Data;

/*PROTECTED REGION ID(ExportOpImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void ExportOp::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(ExportOpImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject ExportOp::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::EXPORTOP__EXPORTTYPE:
    {
        ::ecorecpp::mapping::any_traits< ::IMS_Data::ExportType >::toAny(_any,
                m_exportType);
    }
        return _any;

    }
    throw "Error";
}

void ExportOp::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::EXPORTOP__EXPORTTYPE:
    {
        ::ecorecpp::mapping::any_traits< ::IMS_Data::ExportType >::fromAny(
                _newValue, m_exportType);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean ExportOp::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::EXPORTOP__EXPORTTYPE:
        return m_exportType != 0;

    }
    throw "Error";
}

void ExportOp::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr ExportOp::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::IMS_Data::IMS_DataPackage_ptr > (::IMS_Data::IMS_DataPackage::_instance())->getExportOp();
    return _eclass;
}

