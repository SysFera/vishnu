// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/FMS_DataFactory.hpp
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
 * \file FMS_DataFactory.hpp
 * \brief The FMS_DataFactory class
 * \author Generated file
 * \date 31/03/2011
 */
#ifndef _FMS_DATAFACTORY_HPP
#define _FMS_DATAFACTORY_HPP

#include <ecore/EFactory.hpp>
#include <FMS_Data.hpp>
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
namespace FMS_Data
{
    /**
     * \class FMS_DataFactory
     * \brief The factory class for generated model FMS_Data
     */
    class VISHNU_API_LIB FMS_DataFactory: public virtual ::ecore::EFactory
    {
    public:

        /**
         * \brief To get the instance of the singleton
         * \return Pointer to the instance
         */
        static FMS_DataFactory_ptr _instance();

        /**
         * \brief Creates a new object of class FileStat
         * \return Pointer to the object
         */
        virtual FileStat_ptr createFileStat();
        /**
         * \brief Creates a new object of class CpFileOptions
         * \return Pointer to the object
         */
        virtual CpFileOptions_ptr createCpFileOptions();
        /**
         * \brief Creates a new object of class LsDirOptions
         * \return Pointer to the object
         */
        virtual LsDirOptions_ptr createLsDirOptions();
        /**
         * \brief Creates a new object of class MvFileOptions
         * \return Pointer to the object
         */
        virtual MvFileOptions_ptr createMvFileOptions();
        /**
         * \brief Creates a new object of class LsTransferOptions
         * \return Pointer to the object
         */
        virtual LsTransferOptions_ptr createLsTransferOptions();
        /**
         * \brief Creates a new object of class StopTransferOptions
         * \return Pointer to the object
         */
        virtual StopTransferOptions_ptr createStopTransferOptions();
        /**
         * \brief Creates a new object of class FileTransfer
         * \return Pointer to the object
         */
        virtual FileTransfer_ptr createFileTransfer();
        /**
         * \brief Creates a new object of class FileTransferList
         * \return Pointer to the object
         */
        virtual FileTransferList_ptr createFileTransferList();
        /**
         * \brief Creates a new object of class HeadOfFileOptions
         * \return Pointer to the object
         */
        virtual HeadOfFileOptions_ptr createHeadOfFileOptions();
        /**
         * \brief Creates a new object of class TailOfFileOptions
         * \return Pointer to the object
         */
        virtual TailOfFileOptions_ptr createTailOfFileOptions();
        /**
         * \brief Creates a new object of class RmFileOptions
         * \return Pointer to the object
         */
        virtual RmFileOptions_ptr createRmFileOptions();
        /**
         * \brief Creates a new object of class CreateDirOptions
         * \return Pointer to the object
         */
        virtual CreateDirOptions_ptr createCreateDirOptions();
        /**
         * \brief Creates a new object of class DirEntry
         * \return Pointer to the object
         */
        virtual DirEntry_ptr createDirEntry();
        /**
         * \brief Creates a new object of class DirEntryList
         * \return Pointer to the object
         */
        virtual DirEntryList_ptr createDirEntryList();

        /**
         * \brief To create an object of a given class (reflective API)
         * \param _eClass The class object to create (obtained using the Package class)
         * \return Generic pointer to the object
         */
        virtual ::ecore::EObject_ptr create(::ecore::EClass_ptr _eClass);

        /**
         * \brief To create an object from a string (deserialize)
         * \param _eDataType The datatype
         * \param _literalValue The string
         * \return Generic pointer to the object
         */
        virtual ::ecore::EJavaObject createFromString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EString const& _literalValue);

        /**
         * \brief To convert an object to a string (serialize)
         * \param _eDataType The datatype
         * \param _instanceValue Value of the instance
         * \return The object serialized in a string
         */
        virtual ::ecore::EString convertToString(
                ::ecore::EDataType_ptr _eDataType,
                ::ecore::EJavaObject const& _instanceValue);

    protected:

        /**
         * \brief The instance of the singleton
         */
        static std::auto_ptr< FMS_DataFactory > s_instance;

        /**
         * \brief Constructor
         */
        FMS_DataFactory();

    };

} // FMS_Data


#endif // _FMS_DATAFACTORY_HPP
