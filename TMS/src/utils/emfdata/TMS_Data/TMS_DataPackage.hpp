// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/TMS_DataPackage.hpp
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

#ifndef _TMS_DATAPACKAGE_HPP
#define _TMS_DATAPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <TMS_Data_forward.hpp>

namespace TMS_Data
{

    class TMS_DataPackage: public virtual ::ecore::EPackage
    {
    public:

        static TMS_DataPackage_ptr _instance();

        // IDs for classifiers

        static const int JOB = 0;

        static const int JOBPRIORITY = 1;

        static const int JOBRESULT = 2;

        static const int JOBSTATUS = 3;

        static const int LISTJOBRESULTS = 4;

        static const int LISTJOBS = 5;

        static const int LISTJOBSOPTIONS = 6;

        static const int LISTPROGRESSION = 7;

        static const int LISTQUEUES = 8;

        static const int PROGRESSION = 9;

        static const int PROGRESSOPTIONS = 10;

        static const int QUEUE = 11;

        static const int QUEUEPRIORITY = 12;

        static const int QUEUESTATUS = 13;

        static const int SUBMITOPTIONS = 14;

        static const int JOB__SESSIONID = 0;

        static const int JOB__SUBMITMACHINEID = 1;

        static const int JOB__SUBMITMACHINENAME = 2;

        static const int JOB__JOBID = 3;

        static const int JOB__JOBNAME = 4;

        static const int JOB__JOBPATH = 5;

        static const int JOB__OUTPUTPATH = 6;

        static const int JOB__ERRORPATH = 7;

        static const int JOB__JOBPRIO = 8;

        static const int JOB__NBCPUS = 9;

        static const int JOB__JOBWORKINGDIR = 10;

        static const int JOB__STATUS = 11;

        static const int JOB__SUBMITDATE = 12;

        static const int JOB__ENDDATE = 13;

        static const int JOB__OWNER = 14;

        static const int JOB__JOBQUEUE = 15;

        static const int JOB__WALLCLOCKLIMIT = 16;

        static const int JOB__GROUPNAME = 17;

        static const int JOB__JOBDESCRIPTION = 18;

        static const int JOB__MEMLIMIT = 19;

        static const int JOB__NBNODES = 20;

        static const int JOB__NBNODESANDCPUPERNODE = 21;

        static const int LISTJOBS__NBJOBS = 22;

        static const int LISTJOBS__NBRUNNINGJOBS = 23;

        static const int LISTJOBS__NBWAITINGJOBS = 24;

        static const int LISTJOBS__JOBS = 25;

        static const int SUBMITOPTIONS__NAME = 26;

        static const int SUBMITOPTIONS__PRIORITY = 27;

        static const int SUBMITOPTIONS__QUEUE = 28;

        static const int SUBMITOPTIONS__WALLTIME = 29;

        static const int SUBMITOPTIONS__MEMORY = 30;

        static const int SUBMITOPTIONS__NBCPU = 31;

        static const int SUBMITOPTIONS__NBNODESANDCPUPERNODE = 32;

        static const int SUBMITOPTIONS__OUTPUTPATH = 33;

        static const int SUBMITOPTIONS__ERRORPATH = 34;

        static const int LISTJOBSOPTIONS__JOBID = 35;

        static const int LISTJOBSOPTIONS__NBCPU = 36;

        static const int LISTJOBSOPTIONS__FROMSUBMITDATE = 37;

        static const int LISTJOBSOPTIONS__TOSUBMITDATE = 38;

        static const int LISTJOBSOPTIONS__OWNER = 39;

        static const int LISTJOBSOPTIONS__STATUS = 40;

        static const int LISTJOBSOPTIONS__PRIORITY = 41;

        static const int LISTJOBSOPTIONS__OUTPUTPATH = 42;

        static const int LISTJOBSOPTIONS__ERRORPATH = 43;

        static const int LISTJOBSOPTIONS__QUEUE = 44;

