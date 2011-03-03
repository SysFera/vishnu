// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/Configuration.hpp
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
 * \file Configuration.hpp
 * \brief The Configuration class
 * \author Generated file
 * \date 20/02/2011
 */

#ifndef UMS_DATA_CONFIGURATION_HPP
#define UMS_DATA_CONFIGURATION_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Configuration_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class Configuration
     * \brief Implementation of the Configuration class
     */
    class Configuration: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Configuration
         */
        Configuration();
        /**
         * \brief The destructor for Configuration
         */
        virtual ~Configuration();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the filePath
         * \return The filePath attribute value
         **/
        ::ecore::EString const& getFilePath() const;
        /**
         * \brief To set the filePath
         * \param _filePath The filePath value
         **/
        void setFilePath(::ecore::EString const& _filePath);

        // References
        /**
         * \brief To get the list of Users
         * \return A reference to a list of Users
         **/
        ::ecorecpp::mapping::EList< ::UMS_Data::User >& getListConfUsers();

        /**
         * \brief To get the list of Machines
         * \return A reference to a list of Machines
         **/
        ::ecorecpp::mapping::EList< ::UMS_Data::Machine >
                & getListConfMachines();

        /**
         * \brief To get the list of LocalAccounts
         * \return A reference to a list of LocalAccounts
         **/
        ::ecorecpp::mapping::EList< ::UMS_Data::LocalAccount >
                & getListConfLocalAccounts();

        /*PROTECTED REGION ID(Configuration) START*/
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

        /*PROTECTED REGION ID(ConfigurationImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_filePath;

        // References

        ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList<
                ::UMS_Data::User > > m_listConfUsers;

        ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList<
                ::UMS_Data::Machine > > m_listConfMachines;

        ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList<
                ::UMS_Data::LocalAccount > > m_listConfLocalAccounts;

    };

} // UMS_Data

#endif // UMS_DATA_CONFIGURATION_HPP
