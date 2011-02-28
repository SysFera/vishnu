// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListOptionsValues.hpp
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
 * \file ListOptionsValues.hpp
 * \brief Represents the class to list the values of the options
 * \author Generated file
 * \date 31/01/2011
 */

#ifndef UMS_DATA_LISTOPTIONSVALUES_HPP
#define UMS_DATA_LISTOPTIONSVALUES_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ListOptionsValues_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class ListOptionsValues
     * \brief A list of the options with their values
     */
    class ListOptionsValues: public virtual ::ecore::EObject

    {
    public:
	/**
	 * \brief Default constructor
	 * \fn ListOptionsValues()
	 */
        ListOptionsValues();

	/**
	 * \brief Destructor
	 * \fn ~ListOptionsValues()
	 */
        virtual ~ListOptionsValues();

	/**
	 * \brief To initialize the list
	 * \fn void _initialize()
	 */
        virtual void _initialize();

        // Operations


        // Attributes

        // References
	/**
	 * \brief Getter to get the list of option values
	 * \fn ::ecorecpp::mapping::EList< ::UMS_Data::OptionValue >& getOptionValues()
	 * \return The list of option values
	 */
        ::ecorecpp::mapping::EList< ::UMS_Data::OptionValue >
                & getOptionValues();

        /*PROTECTED REGION ID(ListOptionsValues) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

        // EObjectImpl
	/**
	 * \brief Getter to get a feature of the class
	 * \fn virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID,
	 *                                       ::ecore::EBoolean _resolve)
	 * \param _featureID The id of the attribute
	 * \param _resolve 
	 * \return The attribute corresponding to the id
	 */
        virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID,
                ::ecore::EBoolean _resolve);
	/**
	 * \brief To set a feature value
	 * \fn virtual void eSet(::ecore::EInt _featureID,
	 *                       ::ecore::EJavaObject const& _newValue)
	 * \param _featureID The id of the feature
	 * \param _newValue The new value of the feature
	 */
        virtual void eSet(::ecore::EInt _featureID,
                ::ecore::EJavaObject const& _newValue);
	/**
	 * \brief To know if a feature is set
	 * \fn virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID)
	 * \param _featureID The id of the feature
	 * \return If the feature is set
	 */
        virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID);
	/**
	 * \brief To unset a feature
	 * \fn virtual void eUnset(::ecore::EInt _featureID)
	 * \param _featureID The id of the feature
	 */
        virtual void eUnset(::ecore::EInt _featureID);
	/**
	 * \brief To get tje class
	 * \fn virtual ::ecore::EClass_ptr _eClass()
	 * \return A point to the class
	 */
        virtual ::ecore::EClass_ptr _eClass();

        /*PROTECTED REGION ID(ListOptionsValuesImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes


        // References

	/**
	 * \brief List of Option values
	 */
        ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList<
                ::UMS_Data::OptionValue > > m_optionValues;

    };

} // UMS_Data

#endif // UMS_DATA_LISTOPTIONSVALUES_HPP