        static const int PROGRESSOPTIONS__JOBID = 45;

        static const int PROGRESSOPTIONS__JOBOWNER = 46;

        static const int LISTPROGRESSION__NBJOBS = 47;

        static const int LISTPROGRESSION__PROGRESS = 48;

        static const int PROGRESSION__JOBID = 49;

        static const int PROGRESSION__JOBNAME = 50;

        static const int PROGRESSION__WALLTIME = 51;

        static const int PROGRESSION__STARTTIME = 52;

        static const int PROGRESSION__ENDTIME = 53;

        static const int PROGRESSION__PERCENT = 54;

        static const int PROGRESSION__STATUS = 55;

        static const int LISTQUEUES__NBQUEUES = 56;

        static const int LISTQUEUES__QUEUES = 57;

        static const int QUEUE__NAME = 58;

        static const int QUEUE__MAXJOBCPU = 59;

        static const int QUEUE__MAXPROCCPU = 60;

        static const int QUEUE__MEMORY = 61;

        static const int QUEUE__WALLTIME = 62;

        static const int QUEUE__NODE = 63;

        static const int QUEUE__NBRUNNINGJOBS = 64;

        static const int QUEUE__NBJOBSINQUEUE = 65;

        static const int QUEUE__STATE = 66;

        static const int QUEUE__PRIORITY = 67;

        static const int QUEUE__DESCRIPTION = 68;

        static const int JOBRESULT__JOBID = 69;

        static const int JOBRESULT__OUTPUTPATH = 70;

        static const int JOBRESULT__ERRORPATH = 71;

        static const int LISTJOBRESULTS__NBJOBS = 72;

        static const int LISTJOBRESULTS__RESULTS = 73;

        // IDs for classifiers for class Job 


        // IDs for classifiers for class JobResult 


        // IDs for classifiers for class ListJobResults 


        // IDs for classifiers for class ListJobs 


        // IDs for classifiers for class ListJobsOptions 


        // IDs for classifiers for class ListProgression 


        // IDs for classifiers for class ListQueues 


        // IDs for classifiers for class Progression 


        // IDs for classifiers for class ProgressOptions 


        // IDs for classifiers for class Queue 


        // IDs for classifiers for class SubmitOptions 


        // EClassifiers methods

        virtual ::ecore::EClass_ptr getJob();

        virtual ::ecore::EClass_ptr getListJobs();

        virtual ::ecore::EClass_ptr getSubmitOptions();

        virtual ::ecore::EClass_ptr getListJobsOptions();

        virtual ::ecore::EEnum_ptr getJobPriority();

        virtual ::ecore::EEnum_ptr getJobStatus();

        virtual ::ecore::EClass_ptr getProgressOptions();

        virtual ::ecore::EClass_ptr getListProgression();

        virtual ::ecore::EClass_ptr getProgression();

        virtual ::ecore::EClass_ptr getListQueues();

        virtual ::ecore::EClass_ptr getQueue();

        virtual ::ecore::EEnum_ptr getQueuePriority();

        virtual ::ecore::EEnum_ptr getQueueStatus();

        virtual ::ecore::EClass_ptr getJobResult();

        virtual ::ecore::EClass_ptr getListJobResults();

        // EStructuralFeatures methods

        virtual ::ecore::EAttribute_ptr getJob__sessionId();

        virtual ::ecore::EAttribute_ptr getJob__submitMachineId();

        virtual ::ecore::EAttribute_ptr getJob__submitMachineName();

        virtual ::ecore::EAttribute_ptr getJob__jobId();

        virtual ::ecore::EAttribute_ptr getJob__jobName();

        virtual ::ecore::EAttribute_ptr getJob__jobPath();

        virtual ::ecore::EAttribute_ptr getJob__outputPath();

        virtual ::ecore::EAttribute_ptr getJob__errorPath();

        virtual ::ecore::EAttribute_ptr getJob__jobPrio();

        virtual ::ecore::EAttribute_ptr getJob__nbCpus();

