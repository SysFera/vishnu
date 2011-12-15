// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/DirEntryListImpl.cpp
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

#include "DirEntryList.hpp"
#include <FMS_Data/FMS_DataPackage.hpp>
#include <FMS_Data/DirEntry.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::FMS_Data;

/*PROTECTED REGION ID(DirEntryListImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void DirEntryList::_initialize()
{
    // Supertypes

    // Rerefences
    for (size_t i = 0; i < m_dirEntries->size(); i++)
    {
        (*m_dirEntries)[i]->_initialize();
        (*m_dirEntries)[i]->_setEContainer(
                this,
                ::FMS_Data::FMS_DataPackage::_instance()->getDirEntryList__dirEntries());
    }

    /*PROTECTED REGION ID(DirEntryListImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject DirEntryList::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::DIRENTRYLIST__DIRENTRIES:
    {
        _any = m_dirEntries->asEListOf< ::ecore::EObject > ();
    }
        return _any;

    }
    throw "Error";
}

void DirEntryList::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::DIRENTRYLIST__DIRENTRIES:
    {
        ::ecorecpp::mapping::EList_ptr _t0 =
                ::ecorecpp::mapping::any::any_cast<
                        ::ecorecpp::mapping::EList_ptr >(_newValue);
        ::FMS_Data::DirEntryList::getDirEntries().clear();
        ::FMS_Data::DirEntryList::getDirEntries().insert_all(*_t0);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean DirEntryList::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::DIRENTRYLIST__DIRENTRIES:
        return m_dirEntries && m_dirEntries->size();

    }
    throw "Error";
}

void DirEntryList::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr DirEntryList::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::FMS_Data::FMS_DataPackage_ptr > (::FMS_Data::FMS_DataPackage::_instance())->getDirEntryList();
    return _eclass;
}

