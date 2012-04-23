// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListJobs.hpp
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
 * \file ListJobs.hpp
 * \brief The ListJobs class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef TMS_DATA_LISTJOBS_HPP
#define TMS_DATA_LISTJOBS_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ListJobs_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    /**
     * \class ListJobs
     * \brief Implementation of the ListJobs class
     */
    class ListJobs: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ListJobs
         */
        ListJobs();
        /**
         * \brief The destructor for ListJobs
         */
        virtual ~ListJobs();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the nbJobs
         * \return The nbJobs attribute value
         **/
        ::ecore::ELong getNbJobs() const;
        /**
         * \brief To set the nbJobs
         * \param _nbJobs The nbJobs value
         **/
        void setNbJobs(::ecore::ELong _nbJobs);

        /**
         * \brief To get the nbRunningJobs
         * \return The nbRunningJobs attribute value
         **/
        ::ecore::ELong getNbRunningJobs() const;
        /**
         * \brief To set the nbRunningJobs
         * \param _nbRunningJobs The nbRunningJobs value
         **/
        void setNbRunningJobs(::ecore::ELong _nbRunningJobs);

        /**
         * \brief To get the nbWaitingJobs
         * \return The nbWaitingJobs attribute value
         **/
        ::ecore::ELong getNbWaitingJobs() const;
        /**
         * \brief To set the nbWaitingJobs
         * \param _nbWaitingJobs The nbWaitingJobs value
         **/
        void setNbWaitingJobs(::ecore::ELong _nbWaitingJobs);

        // References
        /**
         * \brief To get the list of Jobs
         * \return A reference to a list of Jobs
         **/
        ::ecorecpp::mapping::EList< ::TMS_Data::Job >& getJobs();

        /*PROTECTED REGION ID(ListJobs) START*/
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

        /*PROTECTED REGION ID(ListJobsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::ELong m_nbJobs;

        ::ecore::ELong m_nbRunningJobs;

        ::ecore::ELong m_nbWaitingJobs;

        // References

        ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList<
                ::TMS_Data::Job > > m_jobs;

    };

} // TMS_Data

#endif // TMS_DATA_LISTJOBS_HPP
