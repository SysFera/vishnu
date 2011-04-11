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

    class ListJobs: public virtual ::ecore::EObject

    {
    public:
        ListJobs();

        virtual ~ListJobs();

        virtual void _initialize();

        // Operations


        // Attributes
        ::ecore::ELong getNbJobs() const;
        void setNbJobs(::ecore::ELong _nbJobs);

        ::ecore::ELong getNbRunningJobs() const;
        void setNbRunningJobs(::ecore::ELong _nbRunningJobs);

        ::ecore::ELong getNbWaitingJobs() const;
        void setNbWaitingJobs(::ecore::ELong _nbWaitingJobs);

        // References
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
