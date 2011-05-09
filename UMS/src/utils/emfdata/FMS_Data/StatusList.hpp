// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/StatusList.hpp
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
 * \file StatusList.hpp
 * \brief The StatusList class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef FMS_DATA_STATUSLIST_HPP
#define FMS_DATA_STATUSLIST_HPP

#include <FMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(StatusList_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace FMS_Data
{

    /**
     * \class StatusList
     * \brief Implementation of the StatusList class
     */
    class StatusList: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for StatusList
         */
        StatusList();
        /**
         * \brief The destructor for StatusList
         */
        virtual ~StatusList();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the status
         * \return The status attribute value
         **/
        std::vector< ::FMS_Data::Status > const& getStatus() const;
        /**
         * \brief To set the status
         * \param _status The status value
         **/
        void setStatus(std::vector< ::FMS_Data::Status > _status);

        void addStatus(::FMS_Data::Status const& _new_element_in_status);
        void setStatusAt(size_t _position,
                ::FMS_Data::Status const& _new_element_in_status);
        void deleteStatusAt(size_t _position);

        // References


        /*PROTECTED REGION ID(StatusList) START*/
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

        /*PROTECTED REGION ID(StatusListImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        std::vector< ::FMS_Data::Status > m_status;

        // References

    };

} // FMS_Data

#endif // FMS_DATA_STATUSLIST_HPP
