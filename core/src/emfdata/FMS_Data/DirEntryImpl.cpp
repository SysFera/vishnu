// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/DirEntryImpl.cpp
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

#include "DirEntry.hpp"
#include <FMS_Data/FMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::FMS_Data;

/*PROTECTED REGION ID(DirEntryImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void DirEntry::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(DirEntryImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject DirEntry::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__PATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any, m_path);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__OWNER:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_owner);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__GROUP:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_group);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__PERMS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::toAny(_any, m_perms);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__SIZE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any, m_size);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__CREATIONTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_creationTime);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__TYPE:
    {
        ::ecorecpp::mapping::any_traits< ::FMS_Data::FileType >::toAny(_any,
                m_type);
    }
        return _any;

    }
    throw "Error";
}

void DirEntry::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__PATH:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_path);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__OWNER:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_owner);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__GROUP:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_group);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__PERMS:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EInt >::fromAny(_newValue,
                m_perms);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__SIZE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_size);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__CREATIONTIME:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_creationTime);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__TYPE:
    {
        ::ecorecpp::mapping::any_traits< ::FMS_Data::FileType >::fromAny(
                _newValue, m_type);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean DirEntry::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__PATH:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_path);
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__OWNER:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_owner);
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__GROUP:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_group);
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__PERMS:
        return m_perms != -1;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__SIZE:
        return m_size != -1;
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__CREATIONTIME:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_creationTime);
    case ::FMS_Data::FMS_DataPackage::DIRENTRY__TYPE:
        return m_type != 6;

    }
    throw "Error";
}

void DirEntry::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr DirEntry::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::FMS_Data::FMS_DataPackage_ptr > (::FMS_Data::FMS_DataPackage::_instance())->getDirEntry();
    return _eclass;
}

