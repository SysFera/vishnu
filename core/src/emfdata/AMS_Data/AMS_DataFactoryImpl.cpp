// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * AMS_Data/AMS_DataFactoryImpl.cpp
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

#include <AMS_Data/AMS_DataFactory.hpp>
#include <AMS_Data/AMS_DataPackage.hpp>
#include <AMS_Data/Application.hpp>
#include <AMS_Data/ListApplications.hpp>
#include <AMS_Data/ListApplicationsOp.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::AMS_Data;

AMS_DataFactory::AMS_DataFactory()
{
    s_instance.reset(this);
}

::ecore::EObject_ptr AMS_DataFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case AMS_DataPackage::APPLICATION:
        return createApplication();
    case AMS_DataPackage::LISTAPPLICATIONS:
        return createListApplications();
    case AMS_DataPackage::LISTAPPLICATIONSOP:
        return createListApplicationsOp();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject AMS_DataFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const& _literalValue)
{
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString AMS_DataFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const& _instanceValue)
{
    switch (_eDataType->getClassifierID())
    {
    default:
        throw "IllegalArgumentException";
    }
}

Application_ptr AMS_DataFactory::createApplication()
{
    return new Application();
}
ListApplications_ptr AMS_DataFactory::createListApplications()
{
    return new ListApplications();
}
ListApplicationsOp_ptr AMS_DataFactory::createListApplicationsOp()
{
    return new ListApplicationsOp();
}

