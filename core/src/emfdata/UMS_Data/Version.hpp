// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/Version.hpp
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
 * \file Version.hpp
 * \brief The Version class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef UMS_DATA_VERSION_HPP
#define UMS_DATA_VERSION_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Version_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class Version
     * \brief Implementation of the Version class
     */
    class Version: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Version
         */
        Version();
        /**
         * \brief The destructor for Version
         */
        virtual ~Version();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the major
         * \return The major attribute value
         **/
        ::ecore::EInt getMajor() const;
        /**
         * \brief To set the major
         * \param _major The major value
         **/
        void setMajor(::ecore::EInt _major);

        /**
         * \brief To get the minor
         * \return The minor attribute value
         **/
        ::ecore::EInt getMinor() const;
        /**
         * \brief To set the minor
         * \param _minor The minor value
         **/
        void setMinor(::ecore::EInt _minor);

        /**
         * \brief To get the patch
         * \return The patch attribute value
         **/
        ::ecore::EInt getPatch() const;
        /**
         * \brief To set the patch
         * \param _patch The patch value
         **/
        void setPatch(::ecore::EInt _patch);

        // References


        /*PROTECTED REGION ID(Version) START*/
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

        /*PROTECTED REGION ID(VersionImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EInt m_major;

        ::ecore::EInt m_minor;

        ::ecore::EInt m_patch;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_VERSION_HPP
