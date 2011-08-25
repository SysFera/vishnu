// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/IMS_DataPackageImpl.cpp
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

#include <IMS_Data/IMS_DataPackage.hpp>
#include <IMS_Data/IMS_DataFactory.hpp>
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

using namespace ::IMS_Data;

IMS_DataPackage::IMS_DataPackage()
{

    s_instance.reset(this);

    // Factory
    ::ecore::EFactory_ptr _fa = IMS_DataFactory::_instance();
    setEFactoryInstance(_fa);
    _fa->setEPackage(this);

    // Create classes and their features

    // Metric
    m_MetricEClass = new ::ecore::EClass();
    m_MetricEClass->setClassifierID(METRIC);
    m_MetricEClass->setEPackage(this);
    getEClassifiers().push_back(m_MetricEClass);
    m_Metric__type = new ::ecore::EAttribute();
    m_Metric__type->setFeatureID(::IMS_Data::IMS_DataPackage::METRIC__TYPE);
    m_MetricEClass->getEStructuralFeatures().push_back(m_Metric__type);
    m_Metric__value = new ::ecore::EAttribute();
    m_Metric__value->setFeatureID(::IMS_Data::IMS_DataPackage::METRIC__VALUE);
    m_MetricEClass->getEStructuralFeatures().push_back(m_Metric__value);
    m_Metric__time = new ::ecore::EAttribute();
    m_Metric__time->setFeatureID(::IMS_Data::IMS_DataPackage::METRIC__TIME);
    m_MetricEClass->getEStructuralFeatures().push_back(m_Metric__time);

    // ListMetric
    m_ListMetricEClass = new ::ecore::EClass();
    m_ListMetricEClass->setClassifierID(LISTMETRIC);
    m_ListMetricEClass->setEPackage(this);
    getEClassifiers().push_back(m_ListMetricEClass);
    m_ListMetric__metric = new ::ecore::EReference();
    m_ListMetric__metric->setFeatureID(
            ::IMS_Data::IMS_DataPackage::LISTMETRIC__METRIC);
    m_ListMetricEClass->getEStructuralFeatures().push_back(m_ListMetric__metric);

    // ListProcesses
    m_ListProcessesEClass = new ::ecore::EClass();
    m_ListProcessesEClass->setClassifierID(LISTPROCESSES);
    m_ListProcessesEClass->setEPackage(this);
    getEClassifiers().push_back(m_ListProcessesEClass);
    m_ListProcesses__process = new ::ecore::EReference();
    m_ListProcesses__process->setFeatureID(
            ::IMS_Data::IMS_DataPackage::LISTPROCESSES__PROCESS);
    m_ListProcessesEClass->getEStructuralFeatures().push_back(
            m_ListProcesses__process);

    // SystemInfo
    m_SystemInfoEClass = new ::ecore::EClass();
    m_SystemInfoEClass->setClassifierID(SYSTEMINFO);
    m_SystemInfoEClass->setEPackage(this);
    getEClassifiers().push_back(m_SystemInfoEClass);
    m_SystemInfo__memory = new ::ecore::EAttribute();
    m_SystemInfo__memory->setFeatureID(
            ::IMS_Data::IMS_DataPackage::SYSTEMINFO__MEMORY);
    m_SystemInfoEClass->getEStructuralFeatures().push_back(m_SystemInfo__memory);
    m_SystemInfo__diskSpace = new ::ecore::EAttribute();
    m_SystemInfo__diskSpace->setFeatureID(
            ::IMS_Data::IMS_DataPackage::SYSTEMINFO__DISKSPACE);
    m_SystemInfoEClass->getEStructuralFeatures().push_back(
            m_SystemInfo__diskSpace);
    m_SystemInfo__machineId = new ::ecore::EAttribute();
    m_SystemInfo__machineId->setFeatureID(
            ::IMS_Data::IMS_DataPackage::SYSTEMINFO__MACHINEID);
    m_SystemInfoEClass->getEStructuralFeatures().push_back(
            m_SystemInfo__machineId);

    // Process
    m_ProcessEClass = new ::ecore::EClass();
    m_ProcessEClass->setClassifierID(PROCESS);
    m_ProcessEClass->setEPackage(this);
    getEClassifiers().push_back(m_ProcessEClass);
    m_Process__processName = new ::ecore::EAttribute();
    m_Process__processName->setFeatureID(
            ::IMS_Data::IMS_DataPackage::PROCESS__PROCESSNAME);
    m_ProcessEClass->getEStructuralFeatures().push_back(m_Process__processName);
    m_Process__machineId = new ::ecore::EAttribute();
    m_Process__machineId->setFeatureID(
            ::IMS_Data::IMS_DataPackage::PROCESS__MACHINEID);
    m_ProcessEClass->getEStructuralFeatures().push_back(m_Process__machineId);
    m_Process__dietId = new ::ecore::EAttribute();
    m_Process__dietId->setFeatureID(
            ::IMS_Data::IMS_DataPackage::PROCESS__DIETID);
    m_ProcessEClass->getEStructuralFeatures().push_back(m_Process__dietId);
    m_Process__state = new ::ecore::EAttribute();
    m_Process__state->setFeatureID(::IMS_Data::IMS_DataPackage::PROCESS__STATE);
    m_ProcessEClass->getEStructuralFeatures().push_back(m_Process__state);
    m_Process__timestamp = new ::ecore::EAttribute();
    m_Process__timestamp->setFeatureID(
            ::IMS_Data::IMS_DataPackage::PROCESS__TIMESTAMP);
    m_ProcessEClass->getEStructuralFeatures().push_back(m_Process__timestamp);
    m_Process__script = new ::ecore::EAttribute();
    m_Process__script->setFeatureID(
            ::IMS_Data::IMS_DataPackage::PROCESS__SCRIPT);
    m_ProcessEClass->getEStructuralFeatures().push_back(m_Process__script);

    // ExportOp
    m_ExportOpEClass = new ::ecore::EClass();
    m_ExportOpEClass->setClassifierID(EXPORTOP);
    m_ExportOpEClass->setEPackage(this);
    getEClassifiers().push_back(m_ExportOpEClass);
    m_ExportOp__exportType = new ::ecore::EAttribute();
    m_ExportOp__exportType->setFeatureID(
            ::IMS_Data::IMS_DataPackage::EXPORTOP__EXPORTTYPE);
    m_ExportOpEClass->getEStructuralFeatures().push_back(m_ExportOp__exportType);

    // MetricHistOp
    m_MetricHistOpEClass = new ::ecore::EClass();
    m_MetricHistOpEClass->setClassifierID(METRICHISTOP);
    m_MetricHistOpEClass->setEPackage(this);
    getEClassifiers().push_back(m_MetricHistOpEClass);
    m_MetricHistOp__startTime = new ::ecore::EAttribute();
    m_MetricHistOp__startTime->setFeatureID(
            ::IMS_Data::IMS_DataPackage::METRICHISTOP__STARTTIME);
    m_MetricHistOpEClass->getEStructuralFeatures().push_back(
            m_MetricHistOp__startTime);
    m_MetricHistOp__endTime = new ::ecore::EAttribute();
    m_MetricHistOp__endTime->setFeatureID(
            ::IMS_Data::IMS_DataPackage::METRICHISTOP__ENDTIME);
    m_MetricHistOpEClass->getEStructuralFeatures().push_back(
            m_MetricHistOp__endTime);
    m_MetricHistOp__type = new ::ecore::EAttribute();
    m_MetricHistOp__type->setFeatureID(
            ::IMS_Data::IMS_DataPackage::METRICHISTOP__TYPE);
    m_MetricHistOpEClass->getEStructuralFeatures().push_back(
            m_MetricHistOp__type);

    // RestartOp
    m_RestartOpEClass = new ::ecore::EClass();
    m_RestartOpEClass->setClassifierID(RESTARTOP);
    m_RestartOpEClass->setEPackage(this);
    getEClassifiers().push_back(m_RestartOpEClass);
    m_RestartOp__vishnuConf = new ::ecore::EAttribute();
    m_RestartOp__vishnuConf->setFeatureID(
            ::IMS_Data::IMS_DataPackage::RESTARTOP__VISHNUCONF);
    m_RestartOpEClass->getEStructuralFeatures().push_back(
            m_RestartOp__vishnuConf);
    m_RestartOp__sedType = new ::ecore::EAttribute();
    m_RestartOp__sedType->setFeatureID(
            ::IMS_Data::IMS_DataPackage::RESTARTOP__SEDTYPE);
    m_RestartOpEClass->getEStructuralFeatures().push_back(m_RestartOp__sedType);

    // CurMetricOp
    m_CurMetricOpEClass = new ::ecore::EClass();
    m_CurMetricOpEClass->setClassifierID(CURMETRICOP);
    m_CurMetricOpEClass->setEPackage(this);
    getEClassifiers().push_back(m_CurMetricOpEClass);
    m_CurMetricOp__metricType = new ::ecore::EAttribute();
    m_CurMetricOp__metricType->setFeatureID(
            ::IMS_Data::IMS_DataPackage::CURMETRICOP__METRICTYPE);
    m_CurMetricOpEClass->getEStructuralFeatures().push_back(
            m_CurMetricOp__metricType);

    // ProcessOp
    m_ProcessOpEClass = new ::ecore::EClass();
    m_ProcessOpEClass->setClassifierID(PROCESSOP);
    m_ProcessOpEClass->setEPackage(this);
    getEClassifiers().push_back(m_ProcessOpEClass);
    m_ProcessOp__machineId = new ::ecore::EAttribute();
    m_ProcessOp__machineId->setFeatureID(
            ::IMS_Data::IMS_DataPackage::PROCESSOP__MACHINEID);
    m_ProcessOpEClass->getEStructuralFeatures().push_back(
            m_ProcessOp__machineId);

    // ThresholdOp
    m_ThresholdOpEClass = new ::ecore::EClass();
    m_ThresholdOpEClass->setClassifierID(THRESHOLDOP);
    m_ThresholdOpEClass->setEPackage(this);
    getEClassifiers().push_back(m_ThresholdOpEClass);
    m_ThresholdOp__machineId = new ::ecore::EAttribute();
    m_ThresholdOp__machineId->setFeatureID(
            ::IMS_Data::IMS_DataPackage::THRESHOLDOP__MACHINEID);
    m_ThresholdOpEClass->getEStructuralFeatures().push_back(
            m_ThresholdOp__machineId);
    m_ThresholdOp__metricType = new ::ecore::EAttribute();
    m_ThresholdOp__metricType->setFeatureID(
            ::IMS_Data::IMS_DataPackage::THRESHOLDOP__METRICTYPE);
    m_ThresholdOpEClass->getEStructuralFeatures().push_back(
            m_ThresholdOp__metricType);

    // ListThreshold
    m_ListThresholdEClass = new ::ecore::EClass();
    m_ListThresholdEClass->setClassifierID(LISTTHRESHOLD);
    m_ListThresholdEClass->setEPackage(this);
    getEClassifiers().push_back(m_ListThresholdEClass);
    m_ListThreshold__Threshold = new ::ecore::EReference();
    m_ListThreshold__Threshold->setFeatureID(
            ::IMS_Data::IMS_DataPackage::LISTTHRESHOLD__THRESHOLD);
    m_ListThresholdEClass->getEStructuralFeatures().push_back(
            m_ListThreshold__Threshold);

    // Threshold
    m_ThresholdEClass = new ::ecore::EClass();
    m_ThresholdEClass->setClassifierID(THRESHOLD);
    m_ThresholdEClass->setEPackage(this);
    getEClassifiers().push_back(m_ThresholdEClass);
    m_Threshold__value = new ::ecore::EAttribute();
    m_Threshold__value->setFeatureID(
            ::IMS_Data::IMS_DataPackage::THRESHOLD__VALUE);
    m_ThresholdEClass->getEStructuralFeatures().push_back(m_Threshold__value);
    m_Threshold__machineId = new ::ecore::EAttribute();
    m_Threshold__machineId->setFeatureID(
            ::IMS_Data::IMS_DataPackage::THRESHOLD__MACHINEID);
    m_ThresholdEClass->getEStructuralFeatures().push_back(
            m_Threshold__machineId);
    m_Threshold__type = new ::ecore::EAttribute();
    m_Threshold__type->setFeatureID(
            ::IMS_Data::IMS_DataPackage::THRESHOLD__TYPE);
    m_ThresholdEClass->getEStructuralFeatures().push_back(m_Threshold__type);
    m_Threshold__handler = new ::ecore::EAttribute();
    m_Threshold__handler->setFeatureID(
            ::IMS_Data::IMS_DataPackage::THRESHOLD__HANDLER);
    m_ThresholdEClass->getEStructuralFeatures().push_back(m_Threshold__handler);

    // SysInfoOp
    m_SysInfoOpEClass = new ::ecore::EClass();
    m_SysInfoOpEClass->setClassifierID(SYSINFOOP);
    m_SysInfoOpEClass->setEPackage(this);
    getEClassifiers().push_back(m_SysInfoOpEClass);
    m_SysInfoOp__machineId = new ::ecore::EAttribute();
    m_SysInfoOp__machineId->setFeatureID(
            ::IMS_Data::IMS_DataPackage::SYSINFOOP__MACHINEID);
    m_SysInfoOpEClass->getEStructuralFeatures().push_back(
            m_SysInfoOp__machineId);

    // ListSysInfo
    m_ListSysInfoEClass = new ::ecore::EClass();
    m_ListSysInfoEClass->setClassifierID(LISTSYSINFO);
    m_ListSysInfoEClass->setEPackage(this);
    getEClassifiers().push_back(m_ListSysInfoEClass);
    m_ListSysInfo__sysInfo = new ::ecore::EReference();
    m_ListSysInfo__sysInfo->setFeatureID(
            ::IMS_Data::IMS_DataPackage::LISTSYSINFO__SYSINFO);
    m_ListSysInfoEClass->getEStructuralFeatures().push_back(
            m_ListSysInfo__sysInfo);

    // Create enums

    m_MetricTypeEEnum = new ::ecore::EEnum();
    m_MetricTypeEEnum->setClassifierID(METRICTYPE);
    m_MetricTypeEEnum->setEPackage(this);
    getEClassifiers().push_back(m_MetricTypeEEnum);

    m_LoadShedTypeEEnum = new ::ecore::EEnum();
    m_LoadShedTypeEEnum->setClassifierID(LOADSHEDTYPE);
    m_LoadShedTypeEEnum->setEPackage(this);
    getEClassifiers().push_back(m_LoadShedTypeEEnum);

    m_ExportTypeEEnum = new ::ecore::EEnum();
    m_ExportTypeEEnum->setClassifierID(EXPORTTYPE);
    m_ExportTypeEEnum->setEPackage(this);
    getEClassifiers().push_back(m_ExportTypeEEnum);

    m_ProcessStateEEnum = new ::ecore::EEnum();
    m_ProcessStateEEnum->setClassifierID(PROCESSSTATE);
    m_ProcessStateEEnum->setEPackage(this);
    getEClassifiers().push_back(m_ProcessStateEEnum);

    m_SeDTypeEEnum = new ::ecore::EEnum();
    m_SeDTypeEEnum->setClassifierID(SEDTYPE);
    m_SeDTypeEEnum->setEPackage(this);
    getEClassifiers().push_back(m_SeDTypeEEnum);

    // Create data types


    // Initialize package
    setName("IMS_Data");
    setNsPrefix("");
    setNsURI("http://www.sysfera.com/IMS/Data");

    // TODO: bounds for type parameters

    // Add supertypes to classes

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // Metric
    m_MetricEClass->setName("Metric");
    m_MetricEClass->setAbstract(false);
    m_MetricEClass->setInterface(false);
    m_Metric__type->setEType(m_MetricTypeEEnum);
    m_Metric__type->setName("type");
    m_Metric__type->setDefaultValueLiteral("0");
    m_Metric__type->setLowerBound(0);
    m_Metric__type->setUpperBound(1);
    m_Metric__type->setTransient(false);
    m_Metric__type->setVolatile(false);
    m_Metric__type->setChangeable(true);
    m_Metric__type->setUnsettable(false);
    m_Metric__type->setID(false);
    m_Metric__type->setUnique(true);
    m_Metric__type->setDerived(false);
    m_Metric__type->setOrdered(true);
    m_Metric__value->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_Metric__value->setName("value");
    m_Metric__value->setDefaultValueLiteral("0");
    m_Metric__value->setLowerBound(0);
    m_Metric__value->setUpperBound(1);
    m_Metric__value->setTransient(false);
    m_Metric__value->setVolatile(false);
    m_Metric__value->setChangeable(true);
    m_Metric__value->setUnsettable(false);
    m_Metric__value->setID(false);
    m_Metric__value->setUnique(true);
    m_Metric__value->setDerived(false);
    m_Metric__value->setOrdered(true);
    m_Metric__time->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_Metric__time->setName("time");
    m_Metric__time->setDefaultValueLiteral("0");
    m_Metric__time->setLowerBound(0);
    m_Metric__time->setUpperBound(1);
    m_Metric__time->setTransient(false);
    m_Metric__time->setVolatile(false);
    m_Metric__time->setChangeable(true);
    m_Metric__time->setUnsettable(false);
    m_Metric__time->setID(false);
    m_Metric__time->setUnique(true);
    m_Metric__time->setDerived(false);
    m_Metric__time->setOrdered(true);
    // ListMetric
    m_ListMetricEClass->setName("ListMetric");
    m_ListMetricEClass->setAbstract(false);
    m_ListMetricEClass->setInterface(false);
    m_ListMetric__metric->setEType(m_MetricEClass);
    m_ListMetric__metric->setName("metric");
    m_ListMetric__metric->setDefaultValueLiteral("");
    m_ListMetric__metric->setLowerBound(0);
    m_ListMetric__metric->setUpperBound(-1);
    m_ListMetric__metric->setTransient(false);
    m_ListMetric__metric->setVolatile(false);
    m_ListMetric__metric->setChangeable(true);
    m_ListMetric__metric->setContainment(true);
    m_ListMetric__metric->setResolveProxies(true);
    m_ListMetric__metric->setUnique(true);
    m_ListMetric__metric->setDerived(false);
    m_ListMetric__metric->setOrdered(true);
    // ListProcesses
    m_ListProcessesEClass->setName("ListProcesses");
    m_ListProcessesEClass->setAbstract(false);
    m_ListProcessesEClass->setInterface(false);
    m_ListProcesses__process->setEType(m_ProcessEClass);
    m_ListProcesses__process->setName("process");
    m_ListProcesses__process->setDefaultValueLiteral("");
    m_ListProcesses__process->setLowerBound(0);
    m_ListProcesses__process->setUpperBound(-1);
    m_ListProcesses__process->setTransient(false);
    m_ListProcesses__process->setVolatile(false);
    m_ListProcesses__process->setChangeable(true);
    m_ListProcesses__process->setContainment(true);
    m_ListProcesses__process->setResolveProxies(true);
    m_ListProcesses__process->setUnique(true);
    m_ListProcesses__process->setDerived(false);
    m_ListProcesses__process->setOrdered(true);
    // SystemInfo
    m_SystemInfoEClass->setName("SystemInfo");
    m_SystemInfoEClass->setAbstract(false);
    m_SystemInfoEClass->setInterface(false);
    m_SystemInfo__memory->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_SystemInfo__memory->setName("memory");
    m_SystemInfo__memory->setDefaultValueLiteral("0");
    m_SystemInfo__memory->setLowerBound(0);
    m_SystemInfo__memory->setUpperBound(1);
    m_SystemInfo__memory->setTransient(false);
    m_SystemInfo__memory->setVolatile(false);
    m_SystemInfo__memory->setChangeable(true);
    m_SystemInfo__memory->setUnsettable(false);
    m_SystemInfo__memory->setID(false);
    m_SystemInfo__memory->setUnique(true);
    m_SystemInfo__memory->setDerived(false);
    m_SystemInfo__memory->setOrdered(true);
    m_SystemInfo__diskSpace->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_SystemInfo__diskSpace->setName("diskSpace");
    m_SystemInfo__diskSpace->setDefaultValueLiteral("0");
    m_SystemInfo__diskSpace->setLowerBound(0);
    m_SystemInfo__diskSpace->setUpperBound(1);
    m_SystemInfo__diskSpace->setTransient(false);
    m_SystemInfo__diskSpace->setVolatile(false);
    m_SystemInfo__diskSpace->setChangeable(true);
    m_SystemInfo__diskSpace->setUnsettable(false);
    m_SystemInfo__diskSpace->setID(false);
    m_SystemInfo__diskSpace->setUnique(true);
    m_SystemInfo__diskSpace->setDerived(false);
    m_SystemInfo__diskSpace->setOrdered(true);
    m_SystemInfo__machineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_SystemInfo__machineId->setName("machineId");
    m_SystemInfo__machineId->setDefaultValueLiteral("" "");
    m_SystemInfo__machineId->setLowerBound(0);
    m_SystemInfo__machineId->setUpperBound(1);
    m_SystemInfo__machineId->setTransient(false);
    m_SystemInfo__machineId->setVolatile(false);
    m_SystemInfo__machineId->setChangeable(true);
    m_SystemInfo__machineId->setUnsettable(false);
    m_SystemInfo__machineId->setID(false);
    m_SystemInfo__machineId->setUnique(true);
    m_SystemInfo__machineId->setDerived(false);
    m_SystemInfo__machineId->setOrdered(true);
    // Process
    m_ProcessEClass->setName("Process");
    m_ProcessEClass->setAbstract(false);
    m_ProcessEClass->setInterface(false);
    m_Process__processName->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Process__processName->setName("processName");
    m_Process__processName->setDefaultValueLiteral("" "");
    m_Process__processName->setLowerBound(0);
    m_Process__processName->setUpperBound(1);
    m_Process__processName->setTransient(false);
    m_Process__processName->setVolatile(false);
    m_Process__processName->setChangeable(true);
    m_Process__processName->setUnsettable(false);
    m_Process__processName->setID(false);
    m_Process__processName->setUnique(true);
    m_Process__processName->setDerived(false);
    m_Process__processName->setOrdered(true);
    m_Process__machineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Process__machineId->setName("machineId");
    m_Process__machineId->setDefaultValueLiteral("" "");
    m_Process__machineId->setLowerBound(0);
    m_Process__machineId->setUpperBound(1);
    m_Process__machineId->setTransient(false);
    m_Process__machineId->setVolatile(false);
    m_Process__machineId->setChangeable(true);
    m_Process__machineId->setUnsettable(false);
    m_Process__machineId->setID(false);
    m_Process__machineId->setUnique(true);
    m_Process__machineId->setDerived(false);
    m_Process__machineId->setOrdered(true);
    m_Process__dietId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Process__dietId->setName("dietId");
    m_Process__dietId->setDefaultValueLiteral("" "");
    m_Process__dietId->setLowerBound(0);
    m_Process__dietId->setUpperBound(1);
    m_Process__dietId->setTransient(false);
    m_Process__dietId->setVolatile(false);
    m_Process__dietId->setChangeable(true);
    m_Process__dietId->setUnsettable(false);
    m_Process__dietId->setID(false);
    m_Process__dietId->setUnique(true);
    m_Process__dietId->setDerived(false);
    m_Process__dietId->setOrdered(true);
    m_Process__state->setEType(m_ProcessStateEEnum);
    m_Process__state->setName("state");
    m_Process__state->setDefaultValueLiteral("0");
    m_Process__state->setLowerBound(0);
    m_Process__state->setUpperBound(1);
    m_Process__state->setTransient(false);
    m_Process__state->setVolatile(false);
    m_Process__state->setChangeable(true);
    m_Process__state->setUnsettable(false);
    m_Process__state->setID(false);
    m_Process__state->setUnique(true);
    m_Process__state->setDerived(false);
    m_Process__state->setOrdered(true);
    m_Process__timestamp->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_Process__timestamp->setName("timestamp");
    m_Process__timestamp->setDefaultValueLiteral("0");
    m_Process__timestamp->setLowerBound(0);
    m_Process__timestamp->setUpperBound(1);
    m_Process__timestamp->setTransient(false);
    m_Process__timestamp->setVolatile(false);
    m_Process__timestamp->setChangeable(true);
    m_Process__timestamp->setUnsettable(false);
    m_Process__timestamp->setID(false);
    m_Process__timestamp->setUnique(true);
    m_Process__timestamp->setDerived(false);
    m_Process__timestamp->setOrdered(true);
    m_Process__script->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Process__script->setName("script");
    m_Process__script->setDefaultValueLiteral("" "");
    m_Process__script->setLowerBound(0);
    m_Process__script->setUpperBound(1);
    m_Process__script->setTransient(false);
    m_Process__script->setVolatile(false);
    m_Process__script->setChangeable(true);
    m_Process__script->setUnsettable(false);
    m_Process__script->setID(false);
    m_Process__script->setUnique(true);
    m_Process__script->setDerived(false);
    m_Process__script->setOrdered(true);
    // ExportOp
    m_ExportOpEClass->setName("ExportOp");
    m_ExportOpEClass->setAbstract(false);
    m_ExportOpEClass->setInterface(false);
    m_ExportOp__exportType->setEType(m_ExportTypeEEnum);
    m_ExportOp__exportType->setName("exportType");
    m_ExportOp__exportType->setDefaultValueLiteral("0");
    m_ExportOp__exportType->setLowerBound(0);
    m_ExportOp__exportType->setUpperBound(1);
    m_ExportOp__exportType->setTransient(false);
    m_ExportOp__exportType->setVolatile(false);
    m_ExportOp__exportType->setChangeable(true);
    m_ExportOp__exportType->setUnsettable(false);
    m_ExportOp__exportType->setID(false);
    m_ExportOp__exportType->setUnique(true);
    m_ExportOp__exportType->setDerived(false);
    m_ExportOp__exportType->setOrdered(true);
    // MetricHistOp
    m_MetricHistOpEClass->setName("MetricHistOp");
    m_MetricHistOpEClass->setAbstract(false);
    m_MetricHistOpEClass->setInterface(false);
    m_MetricHistOp__startTime->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_MetricHistOp__startTime->setName("startTime");
    m_MetricHistOp__startTime->setDefaultValueLiteral("0");
    m_MetricHistOp__startTime->setLowerBound(0);
    m_MetricHistOp__startTime->setUpperBound(1);
    m_MetricHistOp__startTime->setTransient(false);
    m_MetricHistOp__startTime->setVolatile(false);
    m_MetricHistOp__startTime->setChangeable(true);
    m_MetricHistOp__startTime->setUnsettable(false);
    m_MetricHistOp__startTime->setID(false);
    m_MetricHistOp__startTime->setUnique(true);
    m_MetricHistOp__startTime->setDerived(false);
    m_MetricHistOp__startTime->setOrdered(true);
    m_MetricHistOp__endTime->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_MetricHistOp__endTime->setName("endTime");
    m_MetricHistOp__endTime->setDefaultValueLiteral("0");
    m_MetricHistOp__endTime->setLowerBound(0);
    m_MetricHistOp__endTime->setUpperBound(1);
    m_MetricHistOp__endTime->setTransient(false);
    m_MetricHistOp__endTime->setVolatile(false);
    m_MetricHistOp__endTime->setChangeable(true);
    m_MetricHistOp__endTime->setUnsettable(false);
    m_MetricHistOp__endTime->setID(false);
    m_MetricHistOp__endTime->setUnique(true);
    m_MetricHistOp__endTime->setDerived(false);
    m_MetricHistOp__endTime->setOrdered(true);
    m_MetricHistOp__type->setEType(m_MetricTypeEEnum);
    m_MetricHistOp__type->setName("type");
    m_MetricHistOp__type->setDefaultValueLiteral("0");
    m_MetricHistOp__type->setLowerBound(0);
    m_MetricHistOp__type->setUpperBound(1);
    m_MetricHistOp__type->setTransient(false);
    m_MetricHistOp__type->setVolatile(false);
    m_MetricHistOp__type->setChangeable(true);
    m_MetricHistOp__type->setUnsettable(false);
    m_MetricHistOp__type->setID(false);
    m_MetricHistOp__type->setUnique(true);
    m_MetricHistOp__type->setDerived(false);
    m_MetricHistOp__type->setOrdered(true);
    // RestartOp
    m_RestartOpEClass->setName("RestartOp");
    m_RestartOpEClass->setAbstract(false);
    m_RestartOpEClass->setInterface(false);
    m_RestartOp__vishnuConf->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_RestartOp__vishnuConf->setName("vishnuConf");
    m_RestartOp__vishnuConf->setDefaultValueLiteral("" "");
    m_RestartOp__vishnuConf->setLowerBound(0);
    m_RestartOp__vishnuConf->setUpperBound(1);
    m_RestartOp__vishnuConf->setTransient(false);
    m_RestartOp__vishnuConf->setVolatile(false);
    m_RestartOp__vishnuConf->setChangeable(true);
    m_RestartOp__vishnuConf->setUnsettable(false);
    m_RestartOp__vishnuConf->setID(false);
    m_RestartOp__vishnuConf->setUnique(true);
    m_RestartOp__vishnuConf->setDerived(false);
    m_RestartOp__vishnuConf->setOrdered(true);
    m_RestartOp__sedType->setEType(m_SeDTypeEEnum);
    m_RestartOp__sedType->setName("sedType");
    m_RestartOp__sedType->setDefaultValueLiteral("0");
    m_RestartOp__sedType->setLowerBound(0);
    m_RestartOp__sedType->setUpperBound(1);
    m_RestartOp__sedType->setTransient(false);
    m_RestartOp__sedType->setVolatile(false);
    m_RestartOp__sedType->setChangeable(true);
    m_RestartOp__sedType->setUnsettable(false);
    m_RestartOp__sedType->setID(false);
    m_RestartOp__sedType->setUnique(true);
    m_RestartOp__sedType->setDerived(false);
    m_RestartOp__sedType->setOrdered(true);
    // CurMetricOp
    m_CurMetricOpEClass->setName("CurMetricOp");
    m_CurMetricOpEClass->setAbstract(false);
    m_CurMetricOpEClass->setInterface(false);
    m_CurMetricOp__metricType->setEType(m_MetricTypeEEnum);
    m_CurMetricOp__metricType->setName("metricType");
    m_CurMetricOp__metricType->setDefaultValueLiteral("0");
    m_CurMetricOp__metricType->setLowerBound(0);
    m_CurMetricOp__metricType->setUpperBound(1);
    m_CurMetricOp__metricType->setTransient(false);
    m_CurMetricOp__metricType->setVolatile(false);
    m_CurMetricOp__metricType->setChangeable(true);
    m_CurMetricOp__metricType->setUnsettable(false);
    m_CurMetricOp__metricType->setID(false);
    m_CurMetricOp__metricType->setUnique(true);
    m_CurMetricOp__metricType->setDerived(false);
    m_CurMetricOp__metricType->setOrdered(true);
    // ProcessOp
    m_ProcessOpEClass->setName("ProcessOp");
    m_ProcessOpEClass->setAbstract(false);
    m_ProcessOpEClass->setInterface(false);
    m_ProcessOp__machineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_ProcessOp__machineId->setName("machineId");
    m_ProcessOp__machineId->setDefaultValueLiteral("" "");
    m_ProcessOp__machineId->setLowerBound(0);
    m_ProcessOp__machineId->setUpperBound(1);
    m_ProcessOp__machineId->setTransient(false);
    m_ProcessOp__machineId->setVolatile(false);
    m_ProcessOp__machineId->setChangeable(true);
    m_ProcessOp__machineId->setUnsettable(false);
    m_ProcessOp__machineId->setID(false);
    m_ProcessOp__machineId->setUnique(true);
    m_ProcessOp__machineId->setDerived(false);
    m_ProcessOp__machineId->setOrdered(true);
    // ThresholdOp
    m_ThresholdOpEClass->setName("ThresholdOp");
    m_ThresholdOpEClass->setAbstract(false);
    m_ThresholdOpEClass->setInterface(false);
    m_ThresholdOp__machineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_ThresholdOp__machineId->setName("machineId");
    m_ThresholdOp__machineId->setDefaultValueLiteral("" "");
    m_ThresholdOp__machineId->setLowerBound(0);
    m_ThresholdOp__machineId->setUpperBound(1);
    m_ThresholdOp__machineId->setTransient(false);
    m_ThresholdOp__machineId->setVolatile(false);
    m_ThresholdOp__machineId->setChangeable(true);
    m_ThresholdOp__machineId->setUnsettable(false);
    m_ThresholdOp__machineId->setID(false);
    m_ThresholdOp__machineId->setUnique(true);
    m_ThresholdOp__machineId->setDerived(false);
    m_ThresholdOp__machineId->setOrdered(true);
    m_ThresholdOp__metricType->setEType(m_MetricTypeEEnum);
    m_ThresholdOp__metricType->setName("metricType");
    m_ThresholdOp__metricType->setDefaultValueLiteral("0");
    m_ThresholdOp__metricType->setLowerBound(0);
    m_ThresholdOp__metricType->setUpperBound(1);
    m_ThresholdOp__metricType->setTransient(false);
    m_ThresholdOp__metricType->setVolatile(false);
    m_ThresholdOp__metricType->setChangeable(true);
    m_ThresholdOp__metricType->setUnsettable(false);
    m_ThresholdOp__metricType->setID(false);
    m_ThresholdOp__metricType->setUnique(true);
    m_ThresholdOp__metricType->setDerived(false);
    m_ThresholdOp__metricType->setOrdered(true);
    // ListThreshold
    m_ListThresholdEClass->setName("ListThreshold");
    m_ListThresholdEClass->setAbstract(false);
    m_ListThresholdEClass->setInterface(false);
    m_ListThreshold__Threshold->setEType(m_ThresholdEClass);
    m_ListThreshold__Threshold->setName("Threshold");
    m_ListThreshold__Threshold->setDefaultValueLiteral("");
    m_ListThreshold__Threshold->setLowerBound(0);
    m_ListThreshold__Threshold->setUpperBound(-1);
    m_ListThreshold__Threshold->setTransient(false);
    m_ListThreshold__Threshold->setVolatile(false);
    m_ListThreshold__Threshold->setChangeable(true);
    m_ListThreshold__Threshold->setContainment(true);
    m_ListThreshold__Threshold->setResolveProxies(true);
    m_ListThreshold__Threshold->setUnique(true);
    m_ListThreshold__Threshold->setDerived(false);
    m_ListThreshold__Threshold->setOrdered(true);
    // Threshold
    m_ThresholdEClass->setName("Threshold");
    m_ThresholdEClass->setAbstract(false);
    m_ThresholdEClass->setInterface(false);
    m_Threshold__value->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_Threshold__value->setName("value");
    m_Threshold__value->setDefaultValueLiteral("0");
    m_Threshold__value->setLowerBound(0);
    m_Threshold__value->setUpperBound(1);
    m_Threshold__value->setTransient(false);
    m_Threshold__value->setVolatile(false);
    m_Threshold__value->setChangeable(true);
    m_Threshold__value->setUnsettable(false);
    m_Threshold__value->setID(false);
    m_Threshold__value->setUnique(true);
    m_Threshold__value->setDerived(false);
    m_Threshold__value->setOrdered(true);
    m_Threshold__machineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Threshold__machineId->setName("machineId");
    m_Threshold__machineId->setDefaultValueLiteral("" "");
    m_Threshold__machineId->setLowerBound(0);
    m_Threshold__machineId->setUpperBound(1);
    m_Threshold__machineId->setTransient(false);
    m_Threshold__machineId->setVolatile(false);
    m_Threshold__machineId->setChangeable(true);
    m_Threshold__machineId->setUnsettable(false);
    m_Threshold__machineId->setID(false);
    m_Threshold__machineId->setUnique(true);
    m_Threshold__machineId->setDerived(false);
    m_Threshold__machineId->setOrdered(true);
    m_Threshold__type->setEType(m_MetricTypeEEnum);
    m_Threshold__type->setName("type");
    m_Threshold__type->setDefaultValueLiteral("0");
    m_Threshold__type->setLowerBound(0);
    m_Threshold__type->setUpperBound(1);
    m_Threshold__type->setTransient(false);
    m_Threshold__type->setVolatile(false);
    m_Threshold__type->setChangeable(true);
    m_Threshold__type->setUnsettable(false);
    m_Threshold__type->setID(false);
    m_Threshold__type->setUnique(true);
    m_Threshold__type->setDerived(false);
    m_Threshold__type->setOrdered(true);
    m_Threshold__handler->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_Threshold__handler->setName("handler");
    m_Threshold__handler->setDefaultValueLiteral("" "");
    m_Threshold__handler->setLowerBound(0);
    m_Threshold__handler->setUpperBound(1);
    m_Threshold__handler->setTransient(false);
    m_Threshold__handler->setVolatile(false);
    m_Threshold__handler->setChangeable(true);
    m_Threshold__handler->setUnsettable(false);
    m_Threshold__handler->setID(false);
    m_Threshold__handler->setUnique(true);
    m_Threshold__handler->setDerived(false);
    m_Threshold__handler->setOrdered(true);
    // SysInfoOp
    m_SysInfoOpEClass->setName("SysInfoOp");
    m_SysInfoOpEClass->setAbstract(false);
    m_SysInfoOpEClass->setInterface(false);
    m_SysInfoOp__machineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_SysInfoOp__machineId->setName("machineId");
    m_SysInfoOp__machineId->setDefaultValueLiteral("" "");
    m_SysInfoOp__machineId->setLowerBound(0);
    m_SysInfoOp__machineId->setUpperBound(1);
    m_SysInfoOp__machineId->setTransient(false);
    m_SysInfoOp__machineId->setVolatile(false);
    m_SysInfoOp__machineId->setChangeable(true);
    m_SysInfoOp__machineId->setUnsettable(false);
    m_SysInfoOp__machineId->setID(false);
    m_SysInfoOp__machineId->setUnique(true);
    m_SysInfoOp__machineId->setDerived(false);
    m_SysInfoOp__machineId->setOrdered(true);
    // ListSysInfo
    m_ListSysInfoEClass->setName("ListSysInfo");
    m_ListSysInfoEClass->setAbstract(false);
    m_ListSysInfoEClass->setInterface(false);
    m_ListSysInfo__sysInfo->setEType(m_SystemInfoEClass);
    m_ListSysInfo__sysInfo->setName("sysInfo");
    m_ListSysInfo__sysInfo->setDefaultValueLiteral("");
    m_ListSysInfo__sysInfo->setLowerBound(0);
    m_ListSysInfo__sysInfo->setUpperBound(-1);
    m_ListSysInfo__sysInfo->setTransient(false);
    m_ListSysInfo__sysInfo->setVolatile(false);
    m_ListSysInfo__sysInfo->setChangeable(true);
    m_ListSysInfo__sysInfo->setContainment(true);
    m_ListSysInfo__sysInfo->setResolveProxies(true);
    m_ListSysInfo__sysInfo->setUnique(true);
    m_ListSysInfo__sysInfo->setDerived(false);
    m_ListSysInfo__sysInfo->setOrdered(true);

    // TODO: Initialize data types


    // MetricType
    m_MetricTypeEEnum->setName("MetricType");
    m_MetricTypeEEnum->setSerializable(true);

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // UNDEFINED
        _el->setName("UNDEFINED");
        _el->setValue(0);
        _el->setLiteral("UNDEFINED");
        _el->setEEnum(m_MetricTypeEEnum);
        m_MetricTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // CPUUSE
        _el->setName("CPUUSE");
        _el->setValue(1);
        _el->setLiteral("CPUUSE");
        _el->setEEnum(m_MetricTypeEEnum);
        m_MetricTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // FREEDISKSPACE
        _el->setName("FREEDISKSPACE");
        _el->setValue(2);
        _el->setLiteral("FREEDISKSPACE");
        _el->setEEnum(m_MetricTypeEEnum);
        m_MetricTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // FREEMEMORY
        _el->setName("FREEMEMORY");
        _el->setValue(3);
        _el->setLiteral("FREEMEMORY");
        _el->setEEnum(m_MetricTypeEEnum);
        m_MetricTypeEEnum->getELiterals().push_back(_el);
    }

    // LoadShedType
    m_LoadShedTypeEEnum->setName("LoadShedType");
    m_LoadShedTypeEEnum->setSerializable(true);

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // SOFT
        _el->setName("SOFT");
        _el->setValue(0);
        _el->setLiteral("SOFT");
        _el->setEEnum(m_LoadShedTypeEEnum);
        m_LoadShedTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // HARD
        _el->setName("HARD");
        _el->setValue(1);
        _el->setLiteral("HARD");
        _el->setEEnum(m_LoadShedTypeEEnum);
        m_LoadShedTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // SOFT
        _el->setName("SOFT");
        _el->setValue(2);
        _el->setLiteral("SOFT");
        _el->setEEnum(m_LoadShedTypeEEnum);
        m_LoadShedTypeEEnum->getELiterals().push_back(_el);
    }

    // ExportType
    m_ExportTypeEEnum->setName("ExportType");
    m_ExportTypeEEnum->setSerializable(true);

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // UNDEFINED
        _el->setName("UNDEFINED");
        _el->setValue(0);
        _el->setLiteral("UNDEFINED");
        _el->setEEnum(m_ExportTypeEEnum);
        m_ExportTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // SHELL
        _el->setName("SHELL");
        _el->setValue(1);
        _el->setLiteral("SHELL");
        _el->setEEnum(m_ExportTypeEEnum);
        m_ExportTypeEEnum->getELiterals().push_back(_el);
    }

    // ProcessState
    m_ProcessStateEEnum->setName("ProcessState");
    m_ProcessStateEEnum->setSerializable(true);

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // UNDEFINED
        _el->setName("UNDEFINED");
        _el->setValue(0);
        _el->setLiteral("UNDEFINED");
        _el->setEEnum(m_ProcessStateEEnum);
        m_ProcessStateEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // RUNNING
        _el->setName("RUNNING");
        _el->setValue(1);
        _el->setLiteral("RUNNING");
        _el->setEEnum(m_ProcessStateEEnum);
        m_ProcessStateEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // DOWN
        _el->setName("DOWN");
        _el->setValue(2);
        _el->setLiteral("DOWN");
        _el->setEEnum(m_ProcessStateEEnum);
        m_ProcessStateEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // DELETED
        _el->setName("DELETED");
        _el->setValue(3);
        _el->setLiteral("DELETED");
        _el->setEEnum(m_ProcessStateEEnum);
        m_ProcessStateEEnum->getELiterals().push_back(_el);
    }

    // SeDType
    m_SeDTypeEEnum->setName("SeDType");
    m_SeDTypeEEnum->setSerializable(true);

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // UNDEFINED
        _el->setName("UNDEFINED");
        _el->setValue(0);
        _el->setLiteral("UNDEFINED");
        _el->setEEnum(m_SeDTypeEEnum);
        m_SeDTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // UMS
        _el->setName("UMS");
        _el->setValue(1);
        _el->setLiteral("UMS");
        _el->setEEnum(m_SeDTypeEEnum);
        m_SeDTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // TMS
        _el->setName("TMS");
        _el->setValue(2);
        _el->setLiteral("TMS");
        _el->setEEnum(m_SeDTypeEEnum);
        m_SeDTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // FMS
        _el->setName("FMS");
        _el->setValue(3);
        _el->setLiteral("FMS");
        _el->setEEnum(m_SeDTypeEEnum);
        m_SeDTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // IMS
        _el->setName("IMS");
        _el->setValue(4);
        _el->setLiteral("IMS");
        _el->setEEnum(m_SeDTypeEEnum);
        m_SeDTypeEEnum->getELiterals().push_back(_el);
    }

    _initialize();
}

