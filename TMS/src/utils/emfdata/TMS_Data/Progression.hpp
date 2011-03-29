// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/Progression.hpp
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

#ifndef TMS_DATA_PROGRESSION_HPP
#define TMS_DATA_PROGRESSION_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Progression_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    class Progression: public virtual ::ecore::EObject

    {
    public:
        Progression();

        virtual ~Progression();

        virtual void _initialize();

        // Operations


        // Attributes
        ::ecore::EString const& getJobId() const;
        void setJobId(::ecore::EString const& _jobId);

        ::ecore::EString const& getJobName() const;
        void setJobName(::ecore::EString const& _jobName);

        ::ecore::EInt getWallTime() const;
        void setWallTime(::ecore::EInt _wallTime);

        ::ecore::ELong getStartTime() const;
        void setStartTime(::ecore::ELong _startTime);

        ::ecore::ELong getEndTime() const;
        void setEndTime(::ecore::ELong _endTime);

        ::ecore::EDouble getPercent() const;
        void setPercent(::ecore::EDouble _percent);

        ::TMS_Data::JobStatus getStatus() const;
        void setStatus(::TMS_Data::JobStatus _status);

        // References


        /*PROTECTED REGION ID(Progression) START*/
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

        /*PROTECTED REGION ID(ProgressionImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_jobId;

        ::ecore::EString m_jobName;

        ::ecore::EInt m_wallTime;

        ::ecore::ELong m_startTime;

        ::ecore::ELong m_endTime;

        ::ecore::EDouble m_percent;

        ::TMS_Data::JobStatus m_status;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_PROGRESSION_HPP
