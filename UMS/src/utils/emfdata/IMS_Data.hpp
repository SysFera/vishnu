// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data.hpp
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

#ifndef IMS_DATA_HPP
#define IMS_DATA_HPP

#include "IMS_Data_forward.hpp"
#include <ecorecpp/mapping.hpp>

// EPackage


// EClass
#include "IMS_Data/Metric.hpp"
#include "IMS_Data/ListMetric.hpp"
#include "IMS_Data/ListProcesses.hpp"
#include "IMS_Data/SystemInfo.hpp"
#include "IMS_Data/Process.hpp"
#include "IMS_Data/ExportOp.hpp"
#include "IMS_Data/MetricHistOp.hpp"
#include "IMS_Data/RestartOp.hpp"
#include "IMS_Data/CurMetricOp.hpp"
#include "IMS_Data/ProcessOp.hpp"
#include "IMS_Data/ThresholdOp.hpp"
#include "IMS_Data/ListThreshold.hpp"
#include "IMS_Data/Threshold.hpp"
#include "IMS_Data/SysInfoOp.hpp"
#include "IMS_Data/ListSysInfo.hpp"

// Package & Factory
#include "IMS_Data/IMS_DataPackage.hpp"
#include "IMS_Data/IMS_DataFactory.hpp"

#endif // IMS_DATA_HPP
