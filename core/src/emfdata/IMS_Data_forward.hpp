// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data_forward.hpp
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

#ifndef _IMS_DATA_FORWARD_HPP
#define _IMS_DATA_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(IMS_Data_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage


#include <ecore_forward.hpp> // for EDataTypes
namespace IMS_Data
{

    // EDataType
    /*PROTECTED REGION ID(IMS_Data_MetricType) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    typedef int MetricType;
    /*PROTECTED REGION END*/
    /*PROTECTED REGION ID(IMS_Data_LoadShedType) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    typedef int LoadShedType;
    /*PROTECTED REGION END*/
    /*PROTECTED REGION ID(IMS_Data_ExportType) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    typedef int ExportType;
    /*PROTECTED REGION END*/
    /*PROTECTED REGION ID(IMS_Data_ProcessState) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    typedef int ProcessState;
    /*PROTECTED REGION END*/

    // EClass

    // Metric
    class Metric;
    typedef Metric* Metric_ptr;

    // ListMetric
    class ListMetric;
    typedef ListMetric* ListMetric_ptr;

    // ListProcesses
    class ListProcesses;
    typedef ListProcesses* ListProcesses_ptr;

    // SystemInfo
    class SystemInfo;
    typedef SystemInfo* SystemInfo_ptr;

    // Process
    class Process;
    typedef Process* Process_ptr;

    // ExportOp
    class ExportOp;
    typedef ExportOp* ExportOp_ptr;

    // MetricHistOp
    class MetricHistOp;
    typedef MetricHistOp* MetricHistOp_ptr;

    // CurMetricOp
    class CurMetricOp;
    typedef CurMetricOp* CurMetricOp_ptr;

    // ProcessOp
    class ProcessOp;
    typedef ProcessOp* ProcessOp_ptr;

    // ThresholdOp
    class ThresholdOp;
    typedef ThresholdOp* ThresholdOp_ptr;

    // ListThreshold
    class ListThreshold;
    typedef ListThreshold* ListThreshold_ptr;

    // Threshold
    class Threshold;
    typedef Threshold* Threshold_ptr;

    // SysInfoOp
    class SysInfoOp;
    typedef SysInfoOp* SysInfoOp_ptr;

    // ListSysInfo
    class ListSysInfo;
    typedef ListSysInfo* ListSysInfo_ptr;

    // SupervisorOp
    class SupervisorOp;
    typedef SupervisorOp* SupervisorOp_ptr;

    // Package & Factory
    class IMS_DataFactory;
    typedef IMS_DataFactory * IMS_DataFactory_ptr;
    class IMS_DataPackage;
    typedef IMS_DataPackage * IMS_DataPackage_ptr;

    template< typename T, typename S >
    inline T* instanceOf(S* _s)
    {
        return dynamic_cast< T* > (_s);
    }

} // IMS_Data


#endif // _IMS_DATA_FORWARD_HPP
