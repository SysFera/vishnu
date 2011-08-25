// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListCommandsImpl.cpp
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

#include "ListCommands.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <UMS_Data/Command.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(ListCommandsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void ListCommands::_initialize()
{
    // Supertypes

    // Rerefences
    for (size_t i = 0; i < m_Commands->size(); i++)
    {
        (*m_Commands)[i]->_initialize();
        (*m_Commands)[i]->_setEContainer(
                this,
                ::UMS_Data::UMS_DataPackage::_instance()->getListCommands__Commands());
    }

    /*PROTECTED REGION ID(ListCommandsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject ListCommands::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTCOMMANDS__COMMANDS:
    {
        _any = m_Commands->asEListOf< ::ecore::EObject > ();
    }
        return _any;

    }
    throw "Error";
}

void ListCommands::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTCOMMANDS__COMMANDS:
    {
        ::ecorecpp::mapping::EList_ptr _t0 =
                ::ecorecpp::mapping::any::any_cast<
                        ::ecorecpp::mapping::EList_ptr >(_newValue);
        ::UMS_Data::ListCommands::getCommands().clear();
        ::UMS_Data::ListCommands::getCommands().insert_all(*_t0);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean ListCommands::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTCOMMANDS__COMMANDS:
        return m_Commands && m_Commands->size();

    }
    throw "Error";
}

void ListCommands::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr ListCommands::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getListCommands();
    return _eclass;
}

