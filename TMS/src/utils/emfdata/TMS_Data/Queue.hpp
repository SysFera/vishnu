// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/Queue.hpp
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

#ifndef TMS_DATA_QUEUE_HPP
#define TMS_DATA_QUEUE_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Queue_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    class Queue: public virtual ::ecore::EObject

    {
    public:
        Queue();

        virtual ~Queue();

        virtual void _initialize();

        // Operations


        // Attributes
        ::ecore::EString const& getName() const;
        void setName(::ecore::EString const& _name);

        ::ecore::EInt getMaxJobCpu() const;
        void setMaxJobCpu(::ecore::EInt _maxJobCpu);

        ::ecore::EInt getMaxProcCpu() const;
        void setMaxProcCpu(::ecore::EInt _maxProcCpu);

        ::ecore::EInt getMemory() const;
        void setMemory(::ecore::EInt _memory);

        ::ecore::ELong getWallTime() const;
        void setWallTime(::ecore::ELong _wallTime);

        ::ecore::EInt getNode() const;
        void setNode(::ecore::EInt _node);

        ::ecore::EInt getNbRunningJobs() const;
        void setNbRunningJobs(::ecore::EInt _nbRunningJobs);

        ::ecore::EInt getNbJobsInQueue() const;
        void setNbJobsInQueue(::ecore::EInt _nbJobsInQueue);

        ::TMS_Data::QueueStatus getState() const;
        void setState(::TMS_Data::QueueStatus _state);

        ::TMS_Data::QueuePriority getPriority() const;
        void setPriority(::TMS_Data::QueuePriority _priority);

        ::ecore::EString const& getDescription() const;
        void setDescription(::ecore::EString const& _description);

        // References


        /*PROTECTED REGION ID(Queue) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

        // EObjectImpl
        virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID,
                ::ecore::EBoolean _resolve);
        virtual void eSet(::ecore::EInt _featureID,
                ::ecore::EJavaObject const& _newValue);
        virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID);
        virtual void eUnset(::ecore::EInt _featureID);
        virtual ::ecore::EClass_ptr _eClass();

        /*PROTECTED REGION ID(QueueImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_name;

        ::ecore::EInt m_maxJobCpu;

        ::ecore::EInt m_maxProcCpu;

        ::ecore::EInt m_memory;

        ::ecore::ELong m_wallTime;

        ::ecore::EInt m_node;

        ::ecore::EInt m_nbRunningJobs;

        ::ecore::EInt m_nbJobsInQueue;

        ::TMS_Data::QueueStatus m_state;

        ::TMS_Data::QueuePriority m_priority;

        ::ecore::EString m_description;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_QUEUE_HPP
