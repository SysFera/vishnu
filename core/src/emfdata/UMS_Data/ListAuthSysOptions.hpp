// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListAuthSysOptions.hpp
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
 * \file ListAuthSysOptions.hpp
 * \brief The ListAuthSysOptions class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef UMS_DATA_LISTAUTHSYSOPTIONS_HPP
#define UMS_DATA_LISTAUTHSYSOPTIONS_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ListAuthSysOptions_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class ListAuthSysOptions
     * \brief Implementation of the ListAuthSysOptions class
     */
    class ListAuthSysOptions: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ListAuthSysOptions
         */
        ListAuthSysOptions();
        /**
         * \brief The destructor for ListAuthSysOptions
         */
        virtual ~ListAuthSysOptions();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the listAllAuthSystems
         * \return The listAllAuthSystems attribute value
         **/
        ::ecore::EBoolean isListAllAuthSystems() const;
        /**
         * \brief To set the listAllAuthSystems
         * \param _listAllAuthSystems The listAllAuthSystems value
         **/
        void setListAllAuthSystems(::ecore::EBoolean _listAllAuthSystems);

        /**
         * \brief To get the userId
         * \return The userId attribute value
         **/
        ::ecore::EString const& getUserId() const;
        /**
         * \brief To set the userId
         * \param _userId The userId value
         **/
        void setUserId(::ecore::EString const& _userId);

        /**
         * \brief To get the authSystemId
         * \return The authSystemId attribute value
         **/
        ::ecore::EString const& getAuthSystemId() const;
        /**
         * \brief To set the authSystemId
         * \param _authSystemId The authSystemId value
         **/
        void setAuthSystemId(::ecore::EString const& _authSystemId);

        // References


        /*PROTECTED REGION ID(ListAuthSysOptions) START*/
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

        /*PROTECTED REGION ID(ListAuthSysOptionsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EBoolean m_listAllAuthSystems;

        ::ecore::EString m_userId;

        ::ecore::EString m_authSystemId;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_LISTAUTHSYSOPTIONS_HPP
