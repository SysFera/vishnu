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

    class Job: public virtual ::ecore::EObject

    {
    public:
        Job();

        virtual ~Job();

        virtual void _initialize();

        // Operations


        // Attributes
        ::ecore::EString const& getSessionId() const;
        void setSessionId(::ecore::EString const& _sessionId);

        ::ecore::EString const& getSubmitMachineId() const;
        void setSubmitMachineId(::ecore::EString const& _submitMachineId);

        ::ecore::EString const& getSubmitMachineName() const;
        void setSubmitMachineName(::ecore::EString const& _submitMachineName);

        ::ecore::EString const& getJobId() const;
        void setJobId(::ecore::EString const& _jobId);

        ::ecore::EString const& getJobName() const;
        void setJobName(::ecore::EString const& _jobName);

        ::ecore::EString const& getJobPath() const;
        void setJobPath(::ecore::EString const& _jobPath);

        ::ecore::EString const& getOutputPath() const;
        void setOutputPath(::ecore::EString const& _outputPath);

        ::ecore::EString const& getErrorPath() const;
        void setErrorPath(::ecore::EString const& _errorPath);

        ::TMS_Data::JobPriority getJobPrio() const;
        void setJobPrio(::TMS_Data::JobPriority _jobPrio);

        ::ecore::EInt getNbCpus() const;
        void setNbCpus(::ecore::EInt _nbCpus);

        ::ecore::EString const& getJobWorkingDir() const;
        void setJobWorkingDir(::ecore::EString const& _jobWorkingDir);

        ::TMS_Data::JobStatus getStatus() const;
        void setStatus(::TMS_Data::JobStatus _status);

        ::ecore::ELong getSubmitDate() const;
        void setSubmitDate(::ecore::ELong _submitDate);

        ::ecore::ELong getEndDate() const;
        void setEndDate(::ecore::ELong _endDate);

        ::ecore::EString const& getOwner() const;
        void setOwner(::ecore::EString const& _owner);

        ::ecore::EString const& getJobQueue() const;
        void setJobQueue(::ecore::EString const& _jobQueue);

        ::ecore::ELong getWallClockLimit() const;
        void setWallClockLimit(::ecore::ELong _wallClockLimit);

        ::ecore::EString const& getGroupName() const;
        void setGroupName(::ecore::EString const& _groupName);

        ::ecore::EString const& getJobDescription() const;
        void setJobDescription(::ecore::EString const& _jobDescription);

        ::ecore::EInt getMemLimit() const;
        void setMemLimit(::ecore::EInt _memLimit);

        ::ecore::EInt getNbNodes() const;
        void setNbNodes(::ecore::EInt _nbNodes);

        ::ecore::EString const& getNbNodesAndCpuPerNode() const;
        void setNbNodesAndCpuPerNode(
                ::ecore::EString const& _nbNodesAndCpuPerNode);

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

        ::ecore::EString m_nbNodesAndCpuPerNode;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_JOB_HPP
