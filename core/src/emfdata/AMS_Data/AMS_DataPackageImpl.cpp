// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * AMS_Data/AMS_DataPackageImpl.cpp
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

#include <AMS_Data/AMS_DataPackage.hpp>
#include <AMS_Data/AMS_DataFactory.hpp>
#include <ecore.hpp>
#include <ecore/EcorePackage.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EAttribute.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecore/EParameter.hpp>
#include <ecore/EEnum.hpp>
#include <ecore/EEnumLiteral.hpp>
#include <ecore/EDataType.hpp>
#include <ecore/EGenericType.hpp>
#include <ecore/ETypeParameter.hpp>
#include <ecore/EcorePackage.hpp>

using namespace ::AMS_Data;

AMS_DataPackage::AMS_DataPackage()
{

    s_instance.reset(this);

    // Factory
    ::ecore::EFactory_ptr _fa = AMS_DataFactory::_instance();
    setEFactoryInstance(_fa);
    _fa->setEPackage(this);

    // Create classes and their features

    // Application
    m_ApplicationEClass = new ::ecore::EClass();
    m_ApplicationEClass->setClassifierID(APPLICATION);
    m_ApplicationEClass->setEPackage(this);
    getEClassifiers().push_back(m_ApplicationEClass);
    m_Application__name = new ::ecore::EAttribute();
    m_Application__name->setFeatureID(
            ::AMS_Data::AMS_DataPackage::APPLICATION__NAME);
    m_ApplicationEClass->getEStructuralFeatures().push_back(m_Application__name);
    m_Application__description = new ::ecore::EAttribute();
    m_Application__description->setFeatureID(
            ::AMS_Data::AMS_DataPackage::APPLICATION__DESCRIPTION);
    m_ApplicationEClass->getEStructuralFeatures().push_back(
            m_Application__description);
    m_Application__input = new ::ecore::EAttribute();
    m_Application__input->setFeatureID(
            ::AMS_Data::AMS_DataPackage::APPLICATION__INPUT);
    m_ApplicationEClass->getEStructuralFeatures().push_back(
            m_Application__input);
    m_Application__output = new ::ecore::EAttribute();
    m_Application__output->setFeatureID(
            ::AMS_Data::AMS_DataPackage::APPLICATION__OUTPUT);
    m_ApplicationEClass->getEStructuralFeatures().push_back(
            m_Application__output);
    m_Application__script = new ::ecore::EAttribute();
    m_Application__script->setFeatureID(
            ::AMS_Data::AMS_DataPackage::APPLICATION__SCRIPT);
    m_ApplicationEClass->getEStructuralFeatures().push_back(
            m_Application__script);
    m_Application__applicationId = new ::ecore::EAttribute();
    m_Application__applicationId->setFeatureID(
            ::AMS_Data::AMS_DataPackage::APPLICATION__APPLICATIONID);
    m_ApplicationEClass->getEStructuralFeatures().push_back(
            m_Application__applicationId);

    // ListApplications
    m_ListApplicationsEClass = new ::ecore::EClass();
    m_ListApplicationsEClass->setClassifierID(LISTAPPLICATIONS);
    m_ListApplicationsEClass->setEPackage(this);
    getEClassifiers().push_back(m_ListApplicationsEClass);
    m_ListApplications__appl = new ::ecore::EReference();
    m_ListApplications__appl->setFeatureID(
            ::AMS_Data::AMS_DataPackage::LISTAPPLICATIONS__APPL);
    m_ListApplicationsEClass->getEStructuralFeatures().push_back(
            m_ListApplications__appl);

    // ListApplicationsOp
    m_ListApplicationsOpEClass = new ::ecore::EClass();
    m_ListApplicationsOpEClass->setClassifierID(LISTAPPLICATIONSOP);
    m_ListApplicationsOpEClass->setEPackage(this);
    getEClassifiers().push_back(m_ListApplicationsOpEClass);
    m_ListApplicationsOp__machineId = new ::ecore::EAttribute();
    m_ListApplicationsOp__machineId->setFeatureID(
            ::AMS_Data::AMS_DataPackage::LISTAPPLICATIONSOP__MACHINEID);
    m_ListApplicationsOpEClass->getEStructuralFeatures().push_back(
            m_ListApplicationsOp__machineId);

    // Create enums


    // Create data types


    // Initialize package
    setName("AMS_Data");
    setNsPrefix("");
    setNsURI("http://www.sysfera.com/AMS/Data");

    // TODO: bounds for type parameters

    // Add supertypes to classes

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // Application
    m_ApplicationEClass->setName("Application");
    m_ApplicationEClass->setAbstract(false);
    m_ApplicationEClass->setInterface(false);
    m_Application__name->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Application__name->setName("name");
    m_Application__name->setDefaultValueLiteral("" "");
    m_Application__name->setLowerBound(0);
    m_Application__name->setUpperBound(1);
    m_Application__name->setTransient(false);
    m_Application__name->setVolatile(false);
    m_Application__name->setChangeable(true);
    m_Application__name->setUnsettable(false);
    m_Application__name->setID(false);
    m_Application__name->setUnique(true);
    m_Application__name->setDerived(false);
    m_Application__name->setOrdered(true);
    m_Application__description->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Application__description->setName("description");
    m_Application__description->setDefaultValueLiteral("" "");
    m_Application__description->setLowerBound(0);
    m_Application__description->setUpperBound(1);
    m_Application__description->setTransient(true);
    m_Application__description->setVolatile(false);
    m_Application__description->setChangeable(true);
    m_Application__description->setUnsettable(false);
    m_Application__description->setID(false);
    m_Application__description->setUnique(true);
    m_Application__description->setDerived(false);
    m_Application__description->setOrdered(true);
    m_Application__input->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Application__input->setName("input");
    m_Application__input->setDefaultValueLiteral("" "");
    m_Application__input->setLowerBound(0);
    m_Application__input->setUpperBound(1);
    m_Application__input->setTransient(false);
    m_Application__input->setVolatile(false);
    m_Application__input->setChangeable(true);
    m_Application__input->setUnsettable(false);
    m_Application__input->setID(false);
    m_Application__input->setUnique(true);
    m_Application__input->setDerived(false);
    m_Application__input->setOrdered(true);
    m_Application__output->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Application__output->setName("output");
    m_Application__output->setDefaultValueLiteral("" "");
    m_Application__output->setLowerBound(0);
    m_Application__output->setUpperBound(1);
    m_Application__output->setTransient(false);
    m_Application__output->setVolatile(false);
    m_Application__output->setChangeable(true);
    m_Application__output->setUnsettable(false);
    m_Application__output->setID(false);
    m_Application__output->setUnique(true);
    m_Application__output->setDerived(false);
    m_Application__output->setOrdered(true);
    m_Application__script->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Application__script->setName("script");
    m_Application__script->setDefaultValueLiteral("" "");
    m_Application__script->setLowerBound(0);
    m_Application__script->setUpperBound(1);
    m_Application__script->setTransient(false);
    m_Application__script->setVolatile(false);
    m_Application__script->setChangeable(true);
    m_Application__script->setUnsettable(false);
    m_Application__script->setID(false);
    m_Application__script->setUnique(true);
    m_Application__script->setDerived(false);
    m_Application__script->setOrdered(true);
    m_Application__applicationId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Application__applicationId->setName("applicationId");
    m_Application__applicationId->setDefaultValueLiteral("" "");
    m_Application__applicationId->setLowerBound(0);
    m_Application__applicationId->setUpperBound(1);
    m_Application__applicationId->setTransient(false);
    m_Application__applicationId->setVolatile(false);
    m_Application__applicationId->setChangeable(true);
    m_Application__applicationId->setUnsettable(false);
    m_Application__applicationId->setID(false);
    m_Application__applicationId->setUnique(true);
    m_Application__applicationId->setDerived(false);
    m_Application__applicationId->setOrdered(true);
    // ListApplications
    m_ListApplicationsEClass->setName("ListApplications");
    m_ListApplicationsEClass->setAbstract(false);
    m_ListApplicationsEClass->setInterface(false);
    m_ListApplications__appl->setEType(m_ApplicationEClass);
    m_ListApplications__appl->setName("appl");
    m_ListApplications__appl->setDefaultValueLiteral("");
    m_ListApplications__appl->setLowerBound(0);
    m_ListApplications__appl->setUpperBound(-1);
    m_ListApplications__appl->setTransient(false);
    m_ListApplications__appl->setVolatile(false);
    m_ListApplications__appl->setChangeable(true);
    m_ListApplications__appl->setContainment(true);
    m_ListApplications__appl->setResolveProxies(true);
    m_ListApplications__appl->setUnique(true);
    m_ListApplications__appl->setDerived(false);
    m_ListApplications__appl->setOrdered(true);
    // ListApplicationsOp
    m_ListApplicationsOpEClass->setName("ListApplicationsOp");
    m_ListApplicationsOpEClass->setAbstract(false);
    m_ListApplicationsOpEClass->setInterface(false);
    m_ListApplicationsOp__machineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_ListApplicationsOp__machineId->setName("machineId");
    m_ListApplicationsOp__machineId->setDefaultValueLiteral("" "");
    m_ListApplicationsOp__machineId->setLowerBound(0);
    m_ListApplicationsOp__machineId->setUpperBound(1);
    m_ListApplicationsOp__machineId->setTransient(false);
    m_ListApplicationsOp__machineId->setVolatile(false);
    m_ListApplicationsOp__machineId->setChangeable(true);
    m_ListApplicationsOp__machineId->setUnsettable(false);
    m_ListApplicationsOp__machineId->setID(false);
    m_ListApplicationsOp__machineId->setUnique(true);
    m_ListApplicationsOp__machineId->setDerived(false);
    m_ListApplicationsOp__machineId->setOrdered(true);

    // TODO: Initialize data types


    _initialize();
}

