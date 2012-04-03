// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * AMS_Data/Application.hpp
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
 * \file Application.hpp
 * \brief The Application class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef AMS_DATA_APPLICATION_HPP
#define AMS_DATA_APPLICATION_HPP

#include <AMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Application_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace AMS_Data
{

    /**
     * \class Application
     * \brief Implementation of the Application class
     */
    class Application: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Application
         */
        Application();
        /**
         * \brief The destructor for Application
         */
        virtual ~Application();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the profile
         * \return The profile attribute value
         **/
        ::ecore::EString const& getProfile() const;
        /**
         * \brief To set the profile
         * \param _profile The profile value
         **/
        void setProfile(::ecore::EString const& _profile);

        /**
         * \brief To get the description
         * \return The description attribute value
         **/
        ::ecore::EString const& getDescription() const;
        /**
         * \brief To set the description
         * \param _description The description value
         **/
        void setDescription(::ecore::EString const& _description);

        /**
         * \brief To get the input
         * \return The input attribute value
         **/
        ::ecore::EString const& getInput() const;
        /**
         * \brief To set the input
         * \param _input The input value
         **/
        void setInput(::ecore::EString const& _input);

        /**
         * \brief To get the output
         * \return The output attribute value
         **/
        ::ecore::EString const& getOutput() const;
        /**
         * \brief To set the output
         * \param _output The output value
         **/
        void setOutput(::ecore::EString const& _output);

        // References


        /*PROTECTED REGION ID(Application) START*/
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

        /*PROTECTED REGION ID(ApplicationImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_profile;

        ::ecore::EString m_description;

        ::ecore::EString m_input;

        ::ecore::EString m_output;

        // References

    };

} // AMS_Data

#endif // AMS_DATA_APPLICATION_HPP
