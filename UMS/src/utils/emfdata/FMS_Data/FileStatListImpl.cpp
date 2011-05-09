// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/FileStatListImpl.cpp
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

#include "FileStatList.hpp"
#include <FMS_Data/FMS_DataPackage.hpp>
#include <FMS_Data/FileStat.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::FMS_Data;

/*PROTECTED REGION ID(FileStatListImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void FileStatList::_initialize()
{
    // Supertypes

    // Rerefences
    for (size_t i = 0; i < m_fileStats->size(); i++)
    {
        (*m_fileStats)[i]->_initialize();
        (*m_fileStats)[i]->_setEContainer(
                this,
                ::FMS_Data::FMS_DataPackage::_instance()->getFileStatList__fileStats());
    }

    /*PROTECTED REGION ID(FileStatListImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject FileStatList::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::FILESTATLIST__FILESTATS:
    {
        _any = m_fileStats->asEListOf< ::ecore::EObject > ();
    }
        return _any;

    }
    throw "Error";
}

void FileStatList::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::FILESTATLIST__FILESTATS:
    {
        ::ecorecpp::mapping::EList_ptr _t0 =
                ::ecorecpp::mapping::any::any_cast<
                        ::ecorecpp::mapping::EList_ptr >(_newValue);
        ::FMS_Data::FileStatList::getFileStats().clear();
        ::FMS_Data::FileStatList::getFileStats().insert_all(*_t0);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean FileStatList::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::FILESTATLIST__FILESTATS:
        return m_fileStats && m_fileStats->size();

    }
    throw "Error";
}

void FileStatList::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr FileStatList::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::FMS_Data::FMS_DataPackage_ptr > (::FMS_Data::FMS_DataPackage::_instance())->getFileStatList();
    return _eclass;
}

