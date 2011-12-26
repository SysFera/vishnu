// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/DirEntry.cpp
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

#include "DirEntry.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "FMS_Data/FMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::FMS_Data;

// Default constructor
DirEntry::DirEntry() :
    m_perms(-1), m_size(-1), m_type(6)
{

    /*PROTECTED REGION ID(DirEntryImpl__DirEntryImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

DirEntry::~DirEntry()
{
}

/*PROTECTED REGION ID(DirEntry.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& DirEntry::getPath() const
{
    return m_path;
}

void DirEntry::setPath(::ecore::EString const& _path)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_path = m_path;
#endif
    m_path = _path;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getDirEntry__path(),
                _old_path,
                m_path
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& DirEntry::getOwner() const
{
    return m_owner;
}

void DirEntry::setOwner(::ecore::EString const& _owner)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_owner = m_owner;
#endif
    m_owner = _owner;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getDirEntry__owner(),
                _old_owner,
                m_owner
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& DirEntry::getGroup() const
{
    return m_group;
}

void DirEntry::setGroup(::ecore::EString const& _group)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_group = m_group;
#endif
    m_group = _group;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getDirEntry__group(),
                _old_group,
                m_group
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt DirEntry::getPerms() const
{
    return m_perms;
}

void DirEntry::setPerms(::ecore::EInt _perms)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_perms = m_perms;
#endif
    m_perms = _perms;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getDirEntry__perms(),
                _old_perms,
                m_perms
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong DirEntry::getSize() const
{
    return m_size;
}

void DirEntry::setSize(::ecore::ELong _size)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_size = m_size;
#endif
    m_size = _size;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getDirEntry__size(),
                _old_size,
                m_size
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& DirEntry::getCreationTime() const
{
    return m_creationTime;
}

void DirEntry::setCreationTime(::ecore::EString const& _creationTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_creationTime = m_creationTime;
#endif
    m_creationTime = _creationTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getDirEntry__creationTime(),
                _old_creationTime,
                m_creationTime
        );
        eNotify(&notification);
    }
#endif
}

::FMS_Data::FileType DirEntry::getType() const
{
    return m_type;
}

void DirEntry::setType(::FMS_Data::FileType _type)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::FMS_Data::FileType _old_type = m_type;
#endif
    m_type = _type;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getDirEntry__type(),
                _old_type,
                m_type
        );
        eNotify(&notification);
    }
#endif
}

// References

