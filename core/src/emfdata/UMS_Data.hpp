// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data.hpp
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

#ifndef UMS_DATA_HPP
#define UMS_DATA_HPP

#include "UMS_Data_forward.hpp"
#include <ecorecpp/mapping.hpp>

// EPackage


// EClass
#include "UMS_Data/ConnectOptions.hpp"
#include "UMS_Data/Session.hpp"
#include "UMS_Data/ListSessions.hpp"
#include "UMS_Data/ListSessionOptions.hpp"
#include "UMS_Data/User.hpp"
#include "UMS_Data/ListUsers.hpp"
#include "UMS_Data/Command.hpp"
#include "UMS_Data/ListCommands.hpp"
#include "UMS_Data/ListCmdOptions.hpp"
#include "UMS_Data/LocalAccount.hpp"
#include "UMS_Data/ListLocalAccounts.hpp"
#include "UMS_Data/ListLocalAccOptions.hpp"
#include "UMS_Data/Machine.hpp"
#include "UMS_Data/ListMachines.hpp"
#include "UMS_Data/ListMachineOptions.hpp"
#include "UMS_Data/Configuration.hpp"
#include "UMS_Data/OptionValue.hpp"
#include "UMS_Data/ListOptionsValues.hpp"
#include "UMS_Data/ListOptOptions.hpp"

// Package & Factory
#include "UMS_Data/UMS_DataPackage.hpp"
#include "UMS_Data/UMS_DataFactory.hpp"

#endif // UMS_DATA_HPP
