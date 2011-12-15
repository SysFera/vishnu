// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/FMS_DataFactoryImpl.cpp
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

#include <FMS_Data/FMS_DataFactory.hpp>
#include <FMS_Data/FMS_DataPackage.hpp>
#include <FMS_Data/StringList.hpp>
#include <FMS_Data/FileStat.hpp>
#include <FMS_Data/CpFileOptions.hpp>
#include <FMS_Data/LsDirOptions.hpp>
#include <FMS_Data/MvFileOptions.hpp>
#include <FMS_Data/LsTransferOptions.hpp>
#include <FMS_Data/FileStatList.hpp>
#include <FMS_Data/StopTransferOptions.hpp>
#include <FMS_Data/FileTransfer.hpp>
#include <FMS_Data/FileTransferList.hpp>
#include <FMS_Data/HeadOfFileOptions.hpp>
#include <FMS_Data/TailOfFileOptions.hpp>
#include <FMS_Data/RmFileOptions.hpp>
#include <FMS_Data/CreateDirOptions.hpp>
#include <FMS_Data/DirEntry.hpp>
#include <FMS_Data/DirEntryList.hpp>

#include <ecore.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::FMS_Data;

FMS_DataFactory::FMS_DataFactory()
{
    s_instance.reset(this);
}

::ecore::EObject_ptr FMS_DataFactory::create(::ecore::EClass_ptr _eClass)
{
    switch (_eClass->getClassifierID())
    {
    case FMS_DataPackage::STRINGLIST:
        return createStringList();
    case FMS_DataPackage::FILESTAT:
        return createFileStat();
    case FMS_DataPackage::CPFILEOPTIONS:
        return createCpFileOptions();
    case FMS_DataPackage::LSDIROPTIONS:
        return createLsDirOptions();
    case FMS_DataPackage::MVFILEOPTIONS:
        return createMvFileOptions();
    case FMS_DataPackage::LSTRANSFEROPTIONS:
        return createLsTransferOptions();
    case FMS_DataPackage::FILESTATLIST:
        return createFileStatList();
    case FMS_DataPackage::STOPTRANSFEROPTIONS:
        return createStopTransferOptions();
    case FMS_DataPackage::FILETRANSFER:
        return createFileTransfer();
    case FMS_DataPackage::FILETRANSFERLIST:
        return createFileTransferList();
    case FMS_DataPackage::HEADOFFILEOPTIONS:
        return createHeadOfFileOptions();
    case FMS_DataPackage::TAILOFFILEOPTIONS:
        return createTailOfFileOptions();
    case FMS_DataPackage::RMFILEOPTIONS:
        return createRmFileOptions();
    case FMS_DataPackage::CREATEDIROPTIONS:
        return createCreateDirOptions();
    case FMS_DataPackage::DIRENTRY:
        return createDirEntry();
    case FMS_DataPackage::DIRENTRYLIST:
        return createDirEntryList();
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EJavaObject FMS_DataFactory::createFromString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EString const& _literalValue)
{
    switch (_eDataType->getClassifierID())
    {
    case FMS_DataPackage::FILETYPE:
    {
        ::ecore::EJavaObject _any;
        FMS_DataPackage_ptr _epkg =
                dynamic_cast< ::FMS_Data::FMS_DataPackage_ptr > (getEPackage());
        return _epkg->getFileType()->getEEnumLiteralByLiteral(_literalValue)->getValue();
    }
    case FMS_DataPackage::STATUS:
    {
        ::ecore::EJavaObject _any;
        FMS_DataPackage_ptr _epkg =
                dynamic_cast< ::FMS_Data::FMS_DataPackage_ptr > (getEPackage());
        return _epkg->getStatus()->getEEnumLiteralByLiteral(_literalValue)->getValue();
    }
    case FMS_DataPackage::TRANSFERCOMMAND:
    {
        ::ecore::EJavaObject _any;
        FMS_DataPackage_ptr _epkg =
                dynamic_cast< ::FMS_Data::FMS_DataPackage_ptr > (getEPackage());
        return _epkg->getTransferCommand()->getEEnumLiteralByLiteral(
                _literalValue)->getValue();
    }
    default:
        throw "IllegalArgumentException";
    }
}

::ecore::EString FMS_DataFactory::convertToString(
        ::ecore::EDataType_ptr _eDataType,
        ::ecore::EJavaObject const& _instanceValue)
{
    switch (_eDataType->getClassifierID())
    {
    case FMS_DataPackage::FILETYPE:
    {
        FMS_DataPackage_ptr _epkg = ::FMS_Data::instanceOf<
                ::FMS_Data::FMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getFileType()->getEEnumLiteral(_value)->getName();
    }
    case FMS_DataPackage::STATUS:
    {
        FMS_DataPackage_ptr _epkg = ::FMS_Data::instanceOf<
                ::FMS_Data::FMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getStatus()->getEEnumLiteral(_value)->getName();
    }
    case FMS_DataPackage::TRANSFERCOMMAND:
    {
        FMS_DataPackage_ptr _epkg = ::FMS_Data::instanceOf<
                ::FMS_Data::FMS_DataPackage >(getEPackage());
        ::ecore::EInt _value = ::ecorecpp::mapping::any::any_cast<
                ::ecore::EInt >(_instanceValue);
        return _epkg->getTransferCommand()->getEEnumLiteral(_value)->getName();
    }
    default:
        throw "IllegalArgumentException";
    }
}

StringList_ptr FMS_DataFactory::createStringList()
{
    return new StringList();
}
FileStat_ptr FMS_DataFactory::createFileStat()
{
    return new FileStat();
}
CpFileOptions_ptr FMS_DataFactory::createCpFileOptions()
{
    return new CpFileOptions();
}
LsDirOptions_ptr FMS_DataFactory::createLsDirOptions()
{
    return new LsDirOptions();
}
MvFileOptions_ptr FMS_DataFactory::createMvFileOptions()
{
    return new MvFileOptions();
}
LsTransferOptions_ptr FMS_DataFactory::createLsTransferOptions()
{
    return new LsTransferOptions();
}
FileStatList_ptr FMS_DataFactory::createFileStatList()
{
    return new FileStatList();
}
StopTransferOptions_ptr FMS_DataFactory::createStopTransferOptions()
{
    return new StopTransferOptions();
}
FileTransfer_ptr FMS_DataFactory::createFileTransfer()
{
    return new FileTransfer();
}
FileTransferList_ptr FMS_DataFactory::createFileTransferList()
{
    return new FileTransferList();
}
HeadOfFileOptions_ptr FMS_DataFactory::createHeadOfFileOptions()
{
    return new HeadOfFileOptions();
}
TailOfFileOptions_ptr FMS_DataFactory::createTailOfFileOptions()
{
    return new TailOfFileOptions();
}
RmFileOptions_ptr FMS_DataFactory::createRmFileOptions()
{
    return new RmFileOptions();
}
CreateDirOptions_ptr FMS_DataFactory::createCreateDirOptions()
{
    return new CreateDirOptions();
}
DirEntry_ptr FMS_DataFactory::createDirEntry()
{
    return new DirEntry();
}
DirEntryList_ptr FMS_DataFactory::createDirEntryList()
{
    return new DirEntryList();
}

