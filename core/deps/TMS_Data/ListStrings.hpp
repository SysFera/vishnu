// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListStrings.hpp
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
 * \file ListStrings.hpp
 * \brief The ListStrings class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef TMS_DATA_LISTSTRINGS_HPP
#define TMS_DATA_LISTSTRINGS_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ListStrings_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    /**
     * \class ListStrings
     * \brief Implementation of the ListStrings class
     */
    class ListStrings: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ListStrings
         */
        ListStrings();
        /**
         * \brief The destructor for ListStrings
         */
        virtual ~ListStrings();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the string
         * \return The string attribute value
         **/
        std::vector< ::ecore::EString > const& getString() const;
        /**
         * \brief To set the string
         * \param _string The string value
         **/
        void setString(std::vector< ::ecore::EString > const& _string);

        void addString(::ecore::EString const& _new_element_in_string);
        void setStringAt(size_t _position,
                ::ecore::EString const& _new_element_in_string);
        void deleteStringAt(size_t _position);

        // References


        /*PROTECTED REGION ID(ListStrings) START*/
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

        /*PROTECTED REGION ID(ListStringsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        std::vector< ::ecore::EString > m_string;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_LISTSTRINGS_HPP
