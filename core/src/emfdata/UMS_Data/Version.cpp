// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/Version.cpp
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

#include "Version.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
Version::Version()
{

    /*PROTECTED REGION ID(VersionImpl__VersionImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Version::~Version()
{
}

/*PROTECTED REGION ID(Version.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EInt Version::getMajor() const
{
    return m_major;
}

void Version::setMajor(::ecore::EInt _major)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_major = m_major;
#endif
    m_major = _major;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getVersion__major(),
                _old_major,
                m_major
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Version::getMinor() const
{
    return m_minor;
}

void Version::setMinor(::ecore::EInt _minor)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_minor = m_minor;
#endif
    m_minor = _minor;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getVersion__minor(),
                _old_minor,
                m_minor
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Version::getPatch() const
{
    return m_patch;
}

void Version::setPatch(::ecore::EInt _patch)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_patch = m_patch;
#endif
    m_patch = _patch;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getVersion__patch(),
                _old_patch,
                m_patch
        );
        eNotify(&notification);
    }
#endif
}

// References

