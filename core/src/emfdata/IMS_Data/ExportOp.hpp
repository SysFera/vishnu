// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/ExportOp.hpp
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
 * \file ExportOp.hpp
 * \brief The ExportOp class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef IMS_DATA_EXPORTOP_HPP
#define IMS_DATA_EXPORTOP_HPP

#include <IMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ExportOp_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace IMS_Data
{

    /**
     * \class ExportOp
     * \brief Implementation of the ExportOp class
     */
    class ExportOp: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ExportOp
         */
        ExportOp();
        /**
         * \brief The destructor for ExportOp
         */
        virtual ~ExportOp();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the exportType
         * \return The exportType attribute value
         **/
        ::IMS_Data::ExportType getExportType() const;
        /**
         * \brief To set the exportType
         * \param _exportType The exportType value
         **/
        void setExportType(::IMS_Data::ExportType _exportType);

        // References


        /*PROTECTED REGION ID(ExportOp) START*/
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

        /*PROTECTED REGION ID(ExportOpImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::IMS_Data::ExportType m_exportType;

        // References

    };

} // IMS_Data

#endif // IMS_DATA_EXPORTOP_HPP