::ecore::EClass_ptr AMS_DataPackage::getApplication()
{
    return m_ApplicationEClass;
}
::ecore::EClass_ptr AMS_DataPackage::getListApplications()
{
    return m_ListApplicationsEClass;
}
::ecore::EClass_ptr AMS_DataPackage::getListApplicationsOp()
{
    return m_ListApplicationsOpEClass;
}

::ecore::EAttribute_ptr AMS_DataPackage::getApplication__name()
{
    return m_Application__name;
}
::ecore::EAttribute_ptr AMS_DataPackage::getApplication__description()
{
    return m_Application__description;
}
::ecore::EAttribute_ptr AMS_DataPackage::getApplication__input()
{
    return m_Application__input;
}
::ecore::EAttribute_ptr AMS_DataPackage::getApplication__output()
{
    return m_Application__output;
}
::ecore::EAttribute_ptr AMS_DataPackage::getApplication__script()
{
    return m_Application__script;
}
::ecore::EAttribute_ptr AMS_DataPackage::getApplication__applicationId()
{
    return m_Application__applicationId;
}
::ecore::EReference_ptr AMS_DataPackage::getListApplications__appl()
{
    return m_ListApplications__appl;
}
::ecore::EAttribute_ptr AMS_DataPackage::getListApplicationsOp__machineId()
{
    return m_ListApplicationsOp__machineId;
}

