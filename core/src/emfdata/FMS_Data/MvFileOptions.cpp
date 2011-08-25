// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/MvFileOptions.cpp
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

#include "MvFileOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "FMS_Data/FMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::FMS_Data;

// Default constructor
MvFileOptions::MvFileOptions() :
    m_trCommand(2)
{

    /*PROTECTED REGION ID(MvFileOptionsImpl__MvFileOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

MvFileOptions::~MvFileOptions()
{
}

/*PROTECTED REGION ID(MvFileOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::FMS_Data::TransferCommand MvFileOptions::getTrCommand() const
{
    return m_trCommand;
}

void MvFileOptions::setTrCommand(::FMS_Data::TransferCommand _trCommand)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::FMS_Data::TransferCommand _old_trCommand = m_trCommand;
#endif
    m_trCommand = _trCommand;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getMvFileOptions__trCommand(),
                _old_trCommand,
                m_trCommand
        );
        eNotify(&notification);
    }
#endif
}

// References

