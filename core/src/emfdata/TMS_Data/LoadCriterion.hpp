// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/LoadCriterion.hpp
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
 * \file LoadCriterion.hpp
 * \brief The LoadCriterion class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef TMS_DATA_LOADCRITERION_HPP
#define TMS_DATA_LOADCRITERION_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore/EObject.hpp>
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
/*PROTECTED REGION ID(LoadCriterion_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    /**
     * \class LoadCriterion
     * \brief Implementation of the LoadCriterion class
     */
    class VISHNU_API_LIB LoadCriterion: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for LoadCriterion
         */
        LoadCriterion();
        /**
         * \brief The destructor for LoadCriterion
         */
        virtual ~LoadCriterion();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the loadType
         * \return The loadType attribute value
         **/
        ::TMS_Data::LoadType getLoadType() const;
        /**
         * \brief To set the loadType
         * \param _loadType The loadType value
         **/
        void setLoadType(::TMS_Data::LoadType _loadType);

        // References


        /*PROTECTED REGION ID(LoadCriterion) START*/
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

        /*PROTECTED REGION ID(LoadCriterionImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::TMS_Data::LoadType m_loadType;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_LOADCRITERION_HPP