        virtual ::ecore::EAttribute_ptr getJob__jobWorkingDir();

        virtual ::ecore::EAttribute_ptr getJob__status();

        virtual ::ecore::EAttribute_ptr getJob__submitDate();

        virtual ::ecore::EAttribute_ptr getJob__endDate();

        virtual ::ecore::EAttribute_ptr getJob__owner();

        virtual ::ecore::EAttribute_ptr getJob__jobQueue();

        virtual ::ecore::EAttribute_ptr getJob__wallClockLimit();

        virtual ::ecore::EAttribute_ptr getJob__groupName();

        virtual ::ecore::EAttribute_ptr getJob__jobDescription();

        virtual ::ecore::EAttribute_ptr getJob__memLimit();

        virtual ::ecore::EAttribute_ptr getJob__nbNodes();

        virtual ::ecore::EAttribute_ptr getJob__nbNodesAndCpuPerNode();

        virtual ::ecore::EAttribute_ptr getListJobs__nbJobs();

        virtual ::ecore::EAttribute_ptr getListJobs__nbRunningJobs();

        virtual ::ecore::EAttribute_ptr getListJobs__nbWaitingJobs();

        virtual ::ecore::EReference_ptr getListJobs__jobs();

        virtual ::ecore::EAttribute_ptr getSubmitOptions__name();

        virtual ::ecore::EAttribute_ptr getSubmitOptions__priority();

        virtual ::ecore::EAttribute_ptr getSubmitOptions__queue();

        virtual ::ecore::EAttribute_ptr getSubmitOptions__wallTime();

        virtual ::ecore::EAttribute_ptr getSubmitOptions__memory();

        virtual ::ecore::EAttribute_ptr getSubmitOptions__nbCpu();

        virtual ::ecore::EAttribute_ptr
                getSubmitOptions__nbNodesAndCpuPerNode();

        virtual ::ecore::EAttribute_ptr getSubmitOptions__outPutPath();

        virtual ::ecore::EAttribute_ptr getSubmitOptions__errorPath();

        virtual ::ecore::EAttribute_ptr getListJobsOptions__jobId();

        virtual ::ecore::EAttribute_ptr getListJobsOptions__nbCpu();

        virtual ::ecore::EAttribute_ptr getListJobsOptions__fromSubmitDate();

        virtual ::ecore::EAttribute_ptr getListJobsOptions__toSubmitDate();

        virtual ::ecore::EAttribute_ptr getListJobsOptions__owner();

        virtual ::ecore::EAttribute_ptr getListJobsOptions__status();

        virtual ::ecore::EAttribute_ptr getListJobsOptions__priority();

        virtual ::ecore::EAttribute_ptr getListJobsOptions__outPutPath();

        virtual ::ecore::EAttribute_ptr getListJobsOptions__errorPath();

        virtual ::ecore::EAttribute_ptr getListJobsOptions__queue();

        virtual ::ecore::EAttribute_ptr getProgressOptions__jobId();

        virtual ::ecore::EAttribute_ptr getProgressOptions__jobOwner();

        virtual ::ecore::EAttribute_ptr getListProgression__nbJobs();

        virtual ::ecore::EReference_ptr getListProgression__progress();

        virtual ::ecore::EAttribute_ptr getProgression__jobId();

        virtual ::ecore::EAttribute_ptr getProgression__jobName();

        virtual ::ecore::EAttribute_ptr getProgression__wallTime();

        virtual ::ecore::EAttribute_ptr getProgression__startTime();

        virtual ::ecore::EAttribute_ptr getProgression__endTime();

        virtual ::ecore::EAttribute_ptr getProgression__percent();

        virtual ::ecore::EAttribute_ptr getProgression__status();

        virtual ::ecore::EAttribute_ptr getListQueues__nbQueues();

        virtual ::ecore::EReference_ptr getListQueues__queues();

        virtual ::ecore::EAttribute_ptr getQueue__name();

        virtual ::ecore::EAttribute_ptr getQueue__maxJobCpu();

