// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/TMS_DataFactoryImpl.cpp
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

#include <TMS_Data/TMS_DataFactory.hpp>
#include <TMS_Data/TMS_DataPackage.hpp>
#include <TMS_Data/Job.hpp>
#include <TMS_Data/ListJobs.hpp>
#include <TMS_Data/SubmitOptions.hpp>
#include <TMS_Data/ListJobsOptions.hpp>
#include <TMS_Data/ProgressOptions.hpp>
#include <TMS_Data/ListProgression.hpp>
#include <TMS_Data/Progression.hpp>
#include <TMS_Data/ListQueues.hpp>
#include <TMS_Data/Queue.hpp>
#include <TMS_Data/JobResult.hpp>
#include <TMS_Data/ListJobResults.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::TMS_Data;

TMS_DataFactory::TMS_DataFactory()
{
    s_instance.reset(this);
}

::ecore::EObject_ptr TMS_DataFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case TMS_DataPackage::JOB:
        return createJob();
    case TMS_DataPackage::LISTJOBS:
        return createListJobs();
    case TMS_DataPackage::SUBMITOPTIONS:
        return createSubmitOptions();
    case TMS_DataPackage::LISTJOBSOPTIONS:
        return createListJobsOptions();
    case TMS_DataPackage::PROGRESSOPTIONS:
        return createProgressOptions();
    case TMS_DataPackage::LISTPROGRESSION:
        return createListProgression();
    case TMS_DataPackage::PROGRESSION:
        return createProgression();
    case TMS_DataPackage::LISTQUEUES:
        return createListQueues();
    case TMS_DataPackage::QUEUE:
        return createQueue();
    case TMS_DataPackage::JOBRESULT:
        return createJobResult();
    case TMS_DataPackage::LISTJOBRESULTS:
        return createListJobResults();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject TMS_DataFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const& _literalValue)
{
    switch (_eDataType->getClassifierID())
    {
    case TMS_DataPackage::JOBPRIORITY:
    {
        ::ecore::EJavaObject _any;
        TMS_DataPackage_ptr _epkg =
                dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (getEPackage());
        return _epkg->getJobPriority()->getEEnumLiteralByLiteral(_literalValue)->getValue();
    }
    case TMS_DataPackage::JOBSTATUS:
    {
        ::ecore::EJavaObject _any;
        TMS_DataPackage_ptr _epkg =
                dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (getEPackage());
        return _epkg->getJobStatus()->getEEnumLiteralByLiteral(_literalValue)->getValue();
    }
    case TMS_DataPackage::QUEUEPRIORITY:
    {
        ::ecore::EJavaObject _any;
        TMS_DataPackage_ptr _epkg =
                dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (getEPackage());
        return _epkg->getQueuePriority()->getEEnumLiteralByLiteral(
                _literalValue)->getValue();
    }
    case TMS_DataPackage::QUEUESTATUS:
    {
        ::ecore::EJavaObject _any;
        TMS_DataPackage_ptr _epkg =
                dynamic_cast< ::TMS_Data::TMS_DataPackage_ptr > (getEPackage());
        return _epkg->getQueueStatus()->getEEnumLiteralByLiteral(_literalValue)->getValue();
    }
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString TMS_DataFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const& _instanceValue)
{
    switch (_eDataType->getClassifierID())
    {
    case TMS_DataPackage::JOBPRIORITY:
    {
        TMS_DataPackage_ptr _epkg = ::TMS_Data::instanceOf<
                ::TMS_Data::TMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getJobPriority()->getEEnumLiteral(_value)->getName();
    }
    case TMS_DataPackage::JOBSTATUS:
    {
        TMS_DataPackage_ptr _epkg = ::TMS_Data::instanceOf<
                ::TMS_Data::TMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getJobStatus()->getEEnumLiteral(_value)->getName();
    }
    case TMS_DataPackage::QUEUEPRIORITY:
    {
        TMS_DataPackage_ptr _epkg = ::TMS_Data::instanceOf<
                ::TMS_Data::TMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getQueuePriority()->getEEnumLiteral(_value)->getName();
    }
    case TMS_DataPackage::QUEUESTATUS:
    {
        TMS_DataPackage_ptr _epkg = ::TMS_Data::instanceOf<
                ::TMS_Data::TMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getQueueStatus()->getEEnumLiteral(_value)->getName();
    }
    default:
        throw "IllegalArgumentException";
    }
}

Job_ptr TMS_DataFactory::createJob()
{
    return new Job();
}
ListJobs_ptr TMS_DataFactory::createListJobs()
{
    return new ListJobs();
}
SubmitOptions_ptr TMS_DataFactory::createSubmitOptions()
{
    return new SubmitOptions();
}
ListJobsOptions_ptr TMS_DataFactory::createListJobsOptions()
{
    return new ListJobsOptions();
}
ProgressOptions_ptr TMS_DataFactory::createProgressOptions()
{
    return new ProgressOptions();
}
ListProgression_ptr TMS_DataFactory::createListProgression()
{
    return new ListProgression();
}
Progression_ptr TMS_DataFactory::createProgression()
{
    return new Progression();
}
ListQueues_ptr TMS_DataFactory::createListQueues()
{
    return new ListQueues();
}
Queue_ptr TMS_DataFactory::createQueue()
{
    return new Queue();
}
JobResult_ptr TMS_DataFactory::createJobResult()
{
    return new JobResult();
}
ListJobResults_ptr TMS_DataFactory::createListJobResults()
{
    return new ListJobResults();
}

