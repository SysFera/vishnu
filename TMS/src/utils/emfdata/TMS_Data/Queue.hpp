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

/**
 * \file Queue.hpp
 * \brief The Queue class
 * \author Generated file
 * \date 31/03/2011
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

    /**
     * \class Queue
     * \brief Implementation of the Queue class
     */
    class Queue: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Queue
         */
        Queue();
        /**
         * \brief The destructor for Queue
         */
        virtual ~Queue();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the name
         * \return The name attribute value
         **/
        ::ecore::EString const& getName() const;
        /**
         * \brief To set the name
         * \param _name The name value
         **/
        void setName(::ecore::EString const& _name);

        /**
         * \brief To get the maxJobCpu
         * \return The maxJobCpu attribute value
         **/
        ::ecore::EInt getMaxJobCpu() const;
        /**
         * \brief To set the maxJobCpu
         * \param _maxJobCpu The maxJobCpu value
         **/
        void setMaxJobCpu(::ecore::EInt _maxJobCpu);

        /**
         * \brief To get the maxProcCpu
         * \return The maxProcCpu attribute value
         **/
        ::ecore::EInt getMaxProcCpu() const;
        /**
         * \brief To set the maxProcCpu
         * \param _maxProcCpu The maxProcCpu value
         **/
        void setMaxProcCpu(::ecore::EInt _maxProcCpu);

        /**
         * \brief To get the memory
         * \return The memory attribute value
         **/
        ::ecore::EInt getMemory() const;
        /**
         * \brief To set the memory
         * \param _memory The memory value
         **/
        void setMemory(::ecore::EInt _memory);

        /**
         * \brief To get the wallTime
         * \return The wallTime attribute value
         **/
        ::ecore::ELong getWallTime() const;
        /**
         * \brief To set the wallTime
         * \param _wallTime The wallTime value
         **/
        void setWallTime(::ecore::ELong _wallTime);

        /**
         * \brief To get the node
         * \return The node attribute value
         **/
        ::ecore::EInt getNode() const;
        /**
         * \brief To set the node
         * \param _node The node value
         **/
        void setNode(::ecore::EInt _node);

        /**
         * \brief To get the nbRunningJobs
         * \return The nbRunningJobs attribute value
         **/
        ::ecore::EInt getNbRunningJobs() const;
        /**
         * \brief To set the nbRunningJobs
         * \param _nbRunningJobs The nbRunningJobs value
         **/
        void setNbRunningJobs(::ecore::EInt _nbRunningJobs);

        /**
         * \brief To get the nbJobsInQueue
         * \return The nbJobsInQueue attribute value
         **/
        ::ecore::EInt getNbJobsInQueue() const;
        /**
         * \brief To set the nbJobsInQueue
         * \param _nbJobsInQueue The nbJobsInQueue value
         **/
        void setNbJobsInQueue(::ecore::EInt _nbJobsInQueue);

        /**
         * \brief To get the state
         * \return The state attribute value
         **/
        ::TMS_Data::QueueStatus getState() const;
        /**
         * \brief To set the state
         * \param _state The state value
         **/
        void setState(::TMS_Data::QueueStatus _state);

        /**
         * \brief To get the priority
         * \return The priority attribute value
         **/
        ::TMS_Data::QueuePriority getPriority() const;
        /**
         * \brief To set the priority
         * \param _priority The priority value
         **/
        void setPriority(::TMS_Data::QueuePriority _priority);

        /**
         * \brief To get the description
         * \return The description attribute value
         **/
        ::ecore::EString const& getDescription() const;
        /**
         * \brief To set the description
         * \param _description The description value
         **/
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
