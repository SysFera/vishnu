// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/OptionValue.hpp
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
 * \file OptionValue.hpp
 * \brief The OptionValue class
 * \author Generated file
 * \date 20/02/2011
 */

#ifndef UMS_DATA_OPTIONVALUE_HPP
#define UMS_DATA_OPTIONVALUE_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(OptionValue_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class OptionValue
     * \brief Implementation of the OptionValue class
     */
    class OptionValue: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for OptionValue
         */
        OptionValue();
        /**
         * \brief The destructor for OptionValue
         */
        virtual ~OptionValue();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the optionName
         * \return The optionName attribute value
         **/
        ::ecore::EString const& getOptionName() const;
        /**
         * \brief To set the optionName
         * \param _optionName The optionName value
         **/
        void setOptionName(::ecore::EString const& _optionName);

        /**
         * \brief To get the value
         * \return The value attribute value
         **/
        ::ecore::EString const& getValue() const;
        /**
         * \brief To set the value
         * \param _value The value value
         **/
        void setValue(::ecore::EString const& _value);

        // References


        /*PROTECTED REGION ID(OptionValue) START*/
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

        /*PROTECTED REGION ID(OptionValueImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_optionName;

        ::ecore::EString m_value;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_OPTIONVALUE_HPP