::ecore::EEnum_ptr IMS_DataPackage::getMetricType()
{
    return m_MetricTypeEEnum;
}
::ecore::EClass_ptr IMS_DataPackage::getMetric()
{
    return m_MetricEClass;
}
::ecore::EEnum_ptr IMS_DataPackage::getLoadShedType()
{
    return m_LoadShedTypeEEnum;
}
::ecore::EClass_ptr IMS_DataPackage::getListMetric()
{
    return m_ListMetricEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getListProcesses()
{
    return m_ListProcessesEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getSystemInfo()
{
    return m_SystemInfoEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getProcess()
{
    return m_ProcessEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getExportOp()
{
    return m_ExportOpEClass;
}
::ecore::EEnum_ptr IMS_DataPackage::getExportType()
{
    return m_ExportTypeEEnum;
}
::ecore::EClass_ptr IMS_DataPackage::getMetricHistOp()
{
    return m_MetricHistOpEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getRestartOp()
{
    return m_RestartOpEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getCurMetricOp()
{
    return m_CurMetricOpEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getProcessOp()
{
    return m_ProcessOpEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getThresholdOp()
{
    return m_ThresholdOpEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getListThreshold()
{
    return m_ListThresholdEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getThreshold()
{
    return m_ThresholdEClass;
}
::ecore::EEnum_ptr IMS_DataPackage::getProcessState()
{
    return m_ProcessStateEEnum;
}
::ecore::EClass_ptr IMS_DataPackage::getSysInfoOp()
{
    return m_SysInfoOpEClass;
}
::ecore::EClass_ptr IMS_DataPackage::getListSysInfo()
{
    return m_ListSysInfoEClass;
}
::ecore::EEnum_ptr IMS_DataPackage::getSeDType()
{
    return m_SeDTypeEEnum;
}

::ecore::EAttribute_ptr IMS_DataPackage::getMetric__type()
{
    return m_Metric__type;
}
::ecore::EAttribute_ptr IMS_DataPackage::getMetric__value()
{
    return m_Metric__value;
}
::ecore::EAttribute_ptr IMS_DataPackage::getMetric__time()
{
    return m_Metric__time;
}
::ecore::EReference_ptr IMS_DataPackage::getListMetric__metric()
{
    return m_ListMetric__metric;
}
::ecore::EReference_ptr IMS_DataPackage::getListProcesses__process()
{
    return m_ListProcesses__process;
}
::ecore::EAttribute_ptr IMS_DataPackage::getSystemInfo__memory()
{
    return m_SystemInfo__memory;
}
::ecore::EAttribute_ptr IMS_DataPackage::getSystemInfo__diskSpace()
{
    return m_SystemInfo__diskSpace;
}
::ecore::EAttribute_ptr IMS_DataPackage::getSystemInfo__machineId()
{
    return m_SystemInfo__machineId;
}
::ecore::EAttribute_ptr IMS_DataPackage::getProcess__processName()
{
    return m_Process__processName;
}
::ecore::EAttribute_ptr IMS_DataPackage::getProcess__machineId()
{
    return m_Process__machineId;
}
::ecore::EAttribute_ptr IMS_DataPackage::getProcess__dietId()
{
    return m_Process__dietId;
}
::ecore::EAttribute_ptr IMS_DataPackage::getProcess__state()
{
    return m_Process__state;
}
::ecore::EAttribute_ptr IMS_DataPackage::getProcess__timestamp()
{
    return m_Process__timestamp;
}
::ecore::EAttribute_ptr IMS_DataPackage::getProcess__script()
{
    return m_Process__script;
}
::ecore::EAttribute_ptr IMS_DataPackage::getExportOp__exportType()
{
    return m_ExportOp__exportType;
}
::ecore::EAttribute_ptr IMS_DataPackage::getMetricHistOp__startTime()
{
    return m_MetricHistOp__startTime;
}
::ecore::EAttribute_ptr IMS_DataPackage::getMetricHistOp__endTime()
{
    return m_MetricHistOp__endTime;
}
::ecore::EAttribute_ptr IMS_DataPackage::getMetricHistOp__type()
{
    return m_MetricHistOp__type;
}
::ecore::EAttribute_ptr IMS_DataPackage::getRestartOp__vishnuConf()
{
    return m_RestartOp__vishnuConf;
}
::ecore::EAttribute_ptr IMS_DataPackage::getRestartOp__sedType()
{
    return m_RestartOp__sedType;
}
::ecore::EAttribute_ptr IMS_DataPackage::getCurMetricOp__metricType()
{
    return m_CurMetricOp__metricType;
}
::ecore::EAttribute_ptr IMS_DataPackage::getProcessOp__machineId()
{
    return m_ProcessOp__machineId;
}
::ecore::EAttribute_ptr IMS_DataPackage::getThresholdOp__machineId()
{
    return m_ThresholdOp__machineId;
}
::ecore::EAttribute_ptr IMS_DataPackage::getThresholdOp__metricType()
{
    return m_ThresholdOp__metricType;
}
::ecore::EReference_ptr IMS_DataPackage::getListThreshold__Threshold()
{
    return m_ListThreshold__Threshold;
}
::ecore::EAttribute_ptr IMS_DataPackage::getThreshold__value()
{
    return m_Threshold__value;
}
::ecore::EAttribute_ptr IMS_DataPackage::getThreshold__machineId()
{
    return m_Threshold__machineId;
}
::ecore::EAttribute_ptr IMS_DataPackage::getThreshold__type()
{
    return m_Threshold__type;
}
::ecore::EAttribute_ptr IMS_DataPackage::getThreshold__handler()
{
    return m_Threshold__handler;
}
::ecore::EAttribute_ptr IMS_DataPackage::getSysInfoOp__machineId()
{
    return m_SysInfoOp__machineId;
}
::ecore::EReference_ptr IMS_DataPackage::getListSysInfo__sysInfo()
{
    return m_ListSysInfo__sysInfo;
}

