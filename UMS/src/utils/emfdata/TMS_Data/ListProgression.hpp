// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListProgression.hpp
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
 * \file ListProgression.hpp
 * \brief The ListProgression class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef TMS_DATA_LISTPROGRESSION_HPP
#define TMS_DATA_LISTPROGRESSION_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ListProgression_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    /**
     * \class ListProgression
     * \brief Implementation of the ListProgression class
     */
    class ListProgression: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ListProgression
         */
        ListProgression();
        /**
         * \brief The destructor for ListProgression
         */
        virtual ~ListProgression();

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
        ::ecore::EInt getNbJobs() const;
        /**
         * \brief To set the nbJobs
         * \param _nbJobs The nbJobs value
         **/
        void setNbJobs(::ecore::EInt _nbJobs);

        // References
        /**
         * \brief To get the list of Progressions
         * \return A reference to a list of Progressions
         **/
        ::ecorecpp::mapping::EList< ::TMS_Data::Progression >& getProgress();

        /*PROTECTED REGION ID(ListProgression) START*/
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

        /*PROTECTED REGION ID(ListProgressionImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EInt m_nbJobs;

        // References

        ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList<
                ::TMS_Data::Progression > > m_progress;

    };

} // TMS_Data

#endif // TMS_DATA_LISTPROGRESSION_HPP
