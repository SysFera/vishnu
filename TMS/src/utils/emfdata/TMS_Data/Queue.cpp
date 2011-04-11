// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/Queue.cpp
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

#include "Queue.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
Queue::Queue()
{

    /*PROTECTED REGION ID(QueueImpl__QueueImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Queue::~Queue()
{
}

/*PROTECTED REGION ID(Queue.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& Queue::getName() const
{
    return m_name;
}

void Queue::setName(::ecore::EString const& _name)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__name(),
                _old_name,
                m_name
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Queue::getMaxJobCpu() const
{
    return m_maxJobCpu;
}

void Queue::setMaxJobCpu(::ecore::EInt _maxJobCpu)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_maxJobCpu = m_maxJobCpu;
#endif
    m_maxJobCpu = _maxJobCpu;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__maxJobCpu(),
                _old_maxJobCpu,
                m_maxJobCpu
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Queue::getMaxProcCpu() const
{
    return m_maxProcCpu;
}

void Queue::setMaxProcCpu(::ecore::EInt _maxProcCpu)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_maxProcCpu = m_maxProcCpu;
#endif
    m_maxProcCpu = _maxProcCpu;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__maxProcCpu(),
                _old_maxProcCpu,
                m_maxProcCpu
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Queue::getMemory() const
{
    return m_memory;
}

void Queue::setMemory(::ecore::EInt _memory)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__memory(),
                _old_memory,
                m_memory
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Queue::getWallTime() const
{
    return m_wallTime;
}

void Queue::setWallTime(::ecore::ELong _wallTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_wallTime = m_wallTime;
#endif
    m_wallTime = _wallTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__wallTime(),
                _old_wallTime,
                m_wallTime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Queue::getNode() const
{
    return m_node;
}

void Queue::setNode(::ecore::EInt _node)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_node = m_node;
#endif
    m_node = _node;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__node(),
                _old_node,
                m_node
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Queue::getNbRunningJobs() const
{
    return m_nbRunningJobs;
}

void Queue::setNbRunningJobs(::ecore::EInt _nbRunningJobs)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbRunningJobs = m_nbRunningJobs;
#endif
    m_nbRunningJobs = _nbRunningJobs;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__nbRunningJobs(),
                _old_nbRunningJobs,
                m_nbRunningJobs
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt Queue::getNbJobsInQueue() const
{
    return m_nbJobsInQueue;
}

void Queue::setNbJobsInQueue(::ecore::EInt _nbJobsInQueue)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbJobsInQueue = m_nbJobsInQueue;
#endif
    m_nbJobsInQueue = _nbJobsInQueue;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__nbJobsInQueue(),
                _old_nbJobsInQueue,
                m_nbJobsInQueue
        );
        eNotify(&notification);
    }
#endif
}

::TMS_Data::QueueStatus Queue::getState() const
{
    return m_state;
}

void Queue::setState(::TMS_Data::QueueStatus _state)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::TMS_Data::QueueStatus _old_state = m_state;
#endif
    m_state = _state;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__state(),
                _old_state,
                m_state
        );
        eNotify(&notification);
    }
#endif
}

::TMS_Data::QueuePriority Queue::getPriority() const
{
    return m_priority;
}

void Queue::setPriority(::TMS_Data::QueuePriority _priority)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::TMS_Data::QueuePriority _old_priority = m_priority;
#endif
    m_priority = _priority;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__priority(),
                _old_priority,
                m_priority
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Queue::getDescription() const
{
    return m_description;
}

void Queue::setDescription(::ecore::EString const& _description)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_description = m_description;
#endif
    m_description = _description;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getQueue__description(),
                _old_description,
                m_description
        );
        eNotify(&notification);
    }
#endif
}

// References

