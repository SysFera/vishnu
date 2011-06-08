// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/CpFileOptions.cpp
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

#include "CpFileOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "FMS_Data/FMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::FMS_Data;

// Default constructor
CpFileOptions::CpFileOptions() :
    m_isRecursive(false), m_trCommand(2)
{

    /*PROTECTED REGION ID(CpFileOptionsImpl__CpFileOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

CpFileOptions::~CpFileOptions()
{
}

/*PROTECTED REGION ID(CpFileOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EBoolean CpFileOptions::isIsRecursive() const
{
    return m_isRecursive;
}

void CpFileOptions::setIsRecursive(::ecore::EBoolean _isRecursive)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EBoolean _old_isRecursive = m_isRecursive;
#endif
    m_isRecursive = _isRecursive;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getCpFileOptions__isRecursive(),
                _old_isRecursive,
                m_isRecursive
        );
        eNotify(&notification);
    }
#endif
}

::FMS_Data::TransferCommand CpFileOptions::getTrCommand() const
{
    return m_trCommand;
}

void CpFileOptions::setTrCommand(::FMS_Data::TransferCommand _trCommand)
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
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getCpFileOptions__trCommand(),
                _old_trCommand,
                m_trCommand
        );
        eNotify(&notification);
    }
#endif
}

// References

