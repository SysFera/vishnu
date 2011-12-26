// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data_forward.hpp
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

#ifndef _FMS_DATA_FORWARD_HPP
#define _FMS_DATA_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(FMS_Data_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage


#include <ecore_forward.hpp> // for EDataTypes
namespace FMS_Data
{

    // EDataType
    /*PROTECTED REGION ID(FMS_Data_FileType) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    typedef int FileType;
    /*PROTECTED REGION END*/
    /*PROTECTED REGION ID(FMS_Data_Status) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    typedef int Status;
    /*PROTECTED REGION END*/
    /*PROTECTED REGION ID(FMS_Data_TransferCommand) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    typedef int TransferCommand;
    /*PROTECTED REGION END*/

    // EClass

    // FileStat
    class FileStat;
    typedef FileStat* FileStat_ptr;

    // CpFileOptions
    class CpFileOptions;
    typedef CpFileOptions* CpFileOptions_ptr;

    // LsDirOptions
    class LsDirOptions;
    typedef LsDirOptions* LsDirOptions_ptr;

    // MvFileOptions
    class MvFileOptions;
    typedef MvFileOptions* MvFileOptions_ptr;

    // LsTransferOptions
    class LsTransferOptions;
    typedef LsTransferOptions* LsTransferOptions_ptr;

    // StopTransferOptions
    class StopTransferOptions;
    typedef StopTransferOptions* StopTransferOptions_ptr;

    // FileTransfer
    class FileTransfer;
    typedef FileTransfer* FileTransfer_ptr;

    // FileTransferList
    class FileTransferList;
    typedef FileTransferList* FileTransferList_ptr;

    // HeadOfFileOptions
    class HeadOfFileOptions;
    typedef HeadOfFileOptions* HeadOfFileOptions_ptr;

    // TailOfFileOptions
    class TailOfFileOptions;
    typedef TailOfFileOptions* TailOfFileOptions_ptr;

    // RmFileOptions
    class RmFileOptions;
    typedef RmFileOptions* RmFileOptions_ptr;

    // CreateDirOptions
    class CreateDirOptions;
    typedef CreateDirOptions* CreateDirOptions_ptr;

    // DirEntry
    class DirEntry;
    typedef DirEntry* DirEntry_ptr;

    // DirEntryList
    class DirEntryList;
    typedef DirEntryList* DirEntryList_ptr;

    // Package & Factory
    class FMS_DataFactory;
    typedef FMS_DataFactory * FMS_DataFactory_ptr;
    class FMS_DataPackage;
    typedef FMS_DataPackage * FMS_DataPackage_ptr;

    template< typename T, typename S >
    inline T* instanceOf(S* _s)
    {
        return dynamic_cast< T* > (_s);
    }

} // FMS_Data


#endif // _FMS_DATA_FORWARD_HPP
