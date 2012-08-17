// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/IMS_DataFactoryImpl.cpp
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

#include <IMS_Data/IMS_DataFactory.hpp>
#include <IMS_Data/IMS_DataPackage.hpp>
#include <IMS_Data/Metric.hpp>
#include <IMS_Data/ListMetric.hpp>
#include <IMS_Data/ListProcesses.hpp>
#include <IMS_Data/SystemInfo.hpp>
#include <IMS_Data/Process.hpp>
#include <IMS_Data/ExportOp.hpp>
#include <IMS_Data/MetricHistOp.hpp>
#include <IMS_Data/CurMetricOp.hpp>
#include <IMS_Data/ProcessOp.hpp>
#include <IMS_Data/ThresholdOp.hpp>
#include <IMS_Data/ListThreshold.hpp>
#include <IMS_Data/Threshold.hpp>
#include <IMS_Data/SysInfoOp.hpp>
#include <IMS_Data/ListSysInfo.hpp>
#include <IMS_Data/SupervisorOp.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::IMS_Data;

IMS_DataFactory::IMS_DataFactory()
{
    s_instance.reset(this);
}

::ecore::EObject_ptr IMS_DataFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case IMS_DataPackage::METRIC:
        return createMetric();
    case IMS_DataPackage::LISTMETRIC:
        return createListMetric();
    case IMS_DataPackage::LISTPROCESSES:
        return createListProcesses();
    case IMS_DataPackage::SYSTEMINFO:
        return createSystemInfo();
    case IMS_DataPackage::PROCESS:
        return createProcess();
    case IMS_DataPackage::EXPORTOP:
        return createExportOp();
    case IMS_DataPackage::METRICHISTOP:
        return createMetricHistOp();
    case IMS_DataPackage::CURMETRICOP:
        return createCurMetricOp();
    case IMS_DataPackage::PROCESSOP:
        return createProcessOp();
    case IMS_DataPackage::THRESHOLDOP:
        return createThresholdOp();
    case IMS_DataPackage::LISTTHRESHOLD:
        return createListThreshold();
    case IMS_DataPackage::THRESHOLD:
        return createThreshold();
    case IMS_DataPackage::SYSINFOOP:
        return createSysInfoOp();
    case IMS_DataPackage::LISTSYSINFO:
        return createListSysInfo();
    case IMS_DataPackage::SUPERVISOROP:
        return createSupervisorOp();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject IMS_DataFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const& _literalValue)
{
    switch (_eDataType->getClassifierID())
    {
    case IMS_DataPackage::METRICTYPE:
    {
        ::ecore::EJavaObject _any;
        IMS_DataPackage_ptr _epkg =
                dynamic_cast< ::IMS_Data::IMS_DataPackage_ptr > (getEPackage());
        return _epkg->getMetricType()->getEEnumLiteralByLiteral(_literalValue)->getValue();
    }
    case IMS_DataPackage::LOADSHEDTYPE:
    {
        ::ecore::EJavaObject _any;
        IMS_DataPackage_ptr _epkg =
                dynamic_cast< ::IMS_Data::IMS_DataPackage_ptr > (getEPackage());
        return _epkg->getLoadShedType()->getEEnumLiteralByLiteral(_literalValue)->getValue();
    }
    case IMS_DataPackage::EXPORTTYPE:
    {
        ::ecore::EJavaObject _any;
        IMS_DataPackage_ptr _epkg =
                dynamic_cast< ::IMS_Data::IMS_DataPackage_ptr > (getEPackage());
        return _epkg->getExportType()->getEEnumLiteralByLiteral(_literalValue)->getValue();
    }
    case IMS_DataPackage::PROCESSSTATE:
    {
        ::ecore::EJavaObject _any;
        IMS_DataPackage_ptr _epkg =
                dynamic_cast< ::IMS_Data::IMS_DataPackage_ptr > (getEPackage());
        return _epkg->getProcessState()->getEEnumLiteralByLiteral(_literalValue)->getValue();
    }
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString IMS_DataFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const& _instanceValue)
{
    switch (_eDataType->getClassifierID())
    {
    case IMS_DataPackage::METRICTYPE:
    {
        IMS_DataPackage_ptr _epkg = ::IMS_Data::instanceOf<
                ::IMS_Data::IMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getMetricType()->getEEnumLiteral(_value)->getName();
    }
    case IMS_DataPackage::LOADSHEDTYPE:
    {
        IMS_DataPackage_ptr _epkg = ::IMS_Data::instanceOf<
                ::IMS_Data::IMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getLoadShedType()->getEEnumLiteral(_value)->getName();
    }
    case IMS_DataPackage::EXPORTTYPE:
    {
        IMS_DataPackage_ptr _epkg = ::IMS_Data::instanceOf<
                ::IMS_Data::IMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getExportType()->getEEnumLiteral(_value)->getName();
    }
    case IMS_DataPackage::PROCESSSTATE:
    {
        IMS_DataPackage_ptr _epkg = ::IMS_Data::instanceOf<
                ::IMS_Data::IMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getProcessState()->getEEnumLiteral(_value)->getName();
    }
    default:
        throw "IllegalArgumentException";
    }
}

Metric_ptr IMS_DataFactory::createMetric()
{
    return new Metric();
}
ListMetric_ptr IMS_DataFactory::createListMetric()
{
    return new ListMetric();
}
ListProcesses_ptr IMS_DataFactory::createListProcesses()
{
    return new ListProcesses();
}
SystemInfo_ptr IMS_DataFactory::createSystemInfo()
{
    return new SystemInfo();
}
Process_ptr IMS_DataFactory::createProcess()
{
    return new Process();
}
ExportOp_ptr IMS_DataFactory::createExportOp()
{
    return new ExportOp();
}
MetricHistOp_ptr IMS_DataFactory::createMetricHistOp()
{
    return new MetricHistOp();
}
CurMetricOp_ptr IMS_DataFactory::createCurMetricOp()
{
    return new CurMetricOp();
}
ProcessOp_ptr IMS_DataFactory::createProcessOp()
{
    return new ProcessOp();
}
ThresholdOp_ptr IMS_DataFactory::createThresholdOp()
{
    return new ThresholdOp();
}
ListThreshold_ptr IMS_DataFactory::createListThreshold()
{
    return new ListThreshold();
}
Threshold_ptr IMS_DataFactory::createThreshold()
{
    return new Threshold();
}
SysInfoOp_ptr IMS_DataFactory::createSysInfoOp()
{
    return new SysInfoOp();
}
ListSysInfo_ptr IMS_DataFactory::createListSysInfo()
{
    return new ListSysInfo();
}
SupervisorOp_ptr IMS_DataFactory::createSupervisorOp()
{
    return new SupervisorOp();
}

