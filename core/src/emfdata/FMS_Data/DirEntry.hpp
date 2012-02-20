// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/DirEntry.hpp
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
 * \file DirEntry.hpp
 * \brief The DirEntry class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef FMS_DATA_DIRENTRY_HPP
#define FMS_DATA_DIRENTRY_HPP

#include <FMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(DirEntry_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace FMS_Data
{

    /**
     * \class DirEntry
     * \brief Implementation of the DirEntry class
     */
    class DirEntry: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for DirEntry
         */
        DirEntry();
        /**
         * \brief The destructor for DirEntry
         */
        virtual ~DirEntry();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the path
         * \return The path attribute value
         **/
        ::ecore::EString const& getPath() const;
        /**
         * \brief To set the path
         * \param _path The path value
         **/
        void setPath(::ecore::EString const& _path);

        /**
         * \brief To get the owner
         * \return The owner attribute value
         **/
        ::ecore::EString const& getOwner() const;
        /**
         * \brief To set the owner
         * \param _owner The owner value
         **/
        void setOwner(::ecore::EString const& _owner);

        /**
         * \brief To get the group
         * \return The group attribute value
         **/
        ::ecore::EString const& getGroup() const;
        /**
         * \brief To set the group
         * \param _group The group value
         **/
        void setGroup(::ecore::EString const& _group);

        /**
         * \brief To get the perms
         * \return The perms attribute value
         **/
        ::ecore::EInt getPerms() const;
        /**
         * \brief To set the perms
         * \param _perms The perms value
         **/
        void setPerms(::ecore::EInt _perms);

        /**
         * \brief To get the size
         * \return The size attribute value
         **/
        ::ecore::EBigInteger getSize() const;
        /**
         * \brief To set the size
         * \param _size The size value
         **/
        void setSize(::ecore::EBigInteger _size);

        /**
         * \brief To get the creationTime
         * \return The creationTime attribute value
         **/
        ::ecore::EString const& getCreationTime() const;
        /**
         * \brief To set the creationTime
         * \param _creationTime The creationTime value
         **/
        void setCreationTime(::ecore::EString const& _creationTime);

        /**
         * \brief To get the type
         * \return The type attribute value
         **/
        ::FMS_Data::FileType getType() const;
        /**
         * \brief To set the type
         * \param _type The type value
         **/
        void setType(::FMS_Data::FileType _type);

        // References


        /*PROTECTED REGION ID(DirEntry) START*/
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

        /*PROTECTED REGION ID(DirEntryImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_path;

        ::ecore::EString m_owner;

        ::ecore::EString m_group;

        ::ecore::EInt m_perms;

        ::ecore::EBigInteger m_size;

        ::ecore::EString m_creationTime;

        ::FMS_Data::FileType m_type;

        // References

    };

} // FMS_Data

#endif // FMS_DATA_DIRENTRY_HPP