        virtual ::ecore::EAttribute_ptr getQueue__maxProcCpu();

        virtual ::ecore::EAttribute_ptr getQueue__memory();

        virtual ::ecore::EAttribute_ptr getQueue__wallTime();

        virtual ::ecore::EAttribute_ptr getQueue__node();

        virtual ::ecore::EAttribute_ptr getQueue__nbRunningJobs();

        virtual ::ecore::EAttribute_ptr getQueue__nbJobsInQueue();

        virtual ::ecore::EAttribute_ptr getQueue__state();

        virtual ::ecore::EAttribute_ptr getQueue__priority();

        virtual ::ecore::EAttribute_ptr getQueue__description();

        virtual ::ecore::EAttribute_ptr getJobResult__jobId();

        virtual ::ecore::EAttribute_ptr getJobResult__outputPath();

        virtual ::ecore::EAttribute_ptr getJobResult__errorPath();

        virtual ::ecore::EAttribute_ptr getListJobResults__nbJobs();

        virtual ::ecore::EReference_ptr getListJobResults__Results();

    protected:

        static std::auto_ptr< TMS_DataPackage > s_instance;

        TMS_DataPackage();

        // EClass instances 

        ::ecore::EClass_ptr m_JobEClass;

        ::ecore::EClass_ptr m_ListJobsEClass;

        ::ecore::EClass_ptr m_SubmitOptionsEClass;

        ::ecore::EClass_ptr m_ListJobsOptionsEClass;

        ::ecore::EClass_ptr m_ProgressOptionsEClass;

        ::ecore::EClass_ptr m_ListProgressionEClass;

        ::ecore::EClass_ptr m_ProgressionEClass;

        ::ecore::EClass_ptr m_ListQueuesEClass;

        ::ecore::EClass_ptr m_QueueEClass;

        ::ecore::EClass_ptr m_JobResultEClass;

        ::ecore::EClass_ptr m_ListJobResultsEClass;

        // EEnuminstances 

        ::ecore::EEnum_ptr m_JobPriorityEEnum;

        ::ecore::EEnum_ptr m_JobStatusEEnum;

        ::ecore::EEnum_ptr m_QueuePriorityEEnum;

        ::ecore::EEnum_ptr m_QueueStatusEEnum;

        // EDataType instances 


        // EStructuralFeatures instances

        ::ecore::EAttribute_ptr m_Job__sessionId;

        ::ecore::EAttribute_ptr m_Job__submitMachineId;

        ::ecore::EAttribute_ptr m_Job__submitMachineName;

        ::ecore::EAttribute_ptr m_Job__jobId;

        ::ecore::EAttribute_ptr m_Job__jobName;

        ::ecore::EAttribute_ptr m_Job__jobPath;

        ::ecore::EAttribute_ptr m_Job__outputPath;

        ::ecore::EAttribute_ptr m_Job__errorPath;

        ::ecore::EAttribute_ptr m_Job__jobPrio;

        ::ecore::EAttribute_ptr m_Job__nbCpus;

        ::ecore::EAttribute_ptr m_Job__jobWorkingDir;

        ::ecore::EAttribute_ptr m_Job__status;

        ::ecore::EAttribute_ptr m_Job__submitDate;

        ::ecore::EAttribute_ptr m_Job__endDate;

        ::ecore::EAttribute_ptr m_Job__owner;

        ::ecore::EAttribute_ptr m_Job__jobQueue;

        ::ecore::EAttribute_ptr m_Job__wallClockLimit;

        ::ecore::EAttribute_ptr m_Job__groupName;

        ::ecore::EAttribute_ptr m_Job__jobDescription;

        ::ecore::EAttribute_ptr m_Job__memLimit;

        ::ecore::EAttribute_ptr m_Job__nbNodes;

        ::ecore::EAttribute_ptr m_Job__nbNodesAndCpuPerNode;

        ::ecore::EAttribute_ptr m_ListJobs__nbJobs;

