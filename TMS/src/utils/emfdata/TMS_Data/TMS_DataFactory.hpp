// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/TMS_DataFactory.hpp
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

#ifndef _TMS_DATAFACTORY_HPP
#define _TMS_DATAFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <TMS_Data.hpp>

namespace TMS_Data
{

    class TMS_DataFactory: public virtual ::ecore::EFactory
    {
    public:

        static TMS_DataFactory_ptr _instance();

        virtual Job_ptr createJob();
        virtual ListJobs_ptr createListJobs();
        virtual SubmitOptions_ptr createSubmitOptions();
        virtual ListJobsOptions_ptr createListJobsOptions();
        virtual ProgressOptions_ptr createProgressOptions();
        virtual ListProgression_ptr createListProgression();
        virtual Progression_ptr createProgression();
        virtual ListQueues_ptr createListQueues();
        virtual Queue_ptr createQueue();
        virtual JobResult_ptr createJobResult();
        virtual ListJobResults_ptr createListJobResults();

        virtual ::ecore::EObject_ptr create(::ecore::EClass_ptr _eClass);
        virtual ::ecore::EJavaObject createFromString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EString const& _literalValue);
        virtual ::ecore::EString convertToString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EJavaObject const& _instanceValue);

    protected:

        static std::auto_ptr< TMS_DataFactory > s_instance;

        TMS_DataFactory();

    };

} // TMS_Data


#endif // _TMS_DATAFACTORY_HPP
