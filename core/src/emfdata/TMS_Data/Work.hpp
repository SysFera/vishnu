// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/Work.hpp
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
 * \file Work.hpp
 * \brief The Work class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef TMS_DATA_WORK_HPP
#define TMS_DATA_WORK_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Work_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    /**
     * \class Work
     * \brief Implementation of the Work class
     */
    class Work: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Work
         */
        Work();
        /**
         * \brief The destructor for Work
         */
        virtual ~Work();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the sessionId
         * \return The sessionId attribute value
         **/
        ::ecore::EString const& getSessionId() const;
        /**
         * \brief To set the sessionId
         * \param _sessionId The sessionId value
         **/
        void setSessionId(::ecore::EString const& _sessionId);

        /**
         * \brief To get the applicationId
         * \return The applicationId attribute value
         **/
        ::ecore::EString const& getApplicationId() const;
        /**
         * \brief To set the applicationId
         * \param _applicationId The applicationId value
         **/
        void setApplicationId(::ecore::EString const& _applicationId);

        /**
         * \brief To get the subject
         * \return The subject attribute value
         **/
        ::ecore::EString const& getSubject() const;
        /**
         * \brief To set the subject
         * \param _subject The subject value
         **/
        void setSubject(::ecore::EString const& _subject);

        /**
         * \brief To get the priority
         * \return The priority attribute value
         **/
        ::ecore::EInt getPriority() const;
        /**
         * \brief To set the priority
         * \param _priority The priority value
         **/
        void setPriority(::ecore::EInt _priority);

        /**
         * \brief To get the status
         * \return The status attribute value
         **/
        ::ecore::EInt getStatus() const;
        /**
         * \brief To set the status
         * \param _status The status value
         **/
        void setStatus(::ecore::EInt _status);

        /**
         * \brief To get the endDate
         * \return The endDate attribute value
         **/
        ::ecore::ELong getEndDate() const;
        /**
         * \brief To set the endDate
         * \param _endDate The endDate value
         **/
        void setEndDate(::ecore::ELong _endDate);

        /**
         * \brief To get the owner
         * \return The owner attribute value
         **/
        ::ecore::EString const& getOwner() const;
        /**
         * \brief To set the owner
         * \param _owner The owner value
         **/
        void setOwner(::ecore::EString const& _owner);

        /**
         * \brief To get the estimatedHour
         * \return The estimatedHour attribute value
         **/
        ::ecore::ELong getEstimatedHour() const;
        /**
         * \brief To set the estimatedHour
         * \param _estimatedHour The estimatedHour value
         **/
        void setEstimatedHour(::ecore::ELong _estimatedHour);

        /**
         * \brief To get the doneRatio
         * \return The doneRatio attribute value
         **/
        ::ecore::EInt getDoneRatio() const;
        /**
         * \brief To set the doneRatio
         * \param _doneRatio The doneRatio value
         **/
        void setDoneRatio(::ecore::EInt _doneRatio);

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

        /**
         * \brief To get the dateCreated
         * \return The dateCreated attribute value
         **/
        ::ecore::ELong getDateCreated() const;
        /**
         * \brief To set the dateCreated
         * \param _dateCreated The dateCreated value
         **/
        void setDateCreated(::ecore::ELong _dateCreated);

        /**
         * \brief To get the dateEnded
         * \return The dateEnded attribute value
         **/
        ::ecore::ELong getDateEnded() const;
        /**
         * \brief To set the dateEnded
         * \param _dateEnded The dateEnded value
         **/
        void setDateEnded(::ecore::ELong _dateEnded);

        /**
         * \brief To get the dateStarted
         * \return The dateStarted attribute value
         **/
        ::ecore::ELong getDateStarted() const;
        /**
         * \brief To set the dateStarted
         * \param _dateStarted The dateStarted value
         **/
        void setDateStarted(::ecore::ELong _dateStarted);

        /**
         * \brief To get the lastUpdated
         * \return The lastUpdated attribute value
         **/
        ::ecore::ELong getLastUpdated() const;
        /**
         * \brief To set the lastUpdated
         * \param _lastUpdated The lastUpdated value
         **/
        void setLastUpdated(::ecore::ELong _lastUpdated);

        /**
         * \brief To get the workId
         * \return The workId attribute value
         **/
        ::ecore::EString const& getWorkId() const;
        /**
         * \brief To set the workId
         * \param _workId The workId value
         **/
        void setWorkId(::ecore::EString const& _workId);

        /**
         * \brief To get the projectId
         * \return The projectId attribute value
         **/
        ::ecore::EString const& getProjectId() const;
        /**
         * \brief To set the projectId
         * \param _projectId The projectId value
         **/
        void setProjectId(::ecore::EString const& _projectId);

        /**
         * \brief To get the submitDate
         * \return The submitDate attribute value
         **/
        ::ecore::ELong getSubmitDate() const;
        /**
         * \brief To set the submitDate
         * \param _submitDate The submitDate value
         **/
        void setSubmitDate(::ecore::ELong _submitDate);

        /**
         * \brief To get the startDate
         * \return The startDate attribute value
         **/
        ::ecore::ELong getStartDate() const;
        /**
         * \brief To set the startDate
         * \param _startDate The startDate value
         **/
        void setStartDate(::ecore::ELong _startDate);

        /**
         * \brief To get the machineId
         * \return The machineId attribute value
         **/
        ::ecore::EString const& getMachineId() const;
        /**
         * \brief To set the machineId
         * \param _machineId The machineId value
         **/
        void setMachineId(::ecore::EString const& _machineId);

        /**
         * \brief To get the nbCPU
         * \return The nbCPU attribute value
         **/
        ::ecore::EInt getNbCPU() const;
        /**
         * \brief To set the nbCPU
         * \param _nbCPU The nbCPU value
         **/
        void setNbCPU(::ecore::EInt _nbCPU);

        /**
         * \brief To get the dueDate
         * \return The dueDate attribute value
         **/
        ::ecore::ELong getDueDate() const;
        /**
         * \brief To set the dueDate
         * \param _dueDate The dueDate value
         **/
        void setDueDate(::ecore::ELong _dueDate);

        // References


        /*PROTECTED REGION ID(Work) START*/
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

        /*PROTECTED REGION ID(WorkImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_sessionId;

        ::ecore::EString m_applicationId;

        ::ecore::EString m_subject;

        ::ecore::EInt m_priority;

        ::ecore::EInt m_status;

        ::ecore::ELong m_endDate;

        ::ecore::EString m_owner;

        ::ecore::ELong m_estimatedHour;

        ::ecore::EInt m_doneRatio;

        ::ecore::EString m_description;

        ::ecore::ELong m_dateCreated;

        ::ecore::ELong m_dateEnded;

        ::ecore::ELong m_dateStarted;

        ::ecore::ELong m_lastUpdated;

        ::ecore::EString m_workId;

        ::ecore::EString m_projectId;

        ::ecore::ELong m_submitDate;

        ::ecore::ELong m_startDate;

        ::ecore::EString m_machineId;

        ::ecore::EInt m_nbCPU;

        ::ecore::ELong m_dueDate;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_WORK_HPP
