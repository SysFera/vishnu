// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/IMS_DataPackage.hpp
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

/**
 * \file IMS_DataPackage.hpp
 * \brief The IMS_DataPackage class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef _IMS_DATAPACKAGE_HPP
#define _IMS_DATAPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <IMS_Data_forward.hpp>

namespace IMS_Data
{
    /**
     * \class IMS_DataPackage
     * \brief The package class (reflective API) for generated model IMS_Data
     */
    class IMS_DataPackage: public virtual ::ecore::EPackage
    {
    public:

        /**
         * \brief To get the instance of the singleton
         * \return Pointer to the instance
         */
        static IMS_DataPackage_ptr _instance();

        // IDs for classifiers

        /**
         * \brief Constant for CurMetricOp class
         */
        static const int CURMETRICOP = 0;

        /**
         * \brief Constant for ExportOp class
         */
        static const int EXPORTOP = 1;

        /**
         * \brief Constant for ExportType class
         */
        static const int EXPORTTYPE = 2;

        /**
         * \brief Constant for ListMetric class
         */
        static const int LISTMETRIC = 3;

        /**
         * \brief Constant for ListProcesses class
         */
        static const int LISTPROCESSES = 4;

        /**
         * \brief Constant for ListSysInfo class
         */
        static const int LISTSYSINFO = 5;

        /**
         * \brief Constant for ListThreshold class
         */
        static const int LISTTHRESHOLD = 6;

        /**
         * \brief Constant for LoadShedType class
         */
        static const int LOADSHEDTYPE = 7;

        /**
         * \brief Constant for Metric class
         */
        static const int METRIC = 8;

        /**
         * \brief Constant for MetricHistOp class
         */
        static const int METRICHISTOP = 9;

        /**
         * \brief Constant for MetricType class
         */
        static const int METRICTYPE = 10;

        /**
         * \brief Constant for Process class
         */
        static const int PROCESS = 11;

        /**
         * \brief Constant for ProcessOp class
         */
        static const int PROCESSOP = 12;

        /**
         * \brief Constant for ProcessState class
         */
        static const int PROCESSSTATE = 13;

        /**
         * \brief Constant for RestartOp class
         */
        static const int RESTARTOP = 14;

        /**
         * \brief Constant for SeDType class
         */
        static const int SEDTYPE = 15;

        /**
         * \brief Constant for SysInfoOp class
         */
        static const int SYSINFOOP = 16;

        /**
         * \brief Constant for SystemInfo class
         */
        static const int SYSTEMINFO = 17;

        /**
         * \brief Constant for Threshold class
         */
        static const int THRESHOLD = 18;

        /**
         * \brief Constant for ThresholdOp class
         */
        static const int THRESHOLDOP = 19;

        /**
         * \brief Constant for METRIC__TYPE feature
         */
        static const int METRIC__TYPE = 0;

        /**
         * \brief Constant for METRIC__VALUE feature
         */
        static const int METRIC__VALUE = 1;

        /**
         * \brief Constant for METRIC__TIME feature
         */
        static const int METRIC__TIME = 2;

        /**
         * \brief Constant for LISTMETRIC__METRIC feature
         */
        static const int LISTMETRIC__METRIC = 3;

        /**
         * \brief Constant for LISTPROCESSES__PROCESS feature
         */
        static const int LISTPROCESSES__PROCESS = 4;

        /**
         * \brief Constant for SYSTEMINFO__MEMORY feature
         */
        static const int SYSTEMINFO__MEMORY = 5;

        /**
         * \brief Constant for SYSTEMINFO__DISKSPACE feature
         */
        static const int SYSTEMINFO__DISKSPACE = 6;

        /**
         * \brief Constant for SYSTEMINFO__MACHINEID feature
         */
        static const int SYSTEMINFO__MACHINEID = 7;

        /**
         * \brief Constant for PROCESS__PROCESSNAME feature
         */
        static const int PROCESS__PROCESSNAME = 8;

        /**
         * \brief Constant for PROCESS__MACHINEID feature
         */
        static const int PROCESS__MACHINEID = 9;

        /**
         * \brief Constant for PROCESS__DIETID feature
         */
        static const int PROCESS__DIETID = 10;

        /**
         * \brief Constant for PROCESS__STATE feature
         */
        static const int PROCESS__STATE = 11;

        /**
         * \brief Constant for PROCESS__TIMESTAMP feature
         */
        static const int PROCESS__TIMESTAMP = 12;

        /**
         * \brief Constant for PROCESS__SCRIPT feature
         */
        static const int PROCESS__SCRIPT = 13;

        /**
         * \brief Constant for EXPORTOP__EXPORTTYPE feature
         */
        static const int EXPORTOP__EXPORTTYPE = 14;

        /**
         * \brief Constant for METRICHISTOP__STARTTIME feature
         */
        static const int METRICHISTOP__STARTTIME = 15;

        /**
         * \brief Constant for METRICHISTOP__ENDTIME feature
         */
        static const int METRICHISTOP__ENDTIME = 16;

        /**
         * \brief Constant for METRICHISTOP__TYPE feature
         */
        static const int METRICHISTOP__TYPE = 17;

        /**
         * \brief Constant for RESTARTOP__VISHNUCONF feature
         */
        static const int RESTARTOP__VISHNUCONF = 18;

        /**
         * \brief Constant for RESTARTOP__SEDTYPE feature
         */
        static const int RESTARTOP__SEDTYPE = 19;

        /**
         * \brief Constant for CURMETRICOP__METRICTYPE feature
         */
        static const int CURMETRICOP__METRICTYPE = 20;

        /**
         * \brief Constant for PROCESSOP__MACHINEID feature
         */
        static const int PROCESSOP__MACHINEID = 21;

        /**
         * \brief Constant for THRESHOLDOP__MACHINEID feature
         */
        static const int THRESHOLDOP__MACHINEID = 22;

        /**
         * \brief Constant for THRESHOLDOP__METRICTYPE feature
         */
        static const int THRESHOLDOP__METRICTYPE = 23;

        /**
         * \brief Constant for LISTTHRESHOLD__THRESHOLD feature
         */
        static const int LISTTHRESHOLD__THRESHOLD = 24;

        /**
         * \brief Constant for THRESHOLD__VALUE feature
         */
        static const int THRESHOLD__VALUE = 25;

        /**
         * \brief Constant for THRESHOLD__MACHINEID feature
         */
        static const int THRESHOLD__MACHINEID = 26;

        /**
         * \brief Constant for THRESHOLD__TYPE feature
         */
        static const int THRESHOLD__TYPE = 27;

        /**
         * \brief Constant for THRESHOLD__HANDLER feature
         */
        static const int THRESHOLD__HANDLER = 28;

        /**
         * \brief Constant for SYSINFOOP__MACHINEID feature
         */
        static const int SYSINFOOP__MACHINEID = 29;

        /**
         * \brief Constant for LISTSYSINFO__SYSINFO feature
         */
        static const int LISTSYSINFO__SYSINFO = 30;

        // EClassifiers methods

        /**
         * \brief Returns the reflective object for class MetricType
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EEnum_ptr getMetricType();

        /**
         * \brief Returns the reflective object for class Metric
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getMetric();

        /**
         * \brief Returns the reflective object for class LoadShedType
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EEnum_ptr getLoadShedType();

        /**
         * \brief Returns the reflective object for class ListMetric
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListMetric();

        /**
         * \brief Returns the reflective object for class ListProcesses
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListProcesses();

        /**
         * \brief Returns the reflective object for class SystemInfo
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getSystemInfo();

        /**
         * \brief Returns the reflective object for class Process
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getProcess();

        /**
         * \brief Returns the reflective object for class ExportOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getExportOp();

        /**
         * \brief Returns the reflective object for class ExportType
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EEnum_ptr getExportType();

        /**
         * \brief Returns the reflective object for class MetricHistOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getMetricHistOp();

        /**
         * \brief Returns the reflective object for class RestartOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getRestartOp();

        /**
         * \brief Returns the reflective object for class CurMetricOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getCurMetricOp();

        /**
         * \brief Returns the reflective object for class ProcessOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getProcessOp();

        /**
         * \brief Returns the reflective object for class ThresholdOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getThresholdOp();

        /**
         * \brief Returns the reflective object for class ListThreshold
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListThreshold();

        /**
         * \brief Returns the reflective object for class Threshold
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getThreshold();

        /**
         * \brief Returns the reflective object for class ProcessState
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EEnum_ptr getProcessState();

        /**
         * \brief Returns the reflective object for class SysInfoOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getSysInfoOp();

        /**
         * \brief Returns the reflective object for class ListSysInfo
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListSysInfo();

        /**
         * \brief Returns the reflective object for class SeDType
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EEnum_ptr getSeDType();

        // EStructuralFeatures methods

        /**
         * \brief Returns the reflective object for feature type of class Metric
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMetric__type();

        /**
         * \brief Returns the reflective object for feature value of class Metric
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMetric__value();

        /**
         * \brief Returns the reflective object for feature time of class Metric
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMetric__time();

        /**
         * \brief Returns the reflective object for feature metric of class ListMetric
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListMetric__metric();

        /**
         * \brief Returns the reflective object for feature process of class ListProcesses
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListProcesses__process();

        /**
         * \brief Returns the reflective object for feature memory of class SystemInfo
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSystemInfo__memory();

        /**
         * \brief Returns the reflective object for feature diskSpace of class SystemInfo
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSystemInfo__diskSpace();

        /**
         * \brief Returns the reflective object for feature machineId of class SystemInfo
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSystemInfo__machineId();

        /**
         * \brief Returns the reflective object for feature processName of class Process
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getProcess__processName();

        /**
         * \brief Returns the reflective object for feature machineId of class Process
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getProcess__machineId();

        /**
         * \brief Returns the reflective object for feature dietId of class Process
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getProcess__dietId();

        /**
         * \brief Returns the reflective object for feature state of class Process
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getProcess__state();

        /**
         * \brief Returns the reflective object for feature timestamp of class Process
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getProcess__timestamp();

        /**
         * \brief Returns the reflective object for feature script of class Process
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getProcess__script();

        /**
         * \brief Returns the reflective object for feature exportType of class ExportOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getExportOp__exportType();

        /**
         * \brief Returns the reflective object for feature startTime of class MetricHistOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMetricHistOp__startTime();

        /**
         * \brief Returns the reflective object for feature endTime of class MetricHistOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMetricHistOp__endTime();

        /**
         * \brief Returns the reflective object for feature type of class MetricHistOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getMetricHistOp__type();

        /**
         * \brief Returns the reflective object for feature vishnuConf of class RestartOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getRestartOp__vishnuConf();

        /**
         * \brief Returns the reflective object for feature sedType of class RestartOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getRestartOp__sedType();

        /**
         * \brief Returns the reflective object for feature metricType of class CurMetricOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getCurMetricOp__metricType();

        /**
         * \brief Returns the reflective object for feature machineId of class ProcessOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getProcessOp__machineId();

        /**
         * \brief Returns the reflective object for feature machineId of class ThresholdOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getThresholdOp__machineId();

        /**
         * \brief Returns the reflective object for feature metricType of class ThresholdOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getThresholdOp__metricType();

        /**
         * \brief Returns the reflective object for feature Threshold of class ListThreshold
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListThreshold__Threshold();

        /**
         * \brief Returns the reflective object for feature value of class Threshold
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getThreshold__value();

        /**
         * \brief Returns the reflective object for feature machineId of class Threshold
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getThreshold__machineId();

        /**
         * \brief Returns the reflective object for feature type of class Threshold
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getThreshold__type();

        /**
         * \brief Returns the reflective object for feature handler of class Threshold
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getThreshold__handler();

        /**
         * \brief Returns the reflective object for feature machineId of class SysInfoOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getSysInfoOp__machineId();

        /**
         * \brief Returns the reflective object for feature sysInfo of class ListSysInfo
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListSysInfo__sysInfo();

    protected:

        /**
         * \brief The instance of the package
         */
        static std::auto_ptr< IMS_DataPackage > s_instance;

        IMS_DataPackage();

        // EClass instances 

        /**
         * \brief The instance for the class Metric
         */
        ::ecore::EClass_ptr m_MetricEClass;

        /**
         * \brief The instance for the class ListMetric
         */
        ::ecore::EClass_ptr m_ListMetricEClass;

        /**
         * \brief The instance for the class ListProcesses
         */
        ::ecore::EClass_ptr m_ListProcessesEClass;

        /**
         * \brief The instance for the class SystemInfo
         */
        ::ecore::EClass_ptr m_SystemInfoEClass;

        /**
         * \brief The instance for the class Process
         */
        ::ecore::EClass_ptr m_ProcessEClass;

        /**
         * \brief The instance for the class ExportOp
         */
        ::ecore::EClass_ptr m_ExportOpEClass;

        /**
         * \brief The instance for the class MetricHistOp
         */
        ::ecore::EClass_ptr m_MetricHistOpEClass;

        /**
         * \brief The instance for the class RestartOp
         */
        ::ecore::EClass_ptr m_RestartOpEClass;

        /**
         * \brief The instance for the class CurMetricOp
         */
        ::ecore::EClass_ptr m_CurMetricOpEClass;

        /**
         * \brief The instance for the class ProcessOp
         */
        ::ecore::EClass_ptr m_ProcessOpEClass;

        /**
         * \brief The instance for the class ThresholdOp
         */
        ::ecore::EClass_ptr m_ThresholdOpEClass;

        /**
         * \brief The instance for the class ListThreshold
         */
        ::ecore::EClass_ptr m_ListThresholdEClass;

        /**
         * \brief The instance for the class Threshold
         */
        ::ecore::EClass_ptr m_ThresholdEClass;

        /**
         * \brief The instance for the class SysInfoOp
         */
        ::ecore::EClass_ptr m_SysInfoOpEClass;

        /**
         * \brief The instance for the class ListSysInfo
         */
        ::ecore::EClass_ptr m_ListSysInfoEClass;

        // EEnuminstances 

        /**
         * \brief The instance for the enum MetricType
         */
        ::ecore::EEnum_ptr m_MetricTypeEEnum;

        /**
         * \brief The instance for the enum LoadShedType
         */
        ::ecore::EEnum_ptr m_LoadShedTypeEEnum;

        /**
         * \brief The instance for the enum ExportType
         */
        ::ecore::EEnum_ptr m_ExportTypeEEnum;

        /**
         * \brief The instance for the enum ProcessState
         */
        ::ecore::EEnum_ptr m_ProcessStateEEnum;

        /**
         * \brief The instance for the enum SeDType
         */
        ::ecore::EEnum_ptr m_SeDTypeEEnum;

        // EDataType instances 


        // EStructuralFeatures instances

        /**
         * \brief The instance for the feature type of class Metric
         */
        ::ecore::EAttribute_ptr m_Metric__type;

        /**
         * \brief The instance for the feature value of class Metric
         */
        ::ecore::EAttribute_ptr m_Metric__value;

        /**
         * \brief The instance for the feature time of class Metric
         */
        ::ecore::EAttribute_ptr m_Metric__time;

        /**
         * \brief The instance for the feature metric of class ListMetric
         */
        ::ecore::EReference_ptr m_ListMetric__metric;

        /**
         * \brief The instance for the feature process of class ListProcesses
         */
        ::ecore::EReference_ptr m_ListProcesses__process;

        /**
         * \brief The instance for the feature memory of class SystemInfo
         */
        ::ecore::EAttribute_ptr m_SystemInfo__memory;

        /**
         * \brief The instance for the feature diskSpace of class SystemInfo
         */
        ::ecore::EAttribute_ptr m_SystemInfo__diskSpace;

        /**
         * \brief The instance for the feature machineId of class SystemInfo
         */
        ::ecore::EAttribute_ptr m_SystemInfo__machineId;

        /**
         * \brief The instance for the feature processName of class Process
         */
        ::ecore::EAttribute_ptr m_Process__processName;

        /**
         * \brief The instance for the feature machineId of class Process
         */
        ::ecore::EAttribute_ptr m_Process__machineId;

        /**
         * \brief The instance for the feature dietId of class Process
         */
        ::ecore::EAttribute_ptr m_Process__dietId;

        /**
         * \brief The instance for the feature state of class Process
         */
        ::ecore::EAttribute_ptr m_Process__state;

        /**
         * \brief The instance for the feature timestamp of class Process
         */
        ::ecore::EAttribute_ptr m_Process__timestamp;

        /**
         * \brief The instance for the feature script of class Process
         */
        ::ecore::EAttribute_ptr m_Process__script;

        /**
         * \brief The instance for the feature exportType of class ExportOp
         */
        ::ecore::EAttribute_ptr m_ExportOp__exportType;

        /**
         * \brief The instance for the feature startTime of class MetricHistOp
         */
        ::ecore::EAttribute_ptr m_MetricHistOp__startTime;

        /**
         * \brief The instance for the feature endTime of class MetricHistOp
         */
        ::ecore::EAttribute_ptr m_MetricHistOp__endTime;

        /**
         * \brief The instance for the feature type of class MetricHistOp
         */
        ::ecore::EAttribute_ptr m_MetricHistOp__type;

        /**
         * \brief The instance for the feature vishnuConf of class RestartOp
         */
        ::ecore::EAttribute_ptr m_RestartOp__vishnuConf;

        /**
         * \brief The instance for the feature sedType of class RestartOp
         */
        ::ecore::EAttribute_ptr m_RestartOp__sedType;

        /**
         * \brief The instance for the feature metricType of class CurMetricOp
         */
        ::ecore::EAttribute_ptr m_CurMetricOp__metricType;

        /**
         * \brief The instance for the feature machineId of class ProcessOp
         */
        ::ecore::EAttribute_ptr m_ProcessOp__machineId;

        /**
         * \brief The instance for the feature machineId of class ThresholdOp
         */
        ::ecore::EAttribute_ptr m_ThresholdOp__machineId;

        /**
         * \brief The instance for the feature metricType of class ThresholdOp
         */
        ::ecore::EAttribute_ptr m_ThresholdOp__metricType;

        /**
         * \brief The instance for the feature Threshold of class ListThreshold
         */
        ::ecore::EReference_ptr m_ListThreshold__Threshold;

        /**
         * \brief The instance for the feature value of class Threshold
         */
        ::ecore::EAttribute_ptr m_Threshold__value;

        /**
         * \brief The instance for the feature machineId of class Threshold
         */
        ::ecore::EAttribute_ptr m_Threshold__machineId;

        /**
         * \brief The instance for the feature type of class Threshold
         */
        ::ecore::EAttribute_ptr m_Threshold__type;

        /**
         * \brief The instance for the feature handler of class Threshold
         */
        ::ecore::EAttribute_ptr m_Threshold__handler;

        /**
         * \brief The instance for the feature machineId of class SysInfoOp
         */
        ::ecore::EAttribute_ptr m_SysInfoOp__machineId;

        /**
         * \brief The instance for the feature sysInfo of class ListSysInfo
         */
        ::ecore::EReference_ptr m_ListSysInfo__sysInfo;

    };

} // IMS_Data


#endif // _IMS_DATAPACKAGE_HPP
