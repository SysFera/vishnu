// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/CpFileOptions.hpp
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
 * \file CpFileOptions.hpp
 * \brief The CpFileOptions class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef FMS_DATA_CPFILEOPTIONS_HPP
#define FMS_DATA_CPFILEOPTIONS_HPP

#include <FMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
/*PROTECTED REGION ID(CpFileOptions_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace FMS_Data
{

    /**
     * \class CpFileOptions
     * \brief Implementation of the CpFileOptions class
     */
    class VISHNU_API_LIB CpFileOptions: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for CpFileOptions
         */
        CpFileOptions();
        /**
         * \brief The destructor for CpFileOptions
         */
        virtual ~CpFileOptions();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the isRecursive
         * \return The isRecursive attribute value
         **/
        ::ecore::EBoolean isIsRecursive() const;
        /**
         * \brief To set the isRecursive
         * \param _isRecursive The isRecursive value
         **/
        void setIsRecursive(::ecore::EBoolean _isRecursive);

        /**
         * \brief To get the trCommand
         * \return The trCommand attribute value
         **/
        ::FMS_Data::TransferCommand getTrCommand() const;
        /**
         * \brief To set the trCommand
         * \param _trCommand The trCommand value
         **/
        void setTrCommand(::FMS_Data::TransferCommand _trCommand);

        // References


        /*PROTECTED REGION ID(CpFileOptions) START*/
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

        /*PROTECTED REGION ID(CpFileOptionsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EBoolean m_isRecursive;

        ::FMS_Data::TransferCommand m_trCommand;

        // References

    };

} // FMS_Data

#endif // FMS_DATA_CPFILEOPTIONS_HPP
