// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data.hpp
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

#ifndef FMS_DATA_HPP
#define FMS_DATA_HPP

#include "FMS_Data_forward.hpp"
#include <ecorecpp/mapping.hpp>

// EPackage


// EClass
#include "FMS_Data/FileStat.hpp"
#include "FMS_Data/CpFileOptions.hpp"
#include "FMS_Data/LsDirOptions.hpp"
#include "FMS_Data/MvFileOptions.hpp"
#include "FMS_Data/LsTransferOptions.hpp"
#include "FMS_Data/FileStatList.hpp"
#include "FMS_Data/StopTransferOptions.hpp"
#include "FMS_Data/FileTransfer.hpp"
#include "FMS_Data/FileTransferList.hpp"
#include "FMS_Data/HeadOfFileOptions.hpp"
#include "FMS_Data/TailOfFileOptions.hpp"
#include "FMS_Data/RmFileOptions.hpp"
#include "FMS_Data/CreateDirOptions.hpp"
#include "FMS_Data/DirEntry.hpp"
#include "FMS_Data/DirEntryList.hpp"

// Package & Factory
#include "FMS_Data/FMS_DataPackage.hpp"
#include "FMS_Data/FMS_DataFactory.hpp"

#endif // FMS_DATA_HPP
