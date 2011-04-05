// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListJobResults.hpp
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
 * \file ListJobResults.hpp
 * \brief The ListJobResults class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef TMS_DATA_LISTJOBRESULTS_HPP
#define TMS_DATA_LISTJOBRESULTS_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ListJobResults_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    /**
     * \class ListJobResults
     * \brief Implementation of the ListJobResults class
     */
    class ListJobResults: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ListJobResults
         */
        ListJobResults();
        /**
         * \brief The destructor for ListJobResults
         */
        virtual ~ListJobResults();

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
        ::ecore::EString const& getNbJobs() const;
        /**
         * \brief To set the nbJobs
         * \param _nbJobs The nbJobs value
         **/
        void setNbJobs(::ecore::EString const& _nbJobs);

        // References
        /**
         * \brief To get the list of JobResults
         * \return A reference to a list of JobResults
         **/
        ::ecorecpp::mapping::EList< ::TMS_Data::JobResult >& getResults();

        /*PROTECTED REGION ID(ListJobResults) START*/
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

        /*PROTECTED REGION ID(ListJobResultsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_nbJobs;

        // References

        ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList<
                ::TMS_Data::JobResult > > m_Results;

    };

} // TMS_Data

#endif // TMS_DATA_LISTJOBRESULTS_HPP
