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

    class Configuration: public virtual ::ecore::EObject

    {
    public:
        Configuration();

        virtual ~Configuration();

        virtual void _initialize();

        // Operations


        // Attributes
        ::ecore::EString const& getFilePath() const;
        void setFilePath(::ecore::EString const& _filePath);

        // References
        ::ecorecpp::mapping::EList< ::UMS_Data::User >& getListConfUsers();

        ::ecorecpp::mapping::EList< ::UMS_Data::Machine >
                & getListConfMachines();

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
