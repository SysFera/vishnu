// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * AMS_Data/AMS_DataFactory.hpp
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
 * \file AMS_DataFactory.hpp
 * \brief The AMS_DataFactory class
 * \author Generated file
 * \date 31/03/2011
 */
#ifndef _AMS_DATAFACTORY_HPP
#define _AMS_DATAFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <AMS_Data.hpp>

namespace AMS_Data
{
    /**
     * \class AMS_DataFactory
     * \brief The factory class for generated model AMS_Data
     */
    class AMS_DataFactory: public virtual ::ecore::EFactory
    {
    public:

        /**
         * \brief To get the instance of the singleton
         * \return Pointer to the instance
         */
        static AMS_DataFactory_ptr _instance();

        /**
         * \brief Creates a new object of class Application
         * \return Pointer to the object
         */
        virtual Application_ptr createApplication();
        /**
         * \brief Creates a new object of class ListApplications
         * \return Pointer to the object
         */
        virtual ListApplications_ptr createListApplications();
        /**
         * \brief Creates a new object of class ListApplicationsOp
         * \return Pointer to the object
         */
        virtual ListApplicationsOp_ptr createListApplicationsOp();

        /**
         * \brief To create an object of a given class (reflective API)
         * \param _eClass The class object to create (obtained using the Package class)
         * \return Generic pointer to the object
         */
        virtual ::ecore::EObject_ptr create(::ecore::EClass_ptr _eClass);

        /**
         * \brief To create an object from a string (deserialize)
         * \param _eDataType The datatype
         * \param _literalValue The string
         * \return Generic pointer to the object
         */
        virtual ::ecore::EJavaObject createFromString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EString const& _literalValue);

        /**
         * \brief To convert an object to a string (serialize)
         * \param _eDataType The datatype
         * \param _instanceValue Value of the instance
         * \return The object serialized in a string
         */
        virtual ::ecore::EString convertToString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EJavaObject const& _instanceValue);

    protected:

        /**
         * \brief The instance of the singleton
         */
        static std::auto_ptr< AMS_DataFactory > s_instance;

        /**
         * \brief Constructor
         */
        AMS_DataFactory();

    };

} // AMS_Data


#endif // _AMS_DATAFACTORY_HPP
