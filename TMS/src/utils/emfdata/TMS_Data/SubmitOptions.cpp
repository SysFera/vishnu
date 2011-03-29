// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/SubmitOptions.cpp
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

#include "SubmitOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
SubmitOptions::SubmitOptions()
{

    /*PROTECTED REGION ID(SubmitOptionsImpl__SubmitOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

SubmitOptions::~SubmitOptions()
{
}

/*PROTECTED REGION ID(SubmitOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& SubmitOptions::getName() const
{
    return m_name;
}

void SubmitOptions::setName(::ecore::EString const& _name)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_name = m_name;
#endif
    m_name = _name;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__name(),
                _old_name,
                m_name
        );
        eNotify(&notification);
    }
#endif
}

::TMS_Data::JobPriority SubmitOptions::getPriority() const
{
    return m_priority;
}

void SubmitOptions::setPriority(::TMS_Data::JobPriority _priority)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::TMS_Data::JobPriority _old_priority = m_priority;
#endif
    m_priority = _priority;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__priority(),
                _old_priority,
                m_priority
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getQueue() const
{
    return m_queue;
}

void SubmitOptions::setQueue(::ecore::EString const& _queue)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_queue = m_queue;
#endif
    m_queue = _queue;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__queue(),
                _old_queue,
                m_queue
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt SubmitOptions::getWallTime() const
{
    return m_wallTime;
}

void SubmitOptions::setWallTime(::ecore::EInt _wallTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_wallTime = m_wallTime;
#endif
    m_wallTime = _wallTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__wallTime(),
                _old_wallTime,
                m_wallTime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt SubmitOptions::getMemory() const
{
    return m_memory;
}

void SubmitOptions::setMemory(::ecore::EInt _memory)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_memory = m_memory;
#endif
    m_memory = _memory;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__memory(),
                _old_memory,
                m_memory
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt SubmitOptions::getNbCpu() const
{
    return m_nbCpu;
}

void SubmitOptions::setNbCpu(::ecore::EInt _nbCpu)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbCpu = m_nbCpu;
#endif
    m_nbCpu = _nbCpu;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__nbCpu(),
                _old_nbCpu,
                m_nbCpu
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getNbNodesAndCpuPerNode() const
{
    return m_nbNodesAndCpuPerNode;
}

void SubmitOptions::setNbNodesAndCpuPerNode(
        ::ecore::EString const& _nbNodesAndCpuPerNode)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_nbNodesAndCpuPerNode = m_nbNodesAndCpuPerNode;
#endif
    m_nbNodesAndCpuPerNode = _nbNodesAndCpuPerNode;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__nbNodesAndCpuPerNode(),
                _old_nbNodesAndCpuPerNode,
                m_nbNodesAndCpuPerNode
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getOutPutPath() const
{
    return m_outPutPath;
}

void SubmitOptions::setOutPutPath(::ecore::EString const& _outPutPath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_outPutPath = m_outPutPath;
#endif
    m_outPutPath = _outPutPath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__outPutPath(),
                _old_outPutPath,
                m_outPutPath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getErrorPath() const
{
    return m_errorPath;
}

void SubmitOptions::setErrorPath(::ecore::EString const& _errorPath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_errorPath = m_errorPath;
#endif
    m_errorPath = _errorPath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__errorPath(),
                _old_errorPath,
                m_errorPath
        );
        eNotify(&notification);
    }
#endif
}

// References

