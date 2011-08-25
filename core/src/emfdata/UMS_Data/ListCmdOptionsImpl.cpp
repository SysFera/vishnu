// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListCmdOptionsImpl.cpp
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

#include "ListCmdOptions.hpp"
#include <UMS_Data/UMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::UMS_Data;

/*PROTECTED REGION ID(ListCmdOptionsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void ListCmdOptions::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(ListCmdOptionsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject ListCmdOptions::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__ADMINLISTOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::toAny(_any,
                m_adminListOption);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_userId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__SESSIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::toAny(_any,
                m_sessionId);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__STARTDATEOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_startDateOption);
    }
        return _any;
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__ENDDATEOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::toAny(_any,
                m_endDateOption);
    }
        return _any;

    }
    throw "Error";
}

void ListCmdOptions::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__ADMINLISTOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::fromAny(
                _newValue, m_adminListOption);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__USERID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_userId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__SESSIONID:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EString >::fromAny(_newValue,
                m_sessionId);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__STARTDATEOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_startDateOption);
    }
        return;
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__ENDDATEOPTION:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::ELong >::fromAny(_newValue,
                m_endDateOption);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean ListCmdOptions::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__ADMINLISTOPTION:
        return m_adminListOption != false;
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__USERID:
        return ::ecorecpp::mapping::set_traits< ::ecore::EString >::is_set(
                m_userId);
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__SESSIONID:
        return m_sessionId != "";
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__STARTDATEOPTION:
        return m_startDateOption != -1;
    case ::UMS_Data::UMS_DataPackage::LISTCMDOPTIONS__ENDDATEOPTION:
        return m_endDateOption != -1;

    }
    throw "Error";
}

void ListCmdOptions::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr ListCmdOptions::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::UMS_Data::UMS_DataPackage_ptr > (::UMS_Data::UMS_DataPackage::_instance())->getListCmdOptions();
    return _eclass;
}

