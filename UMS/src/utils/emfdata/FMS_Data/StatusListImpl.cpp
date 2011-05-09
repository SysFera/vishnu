// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/StatusListImpl.cpp
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

#include "StatusList.hpp"
#include <FMS_Data/FMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::FMS_Data;

/*PROTECTED REGION ID(StatusListImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void StatusList::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(StatusListImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject StatusList::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::STATUSLIST__STATUS:
    {
        std::vector < ::ecorecpp::mapping::any > _anys(m_status.size());
        for (size_t _i = 0; _i < m_status.size(); _i++)
            ::ecorecpp::mapping::any_traits< ::FMS_Data::Status >::toAny(
                    _anys[_i], m_status[_i]);
        _any = _anys;
    }
        return _any;

    }
    throw "Error";
}

void StatusList::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::STATUSLIST__STATUS:
    {
        ::FMS_Data::Status _t0;
        ::ecorecpp::mapping::any_traits< ::FMS_Data::Status >::fromAny(
                _newValue, _t0);
        ::FMS_Data::StatusList::addStatus(_t0);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean StatusList::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::STATUSLIST__STATUS:
        return m_status.size();

    }
    throw "Error";
}

void StatusList::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr StatusList::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::FMS_Data::FMS_DataPackage_ptr > (::FMS_Data::FMS_DataPackage::_instance())->getStatusList();
    return _eclass;
}

