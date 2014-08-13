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
         * \brief To get the machine
         * \return The machine attribute value
         **/
        ::ecore::EString const& getMachine() const;
        /**
         * \brief To set the machine
         * \param _machine The machine value
         **/
        void setMachine(::ecore::EString const& _machine);

        /**
         * \brief To get the id
         * \return The id attribute value
         **/
        ::ecore::EString const& getId() const;
        /**
         * \brief To set the id
         * \param _id The id value
         **/
        void setId(::ecore::EString const& _id);

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
         * \brief To get the path
         * \return The path attribute value
         **/
        ::ecore::EString const& getPath() const;
        /**
         * \brief To set the path
         * \param _path The path value
         **/
        void setPath(::ecore::EString const& _path);

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
         * \brief To get the workingDir
         * \return The workingDir attribute value
         **/
        ::ecore::EString const& getWorkingDir() const;
        /**
         * \brief To set the workingDir
         * \param _workingDir The workingDir value
         **/
        void setWorkingDir(::ecore::EString const& _workingDir);

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
         * \brief To get the localAccount
         * \return The localAccount attribute value
         **/
        ::ecore::EString const& getLocalAccount() const;
        /**
         * \brief To set the localAccount
         * \param _localAccount The localAccount value
         **/
        void setLocalAccount(::ecore::EString const& _localAccount);

        /**
         * \brief To get the queue
         * \return The queue attribute value
         **/
        ::ecore::EString const& getQueue() const;
        /**
         * \brief To set the queue
         * \param _queue The queue value
         **/
        void setQueue(::ecore::EString const& _queue);

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
        ::ecore::EString const& getNbNodesAndCpuPerNode() const;
        /**
         * \brief To set the nbNodesAndCpuPerNode
         * \param _nbNodesAndCpuPerNode The nbNodesAndCpuPerNode value
         **/
        void setNbNodesAndCpuPerNode(
                ::ecore::EString const& _nbNodesAndCpuPerNode);

        /**
         * \brief To get the batchJobId
         * \return The batchJobId attribute value
         **/
        ::ecore::EString const& getBatchJobId() const;
        /**
         * \brief To set the batchJobId
         * \param _batchJobId The batchJobId value
         **/
        void setBatchJobId(::ecore::EString const& _batchJobId);

        /**
         * \brief To get the outputDir
         * \return The outputDir attribute value
         **/
        ::ecore::EString const& getOutputDir() const;
        /**
         * \brief To set the outputDir
         * \param _outputDir The outputDir value
         **/
        void setOutputDir(::ecore::EString const& _outputDir);

        /**
         * \brief To get the workId
         * \return The workId attribute value
         **/
        ::ecore::ELong getWorkId() const;
        /**
         * \brief To set the workId
         * \param _workId The workId value
         **/
        void setWorkId(::ecore::ELong _workId);

        /**
         * \brief To get the userId
         * \return The userId attribute value
         **/
        ::ecore::EString const& getUserId() const;
        /**
         * \brief To set the userId
         * \param _userId The userId value
         **/
        void setUserId(::ecore::EString const& _userId);

        /**
         * \brief To get the vmId
         * \return The vmId attribute value
         **/
        ::ecore::EString const& getVmId() const;
        /**
         * \brief To set the vmId
         * \param _vmId The vmId value
         **/
        void setVmId(::ecore::EString const& _vmId);

        /**
         * \brief To get the vmIp
         * \return The vmIp attribute value
         **/
        ::ecore::EString const& getVmIp() const;
        /**
         * \brief To set the vmIp
         * \param _vmIp The vmIp value
         **/
        void setVmIp(::ecore::EString const& _vmIp);

        /**
         * \brief To get the relatedSteps
         * \return The relatedSteps attribute value
         **/
        ::ecore::EString const& getRelatedSteps() const;
        /**
         * \brief To set the relatedSteps
         * \param _relatedSteps The relatedSteps value
         **/
        void setRelatedSteps(::ecore::EString const& _relatedSteps);

        /**
         * \brief To get the submitError
         * \return The submitError attribute value
         **/
        ::ecore::EString const& getSubmitError() const;
        /**
         * \brief To set the submitError
         * \param _submitError The submitError value
         **/
        void setSubmitError(::ecore::EString const& _submitError);

        /**
         * \brief To get the priority
         * \return The priority attribute value
         **/
        ::TMS_Data::JobStatus getPriority() const;
        /**
         * \brief To set the priority
         * \param _priority The priority value
         **/
        void setPriority(::TMS_Data::JobStatus _priority);

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

        ::ecore::EString m_machine;

        ::ecore::EString m_id;

        ::ecore::EString m_name;

        ::ecore::EString m_path;

        ::ecore::EString m_outputPath;

        ::ecore::EString m_errorPath;

        ::ecore::EInt m_nbCpus;

        ::ecore::EString m_workingDir;

        ::TMS_Data::JobStatus m_status;

        ::ecore::ELong m_submitDate;

        ::ecore::ELong m_endDate;

        ::ecore::EString m_localAccount;

        ::ecore::EString m_queue;

        ::ecore::ELong m_wallClockLimit;

        ::ecore::EString m_groupName;

        ::ecore::EString m_description;

        ::ecore::EInt m_memLimit;

        ::ecore::EInt m_nbNodes;

        ::ecore::EString m_nbNodesAndCpuPerNode;

        ::ecore::EString m_batchJobId;

        ::ecore::EString m_outputDir;

        ::ecore::ELong m_workId;

        ::ecore::EString m_userId;

        ::ecore::EString m_vmId;

        ::ecore::EString m_vmIp;

        ::ecore::EString m_relatedSteps;

        ::ecore::EString m_submitError;

        ::TMS_Data::JobStatus m_priority;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_JOB_HPP
