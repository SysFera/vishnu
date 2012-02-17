// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/RestartOp.hpp
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
 * \file RestartOp.hpp
 * \brief The RestartOp class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef IMS_DATA_RESTARTOP_HPP
#define IMS_DATA_RESTARTOP_HPP

#include <IMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
/*PROTECTED REGION ID(RestartOp_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace IMS_Data
{

    /**
     * \class RestartOp
     * \brief Implementation of the RestartOp class
     */
    class VISHNU_API_LIB RestartOp: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for RestartOp
         */
        RestartOp();
        /**
         * \brief The destructor for RestartOp
         */
        virtual ~RestartOp();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the vishnuConf
         * \return The vishnuConf attribute value
         **/
        ::ecore::EString const& getVishnuConf() const;
        /**
         * \brief To set the vishnuConf
         * \param _vishnuConf The vishnuConf value
         **/
        void setVishnuConf(::ecore::EString const& _vishnuConf);

        /**
         * \brief To get the sedType
         * \return The sedType attribute value
         **/
        ::IMS_Data::SeDType getSedType() const;
        /**
         * \brief To set the sedType
         * \param _sedType The sedType value
         **/
        void setSedType(::IMS_Data::SeDType _sedType);

        // References


        /*PROTECTED REGION ID(RestartOp) START*/
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

        /*PROTECTED REGION ID(RestartOpImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_vishnuConf;

        ::IMS_Data::SeDType m_sedType;

        // References

    };

} // IMS_Data

#endif // IMS_DATA_RESTARTOP_HPP
