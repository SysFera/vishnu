// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/FileStat.cpp
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

#include "FileStat.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "FMS_Data/FMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::FMS_Data;

// Default constructor
FileStat::FileStat() :
    m_perms(-1), m_uid(-1), m_gid(-1), m_size(-1), m_atime(-1), m_mtime(-1),
            m_ctime(-1), m_type(6)
{

    /*PROTECTED REGION ID(FileStatImpl__FileStatImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

FileStat::~FileStat()
{
}

/*PROTECTED REGION ID(FileStat.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& FileStat::getPath() const
{
    return m_path;
}

void FileStat::setPath(::ecore::EString const& _path)
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
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__path(),
                _old_path,
                m_path
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& FileStat::getOwner() const
{
    return m_owner;
}

void FileStat::setOwner(::ecore::EString const& _owner)
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
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__owner(),
                _old_owner,
                m_owner
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& FileStat::getGroup() const
{
    return m_group;
}

void FileStat::setGroup(::ecore::EString const& _group)
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
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__group(),
                _old_group,
                m_group
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt FileStat::getPerms() const
{
    return m_perms;
}

void FileStat::setPerms(::ecore::EInt _perms)
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
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__perms(),
                _old_perms,
                m_perms
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong FileStat::getUid() const
{
    return m_uid;
}

void FileStat::setUid(::ecore::ELong _uid)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_uid = m_uid;
#endif
    m_uid = _uid;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__uid(),
                _old_uid,
                m_uid
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong FileStat::getGid() const
{
    return m_gid;
}

void FileStat::setGid(::ecore::ELong _gid)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_gid = m_gid;
#endif
    m_gid = _gid;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__gid(),
                _old_gid,
                m_gid
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EBigInteger FileStat::getSize() const
{
    return m_size;
}

void FileStat::setSize(::ecore::EBigInteger _size)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EBigInteger _old_size = m_size;
#endif
    m_size = _size;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__size(),
                _old_size,
                m_size
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong FileStat::getAtime() const
{
    return m_atime;
}

void FileStat::setAtime(::ecore::ELong _atime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_atime = m_atime;
#endif
    m_atime = _atime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__atime(),
                _old_atime,
                m_atime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong FileStat::getMtime() const
{
    return m_mtime;
}

void FileStat::setMtime(::ecore::ELong _mtime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_mtime = m_mtime;
#endif
    m_mtime = _mtime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__mtime(),
                _old_mtime,
                m_mtime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong FileStat::getCtime() const
{
    return m_ctime;
}

void FileStat::setCtime(::ecore::ELong _ctime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_ctime = m_ctime;
#endif
    m_ctime = _ctime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__ctime(),
                _old_ctime,
                m_ctime
        );
        eNotify(&notification);
    }
#endif
}

::FMS_Data::FileType FileStat::getType() const
{
    return m_type;
}

void FileStat::setType(::FMS_Data::FileType _type)
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
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getFileStat__type(),
                _old_type,
                m_type
        );
        eNotify(&notification);
    }
#endif
}

// References

