// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/ListThresholdImpl.cpp
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

#include "ListThreshold.hpp"
#include <IMS_Data/IMS_DataPackage.hpp>
#include <IMS_Data/Threshold.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::IMS_Data;

/*PROTECTED REGION ID(ListThresholdImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void ListThreshold::_initialize()
{
    // Supertypes

    // Rerefences
    for (size_t i = 0; i < m_Threshold->size(); i++)
    {
        (*m_Threshold)[i]->_initialize();
        (*m_Threshold)[i]->_setEContainer(
                this,
                ::IMS_Data::IMS_DataPackage::_instance()->getListThreshold__Threshold());
    }

    /*PROTECTED REGION ID(ListThresholdImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject ListThreshold::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::LISTTHRESHOLD__THRESHOLD:
    {
        _any = m_Threshold->asEListOf< ::ecore::EObject > ();
    }
        return _any;

    }
    throw "Error";
}

void ListThreshold::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::LISTTHRESHOLD__THRESHOLD:
    {
        ::ecorecpp::mapping::EList_ptr _t0 =
                ::ecorecpp::mapping::any::any_cast<
                        ::ecorecpp::mapping::EList_ptr >(_newValue);
        ::IMS_Data::ListThreshold::getThreshold().clear();
        ::IMS_Data::ListThreshold::getThreshold().insert_all(*_t0);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean ListThreshold::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::IMS_Data::IMS_DataPackage::LISTTHRESHOLD__THRESHOLD:
        return m_Threshold && m_Threshold->size();

    }
    throw "Error";
}

void ListThreshold::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr ListThreshold::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::IMS_Data::IMS_DataPackage_ptr > (::IMS_Data::IMS_DataPackage::_instance())->getListThreshold();
    return _eclass;
}

