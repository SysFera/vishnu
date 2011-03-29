// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data.hpp
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

#ifndef TMS_DATA_HPP
#define TMS_DATA_HPP

#include "TMS_Data_forward.hpp"
#include <ecorecpp/mapping.hpp>

// EPackage


// EClass
#include "TMS_Data/Job.hpp"
#include "TMS_Data/ListJobs.hpp"
#include "TMS_Data/SubmitOptions.hpp"
#include "TMS_Data/ListJobsOptions.hpp"
#include "TMS_Data/ProgressOptions.hpp"
#include "TMS_Data/ListProgression.hpp"
#include "TMS_Data/Progression.hpp"
#include "TMS_Data/ListQueues.hpp"
#include "TMS_Data/Queue.hpp"
#include "TMS_Data/JobResult.hpp"
#include "TMS_Data/ListJobResults.hpp"

// Package & Factory
#include "TMS_Data/TMS_DataPackage.hpp"
#include "TMS_Data/TMS_DataFactory.hpp"

#endif // TMS_DATA_HPP
