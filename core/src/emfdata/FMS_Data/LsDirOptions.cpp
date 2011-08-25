// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/LsDirOptions.cpp
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

#include "LsDirOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "FMS_Data/FMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::FMS_Data;

// Default constructor
LsDirOptions::LsDirOptions() :
    m_longFormat(false), m_allFiles(false)
{

    /*PROTECTED REGION ID(LsDirOptionsImpl__LsDirOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

LsDirOptions::~LsDirOptions()
{
}

/*PROTECTED REGION ID(LsDirOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EBoolean LsDirOptions::isLongFormat() const
{
    return m_longFormat;
}

void LsDirOptions::setLongFormat(::ecore::EBoolean _longFormat)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EBoolean _old_longFormat = m_longFormat;
#endif
    m_longFormat = _longFormat;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getLsDirOptions__longFormat(),
                _old_longFormat,
                m_longFormat
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EBoolean LsDirOptions::isAllFiles() const
{
    return m_allFiles;
}

void LsDirOptions::setAllFiles(::ecore::EBoolean _allFiles)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EBoolean _old_allFiles = m_allFiles;
#endif
    m_allFiles = _allFiles;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getLsDirOptions__allFiles(),
                _old_allFiles,
                m_allFiles
        );
        eNotify(&notification);
    }
#endif
}

// References

