// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * AMS_Data/AMS_DataPackage.hpp
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
 * \file AMS_DataPackage.hpp
 * \brief The AMS_DataPackage class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef _AMS_DATAPACKAGE_HPP
#define _AMS_DATAPACKAGE_HPP

#include <ecore/EPackage.hpp>
#include <AMS_Data_forward.hpp>

namespace AMS_Data
{
    /**
     * \class AMS_DataPackage
     * \brief The package class (reflective API) for generated model AMS_Data
     */
    class AMS_DataPackage: public virtual ::ecore::EPackage
    {
    public:

        /**
         * \brief To get the instance of the singleton
         * \return Pointer to the instance
         */
        static AMS_DataPackage_ptr _instance();

        // IDs for classifiers

        /**
         * \brief Constant for Application class
         */
        static const int APPLICATION = 0;

        /**
         * \brief Constant for ListApplications class
         */
        static const int LISTAPPLICATIONS = 1;

        /**
         * \brief Constant for ListApplicationsOp class
         */
        static const int LISTAPPLICATIONSOP = 2;

        /**
         * \brief Constant for APPLICATION__NAME feature
         */
        static const int APPLICATION__NAME = 0;

        /**
         * \brief Constant for APPLICATION__DESCRIPTION feature
         */
        static const int APPLICATION__DESCRIPTION = 1;

        /**
         * \brief Constant for APPLICATION__INPUT feature
         */
        static const int APPLICATION__INPUT = 2;

        /**
         * \brief Constant for APPLICATION__OUTPUT feature
         */
        static const int APPLICATION__OUTPUT = 3;

        /**
         * \brief Constant for APPLICATION__SCRIPT feature
         */
        static const int APPLICATION__SCRIPT = 4;

        /**
         * \brief Constant for APPLICATION__APPLICATIONID feature
         */
        static const int APPLICATION__APPLICATIONID = 5;

        /**
         * \brief Constant for LISTAPPLICATIONS__APPL feature
         */
        static const int LISTAPPLICATIONS__APPL = 6;

        /**
         * \brief Constant for LISTAPPLICATIONSOP__MACHINEID feature
         */
        static const int LISTAPPLICATIONSOP__MACHINEID = 7;

        // EClassifiers methods

        /**
         * \brief Returns the reflective object for class Application
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getApplication();

        /**
         * \brief Returns the reflective object for class ListApplications
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListApplications();

        /**
         * \brief Returns the reflective object for class ListApplicationsOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EClass_ptr getListApplicationsOp();

        // EStructuralFeatures methods

        /**
         * \brief Returns the reflective object for feature name of class Application
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getApplication__name();

        /**
         * \brief Returns the reflective object for feature description of class Application
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getApplication__description();

        /**
         * \brief Returns the reflective object for feature input of class Application
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getApplication__input();

        /**
         * \brief Returns the reflective object for feature output of class Application
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getApplication__output();

        /**
         * \brief Returns the reflective object for feature script of class Application
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getApplication__script();

        /**
         * \brief Returns the reflective object for feature applicationId of class Application
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getApplication__applicationId();

        /**
         * \brief Returns the reflective object for feature appl of class ListApplications
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EReference_ptr getListApplications__appl();

        /**
         * \brief Returns the reflective object for feature machineId of class ListApplicationsOp
         * \return A pointer to the reflective object
         */
        virtual ::ecore::EAttribute_ptr getListApplicationsOp__machineId();

    protected:

        /**
         * \brief The instance of the package
         */
        static std::auto_ptr< AMS_DataPackage > s_instance;

        AMS_DataPackage();

        // EClass instances 

        /**
         * \brief The instance for the class Application
         */
        ::ecore::EClass_ptr m_ApplicationEClass;

        /**
         * \brief The instance for the class ListApplications
         */
        ::ecore::EClass_ptr m_ListApplicationsEClass;

        /**
         * \brief The instance for the class ListApplicationsOp
         */
        ::ecore::EClass_ptr m_ListApplicationsOpEClass;

        // EEnuminstances 


        // EDataType instances 


        // EStructuralFeatures instances

        /**
         * \brief The instance for the feature name of class Application
         */
        ::ecore::EAttribute_ptr m_Application__name;

        /**
         * \brief The instance for the feature description of class Application
         */
        ::ecore::EAttribute_ptr m_Application__description;

        /**
         * \brief The instance for the feature input of class Application
         */
        ::ecore::EAttribute_ptr m_Application__input;

        /**
         * \brief The instance for the feature output of class Application
         */
        ::ecore::EAttribute_ptr m_Application__output;

        /**
         * \brief The instance for the feature script of class Application
         */
        ::ecore::EAttribute_ptr m_Application__script;

        /**
         * \brief The instance for the feature applicationId of class Application
         */
        ::ecore::EAttribute_ptr m_Application__applicationId;

        /**
         * \brief The instance for the feature appl of class ListApplications
         */
        ::ecore::EReference_ptr m_ListApplications__appl;

        /**
         * \brief The instance for the feature machineId of class ListApplicationsOp
         */
        ::ecore::EAttribute_ptr m_ListApplicationsOp__machineId;

    };

} // AMS_Data


#endif // _AMS_DATAPACKAGE_HPP
