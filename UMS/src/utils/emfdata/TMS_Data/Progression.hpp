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

/**
 * \file Progression.hpp
 * \brief The Progression class
 * \author Generated file
 * \date 31/03/2011
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

    /**
     * \class Progression
     * \brief Implementation of the Progression class
     */
    class Progression: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Progression
         */
        Progression();
        /**
         * \brief The destructor for Progression
         */
        virtual ~Progression();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
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
         * \brief To get the wallTime
         * \return The wallTime attribute value
         **/
        ::ecore::EInt getWallTime() const;
        /**
         * \brief To set the wallTime
         * \param _wallTime The wallTime value
         **/
        void setWallTime(::ecore::EInt _wallTime);

        /**
         * \brief To get the startTime
         * \return The startTime attribute value
         **/
        ::ecore::ELong getStartTime() const;
        /**
         * \brief To set the startTime
         * \param _startTime The startTime value
         **/
        void setStartTime(::ecore::ELong _startTime);

        /**
         * \brief To get the endTime
         * \return The endTime attribute value
         **/
        ::ecore::ELong getEndTime() const;
        /**
         * \brief To set the endTime
         * \param _endTime The endTime value
         **/
        void setEndTime(::ecore::ELong _endTime);

        /**
         * \brief To get the percent
         * \return The percent attribute value
         **/
        ::ecore::EDouble getPercent() const;
        /**
         * \brief To set the percent
         * \param _percent The percent value
         **/
        void setPercent(::ecore::EDouble _percent);

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
