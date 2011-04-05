// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/Job.hpp
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
 * \file Job.hpp
 * \brief The Job class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef TMS_DATA_JOB_HPP
#define TMS_DATA_JOB_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Job_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    /**
     * \class Job
     * \brief Implementation of the Job class
     */
    class Job: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Job
         */
        Job();
        /**
         * \brief The destructor for Job
         */
        virtual ~Job();

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
         * \brief To get the submitMachineId
         * \return The submitMachineId attribute value
         **/
        ::ecore::EString const& getSubmitMachineId() const;
        /**
         * \brief To set the submitMachineId
         * \param _submitMachineId The submitMachineId value
         **/
        void setSubmitMachineId(::ecore::EString const& _submitMachineId);

        /**
         * \brief To get the submitMachineName
         * \return The submitMachineName attribute value
         **/
        ::ecore::EString const& getSubmitMachineName() const;
        /**
         * \brief To set the submitMachineName
         * \param _submitMachineName The submitMachineName value
         **/
        void setSubmitMachineName(::ecore::EString const& _submitMachineName);

        /**
         * \brief To get the jobId
         * \return The jobId attribute value
         **/
        ::ecore::EString const& getJobId() const;
        /**
         * \brief To set the jobId
         * \param _jobId The jobId value
         **/
        void setJobId(::ecore::EString const& _jobId);

        /**
         * \brief To get the jobName
         * \return The jobName attribute value
         **/
        ::ecore::EString const& getJobName() const;
        /**
         * \brief To set the jobName
         * \param _jobName The jobName value
         **/
        void setJobName(::ecore::EString const& _jobName);

        /**
         * \brief To get the jobPath
         * \return The jobPath attribute value
         **/
        ::ecore::EString const& getJobPath() const;
        /**
         * \brief To set the jobPath
         * \param _jobPath The jobPath value
         **/
        void setJobPath(::ecore::EString const& _jobPath);

        /**
         * \brief To get the outputPath
         * \return The outputPath attribute value
         **/
        ::ecore::EString const& getOutputPath() const;
        /**
         * \brief To set the outputPath
         * \param _outputPath The outputPath value
         **/
        void setOutputPath(::ecore::EString const& _outputPath);

        /**
         * \brief To get the errorPath
         * \return The errorPath attribute value
         **/
        ::ecore::EString const& getErrorPath() const;
        /**
         * \brief To set the errorPath
         * \param _errorPath The errorPath value
         **/
        void setErrorPath(::ecore::EString const& _errorPath);

        /**
         * \brief To get the scriptContent
         * \return The scriptContent attribute value
         **/
        ::ecore::EString const& getScriptContent() const;
        /**
         * \brief To set the scriptContent
         * \param _scriptContent The scriptContent value
         **/
        void setScriptContent(::ecore::EString const& _scriptContent);

        /**
         * \brief To get the jobPrio
         * \return The jobPrio attribute value
         **/
        ::TMS_Data::JobPriority getJobPrio() const;
        /**
         * \brief To set the jobPrio
         * \param _jobPrio The jobPrio value
         **/
        void setJobPrio(::TMS_Data::JobPriority _jobPrio);

        /**
         * \brief To get the nbCpus
         * \return The nbCpus attribute value
         **/
        ::ecore::EInt getNbCpus() const;
        /**
         * \brief To set the nbCpus
         * \param _nbCpus The nbCpus value
         **/
        void setNbCpus(::ecore::EInt _nbCpus);

        /**
         * \brief To get the jobWorkingDir
         * \return The jobWorkingDir attribute value
         **/
        ::ecore::EString const& getJobWorkingDir() const;
        /**
         * \brief To set the jobWorkingDir
         * \param _jobWorkingDir The jobWorkingDir value
         **/
        void setJobWorkingDir(::ecore::EString const& _jobWorkingDir);

        /**
         * \brief To get the status
         * \return The status attribute value
         **/
        ::TMS_Data::JobStatus getStatus() const;
        /**
         * \brief To set the status
         * \param _status The status value
         **/
        void setStatus(::TMS_Data::JobStatus _status);

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
         * \brief To get the jobQueue
         * \return The jobQueue attribute value
         **/
        ::ecore::EString const& getJobQueue() const;
        /**
         * \brief To set the jobQueue
         * \param _jobQueue The jobQueue value
         **/
        void setJobQueue(::ecore::EString const& _jobQueue);

        /**
         * \brief To get the wallClockLimit
         * \return The wallClockLimit attribute value
         **/
        ::ecore::ELong getWallClockLimit() const;
        /**
         * \brief To set the wallClockLimit
         * \param _wallClockLimit The wallClockLimit value
         **/
        void setWallClockLimit(::ecore::ELong _wallClockLimit);

        /**
         * \brief To get the groupName
         * \return The groupName attribute value
         **/
        ::ecore::EString const& getGroupName() const;
        /**
         * \brief To set the groupName
         * \param _groupName The groupName value
         **/
        void setGroupName(::ecore::EString const& _groupName);

        /**
         * \brief To get the jobDescription
         * \return The jobDescription attribute value
         **/
        ::ecore::EString const& getJobDescription() const;
        /**
         * \brief To set the jobDescription
         * \param _jobDescription The jobDescription value
         **/
        void setJobDescription(::ecore::EString const& _jobDescription);

        /**
         * \brief To get the memLimit
         * \return The memLimit attribute value
         **/
        ::ecore::EInt getMemLimit() const;
        /**
         * \brief To set the memLimit
         * \param _memLimit The memLimit value
         **/
        void setMemLimit(::ecore::EInt _memLimit);

        /**
         * \brief To get the nbNodes
         * \return The nbNodes attribute value
         **/
        ::ecore::EInt getNbNodes() const;
        /**
         * \brief To set the nbNodes
         * \param _nbNodes The nbNodes value
         **/
        void setNbNodes(::ecore::EInt _nbNodes);

        /**
         * \brief To get the nbNodesAndCpuPerNode
         * \return The nbNodesAndCpuPerNode attribute value
         **/
        ::ecore::EInt getNbNodesAndCpuPerNode() const;
        /**
         * \brief To set the nbNodesAndCpuPerNode
         * \param _nbNodesAndCpuPerNode The nbNodesAndCpuPerNode value
         **/
        void setNbNodesAndCpuPerNode(::ecore::EInt _nbNodesAndCpuPerNode);

        // References


        /*PROTECTED REGION ID(Job) START*/
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

        /*PROTECTED REGION ID(JobImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_sessionId;

        ::ecore::EString m_submitMachineId;

        ::ecore::EString m_submitMachineName;

        ::ecore::EString m_jobId;

        ::ecore::EString m_jobName;

        ::ecore::EString m_jobPath;

        ::ecore::EString m_outputPath;

        ::ecore::EString m_errorPath;

        ::ecore::EString m_scriptContent;

        ::TMS_Data::JobPriority m_jobPrio;

        ::ecore::EInt m_nbCpus;

        ::ecore::EString m_jobWorkingDir;

        ::TMS_Data::JobStatus m_status;

        ::ecore::ELong m_submitDate;

        ::ecore::ELong m_endDate;

        ::ecore::EString m_owner;

        ::ecore::EString m_jobQueue;

        ::ecore::ELong m_wallClockLimit;

        ::ecore::EString m_groupName;

        ::ecore::EString m_jobDescription;

        ::ecore::EInt m_memLimit;

        ::ecore::EInt m_nbNodes;

        ::ecore::EInt m_nbNodesAndCpuPerNode;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_JOB_HPP