        ::ecore::EAttribute_ptr m_ListJobs__nbRunningJobs;

        ::ecore::EAttribute_ptr m_ListJobs__nbWaitingJobs;

        ::ecore::EReference_ptr m_ListJobs__jobs;

        ::ecore::EAttribute_ptr m_SubmitOptions__name;

        ::ecore::EAttribute_ptr m_SubmitOptions__priority;

        ::ecore::EAttribute_ptr m_SubmitOptions__queue;

        ::ecore::EAttribute_ptr m_SubmitOptions__wallTime;

        ::ecore::EAttribute_ptr m_SubmitOptions__memory;

        ::ecore::EAttribute_ptr m_SubmitOptions__nbCpu;

        ::ecore::EAttribute_ptr m_SubmitOptions__nbNodesAndCpuPerNode;

        ::ecore::EAttribute_ptr m_SubmitOptions__outPutPath;

        ::ecore::EAttribute_ptr m_SubmitOptions__errorPath;

        ::ecore::EAttribute_ptr m_ListJobsOptions__jobId;

        ::ecore::EAttribute_ptr m_ListJobsOptions__nbCpu;

        ::ecore::EAttribute_ptr m_ListJobsOptions__fromSubmitDate;

        ::ecore::EAttribute_ptr m_ListJobsOptions__toSubmitDate;

        ::ecore::EAttribute_ptr m_ListJobsOptions__owner;

        ::ecore::EAttribute_ptr m_ListJobsOptions__status;

        ::ecore::EAttribute_ptr m_ListJobsOptions__priority;

        ::ecore::EAttribute_ptr m_ListJobsOptions__outPutPath;

        ::ecore::EAttribute_ptr m_ListJobsOptions__errorPath;

        ::ecore::EAttribute_ptr m_ListJobsOptions__queue;

        ::ecore::EAttribute_ptr m_ProgressOptions__jobId;

        ::ecore::EAttribute_ptr m_ProgressOptions__jobOwner;

        ::ecore::EAttribute_ptr m_ListProgression__nbJobs;

        ::ecore::EReference_ptr m_ListProgression__progress;

        ::ecore::EAttribute_ptr m_Progression__jobId;

        ::ecore::EAttribute_ptr m_Progression__jobName;

        ::ecore::EAttribute_ptr m_Progression__wallTime;

        ::ecore::EAttribute_ptr m_Progression__startTime;

        ::ecore::EAttribute_ptr m_Progression__endTime;

        ::ecore::EAttribute_ptr m_Progression__percent;

        ::ecore::EAttribute_ptr m_Progression__status;

        ::ecore::EAttribute_ptr m_ListQueues__nbQueues;

        ::ecore::EReference_ptr m_ListQueues__queues;

        ::ecore::EAttribute_ptr m_Queue__name;

        ::ecore::EAttribute_ptr m_Queue__maxJobCpu;

        ::ecore::EAttribute_ptr m_Queue__maxProcCpu;

        ::ecore::EAttribute_ptr m_Queue__memory;

        ::ecore::EAttribute_ptr m_Queue__wallTime;

        ::ecore::EAttribute_ptr m_Queue__node;

        ::ecore::EAttribute_ptr m_Queue__nbRunningJobs;

        ::ecore::EAttribute_ptr m_Queue__nbJobsInQueue;

        ::ecore::EAttribute_ptr m_Queue__state;

        ::ecore::EAttribute_ptr m_Queue__priority;

        ::ecore::EAttribute_ptr m_Queue__description;

        ::ecore::EAttribute_ptr m_JobResult__jobId;

        ::ecore::EAttribute_ptr m_JobResult__outputPath;

        ::ecore::EAttribute_ptr m_JobResult__errorPath;

        ::ecore::EAttribute_ptr m_ListJobResults__nbJobs;

        ::ecore::EReference_ptr m_ListJobResults__Results;

    };

} // TMS_Data


#endif // _TMS_DATAPACKAGE_HPP
