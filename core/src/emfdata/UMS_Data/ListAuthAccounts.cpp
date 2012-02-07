// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListAuthAccounts.cpp
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

#include "ListAuthAccounts.hpp"
#include <UMS_Data/AuthAccount.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
ListAuthAccounts::ListAuthAccounts()
{

    m_AuthAccounts.reset(new ::ecorecpp::mapping::ReferenceEListImpl<
            ::UMS_Data::AuthAccount, -1, true, false >(this, NULL));

    /*PROTECTED REGION ID(ListAuthAccountsImpl__ListAuthAccountsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListAuthAccounts::~ListAuthAccounts()
{
}

/*PROTECTED REGION ID(ListAuthAccounts.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

// References
::ecorecpp::mapping::EList< ::UMS_Data::AuthAccount >& ListAuthAccounts::getAuthAccounts()
{
    return *m_AuthAccounts;
}

