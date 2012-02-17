// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/SystemInfo.hpp
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
 * \file SystemInfo.hpp
 * \brief The SystemInfo class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef IMS_DATA_SYSTEMINFO_HPP
#define IMS_DATA_SYSTEMINFO_HPP

#include <IMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
/*PROTECTED REGION ID(SystemInfo_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace IMS_Data
{

    /**
     * \class SystemInfo
     * \brief Implementation of the SystemInfo class
     */
    class VISHNU_API_LIB SystemInfo: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for SystemInfo
         */
        SystemInfo();
        /**
         * \brief The destructor for SystemInfo
         */
        virtual ~SystemInfo();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the memory
         * \return The memory attribute value
         **/
        ::ecore::ELong getMemory() const;
        /**
         * \brief To set the memory
         * \param _memory The memory value
         **/
        void setMemory(::ecore::ELong _memory);

        /**
         * \brief To get the diskSpace
         * \return The diskSpace attribute value
         **/
        ::ecore::ELong getDiskSpace() const;
        /**
         * \brief To set the diskSpace
         * \param _diskSpace The diskSpace value
         **/
        void setDiskSpace(::ecore::ELong _diskSpace);

        /**
         * \brief To get the machineId
         * \return The machineId attribute value
         **/
        ::ecore::EString const& getMachineId() const;
        /**
         * \brief To set the machineId
         * \param _machineId The machineId value
         **/
        void setMachineId(::ecore::EString const& _machineId);

        // References


        /*PROTECTED REGION ID(SystemInfo) START*/
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

        /*PROTECTED REGION ID(SystemInfoImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::ELong m_memory;

        ::ecore::ELong m_diskSpace;

        ::ecore::EString m_machineId;

        // References

    };

} // IMS_Data

#endif // IMS_DATA_SYSTEMINFO_HPP
