// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ConfigurationImpl.cpp
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

#include "Configuration.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <UMS_Data/User.hpp>
#include <UMS_Data/Machine.hpp>
#include <UMS_Data/LocalAccount.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(ConfigurationImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void Configuration::_initialize()
{
    // Supertypes

    // Rerefences
    for (size_t i = 0; i < m_listConfUsers->size(); i++)
    {
        (*m_listConfUsers)[i]->_initialize();
        (*m_listConfUsers)[i]->_setEContainer(
                this,
                ::UMS_Data::UMS_DataPackage::_instance()->getConfiguration__listConfUsers());
    }
    for (size_t i = 0; i < m_listConfMachines->size(); i++)
    {
        (*m_listConfMachines)[i]->_initialize();
        (*m_listConfMachines)[i]->_setEContainer(
                this,
                ::UMS_Data::UMS_DataPackage::_instance()->getConfiguration__listConfMachines());
    }
    for (size_t i = 0; i < m_listConfLocalAccounts->size(); i++)
    {
        (*m_listConfLocalAccounts)[i]->_initialize();
        (*m_listConfLocalAccounts)[i]->_setEContainer(
                this,
                ::UMS_Data::UMS_DataPackage::_instance()->getConfiguration__listConfLocalAccounts());
    }

    /*PROTECTED REGION ID(ConfigurationImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject Configuration::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::CONFIGURATION__LISTCONFUSERS:
    {
        _any = m_listConfUsers->asEListOf< ::ecore::EObject > ();
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::CONFIGURATION__LISTCONFMACHINES:
    {
        _any = m_listConfMachines->asEListOf< ::ecore::EObject > ();
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::CONFIGURATION__LISTCONFLOCALACCOUNTS:
    {
        _any = m_listConfLocalAccounts->asEListOf< ::ecore::EObject > ();
    }
        return _any;

    }
    throw "Error";
}

void Configuration::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::CONFIGURATION__LISTCONFUSERS:
    {
        ::ecorecpp::mapping::EList_ptr _t0 =
                ::ecorecpp::mapping::any::any_cast<
                        ::ecorecpp::mapping::EList_ptr >(_newValue);
        ::UMS_Data::Configuration::getListConfUsers().clear();
        ::UMS_Data::Configuration::getListConfUsers().insert_all(*_t0);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::CONFIGURATION__LISTCONFMACHINES:
    {
        ::ecorecpp::mapping::EList_ptr _t0 =
                ::ecorecpp::mapping::any::any_cast<
                        ::ecorecpp::mapping::EList_ptr >(_newValue);
        ::UMS_Data::Configuration::getListConfMachines().clear();
        ::UMS_Data::Configuration::getListConfMachines().insert_all(*_t0);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::CONFIGURATION__LISTCONFLOCALACCOUNTS:
    {
        ::ecorecpp::mapping::EList_ptr _t0 =
                ::ecorecpp::mapping::any::any_cast<
                        ::ecorecpp::mapping::EList_ptr >(_newValue);
        ::UMS_Data::Configuration::getListConfLocalAccounts().clear();
        ::UMS_Data::Configuration::getListConfLocalAccounts().insert_all(*_t0);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean Configuration::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::CONFIGURATION__LISTCONFUSERS:
        return m_listConfUsers && m_listConfUsers->size();
    case ::UMS_Data::UMS_DataPackage::CONFIGURATION__LISTCONFMACHINES:
        return m_listConfMachines && m_listConfMachines->size();
    case ::UMS_Data::UMS_DataPackage::CONFIGURATION__LISTCONFLOCALACCOUNTS:
        return m_listConfLocalAccounts && m_listConfLocalAccounts->size();

    }
    throw "Error";
}

void Configuration::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr Configuration::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getConfiguration();
    return _eclass;
}

