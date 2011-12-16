// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/FMS_DataPackageImpl.cpp
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

#include <FMS_Data/FMS_DataPackage.hpp>
#include <FMS_Data/FMS_DataFactory.hpp>
#include <ecore.hpp>
#include <ecore/EcorePackage.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EAttribute.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EOperation.hpp>
#include <ecore/EParameter.hpp>
#include <ecore/EEnum.hpp>
#include <ecore/EEnumLiteral.hpp>
#include <ecore/EDataType.hpp>
#include <ecore/EGenericType.hpp>
#include <ecore/ETypeParameter.hpp>
#include <ecore/EcorePackage.hpp>
#include <ecore/EcorePackage.hpp>

using namespace ::FMS_Data;

FMS_DataPackage::FMS_DataPackage()
{

    s_instance.reset(this);

    // Factory
    ::ecore::EFactory_ptr _fa = FMS_DataFactory::_instance();
    setEFactoryInstance(_fa);
    _fa->setEPackage(this);

    // Create classes and their features

    // FileStat
    m_FileStatEClass = new ::ecore::EClass();
    m_FileStatEClass->setClassifierID(FILESTAT);
    m_FileStatEClass->setEPackage(this);
    getEClassifiers().push_back(m_FileStatEClass);
    m_FileStat__path = new ::ecore::EAttribute();
    m_FileStat__path->setFeatureID(::FMS_Data::FMS_DataPackage::FILESTAT__PATH);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__path);
    m_FileStat__owner = new ::ecore::EAttribute();
    m_FileStat__owner->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILESTAT__OWNER);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__owner);
    m_FileStat__group = new ::ecore::EAttribute();
    m_FileStat__group->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILESTAT__GROUP);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__group);
    m_FileStat__perms = new ::ecore::EAttribute();
    m_FileStat__perms->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILESTAT__PERMS);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__perms);
    m_FileStat__uid = new ::ecore::EAttribute();
    m_FileStat__uid->setFeatureID(::FMS_Data::FMS_DataPackage::FILESTAT__UID);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__uid);
    m_FileStat__gid = new ::ecore::EAttribute();
    m_FileStat__gid->setFeatureID(::FMS_Data::FMS_DataPackage::FILESTAT__GID);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__gid);
    m_FileStat__size = new ::ecore::EAttribute();
    m_FileStat__size->setFeatureID(::FMS_Data::FMS_DataPackage::FILESTAT__SIZE);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__size);
    m_FileStat__atime = new ::ecore::EAttribute();
    m_FileStat__atime->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILESTAT__ATIME);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__atime);
    m_FileStat__mtime = new ::ecore::EAttribute();
    m_FileStat__mtime->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILESTAT__MTIME);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__mtime);
    m_FileStat__ctime = new ::ecore::EAttribute();
    m_FileStat__ctime->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILESTAT__CTIME);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__ctime);
    m_FileStat__type = new ::ecore::EAttribute();
    m_FileStat__type->setFeatureID(::FMS_Data::FMS_DataPackage::FILESTAT__TYPE);
    m_FileStatEClass->getEStructuralFeatures().push_back(m_FileStat__type);

    // CpFileOptions
    m_CpFileOptionsEClass = new ::ecore::EClass();
    m_CpFileOptionsEClass->setClassifierID(CPFILEOPTIONS);
    m_CpFileOptionsEClass->setEPackage(this);
    getEClassifiers().push_back(m_CpFileOptionsEClass);
    m_CpFileOptions__isRecursive = new ::ecore::EAttribute();
    m_CpFileOptions__isRecursive->setFeatureID(
            ::FMS_Data::FMS_DataPackage::CPFILEOPTIONS__ISRECURSIVE);
    m_CpFileOptionsEClass->getEStructuralFeatures().push_back(
            m_CpFileOptions__isRecursive);
    m_CpFileOptions__trCommand = new ::ecore::EAttribute();
    m_CpFileOptions__trCommand->setFeatureID(
            ::FMS_Data::FMS_DataPackage::CPFILEOPTIONS__TRCOMMAND);
    m_CpFileOptionsEClass->getEStructuralFeatures().push_back(
            m_CpFileOptions__trCommand);

    // LsDirOptions
    m_LsDirOptionsEClass = new ::ecore::EClass();
    m_LsDirOptionsEClass->setClassifierID(LSDIROPTIONS);
    m_LsDirOptionsEClass->setEPackage(this);
    getEClassifiers().push_back(m_LsDirOptionsEClass);
    m_LsDirOptions__longFormat = new ::ecore::EAttribute();
    m_LsDirOptions__longFormat->setFeatureID(
            ::FMS_Data::FMS_DataPackage::LSDIROPTIONS__LONGFORMAT);
    m_LsDirOptionsEClass->getEStructuralFeatures().push_back(
            m_LsDirOptions__longFormat);
    m_LsDirOptions__allFiles = new ::ecore::EAttribute();
    m_LsDirOptions__allFiles->setFeatureID(
            ::FMS_Data::FMS_DataPackage::LSDIROPTIONS__ALLFILES);
    m_LsDirOptionsEClass->getEStructuralFeatures().push_back(
            m_LsDirOptions__allFiles);

    // MvFileOptions
    m_MvFileOptionsEClass = new ::ecore::EClass();
    m_MvFileOptionsEClass->setClassifierID(MVFILEOPTIONS);
    m_MvFileOptionsEClass->setEPackage(this);
    getEClassifiers().push_back(m_MvFileOptionsEClass);
    m_MvFileOptions__trCommand = new ::ecore::EAttribute();
    m_MvFileOptions__trCommand->setFeatureID(
            ::FMS_Data::FMS_DataPackage::MVFILEOPTIONS__TRCOMMAND);
    m_MvFileOptionsEClass->getEStructuralFeatures().push_back(
            m_MvFileOptions__trCommand);

    // LsTransferOptions
    m_LsTransferOptionsEClass = new ::ecore::EClass();
    m_LsTransferOptionsEClass->setClassifierID(LSTRANSFEROPTIONS);
    m_LsTransferOptionsEClass->setEPackage(this);
    getEClassifiers().push_back(m_LsTransferOptionsEClass);
    m_LsTransferOptions__transferId = new ::ecore::EAttribute();
    m_LsTransferOptions__transferId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__TRANSFERID);
    m_LsTransferOptionsEClass->getEStructuralFeatures().push_back(
            m_LsTransferOptions__transferId);
    m_LsTransferOptions__fromMachineId = new ::ecore::EAttribute();
    m_LsTransferOptions__fromMachineId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__FROMMACHINEID);
    m_LsTransferOptionsEClass->getEStructuralFeatures().push_back(
            m_LsTransferOptions__fromMachineId);
    m_LsTransferOptions__userId = new ::ecore::EAttribute();
    m_LsTransferOptions__userId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__USERID);
    m_LsTransferOptionsEClass->getEStructuralFeatures().push_back(
            m_LsTransferOptions__userId);
    m_LsTransferOptions__status = new ::ecore::EAttribute();
    m_LsTransferOptions__status->setFeatureID(
            ::FMS_Data::FMS_DataPackage::LSTRANSFEROPTIONS__STATUS);
    m_LsTransferOptionsEClass->getEStructuralFeatures().push_back(
            m_LsTransferOptions__status);

    // FileStatList
    m_FileStatListEClass = new ::ecore::EClass();
    m_FileStatListEClass->setClassifierID(FILESTATLIST);
    m_FileStatListEClass->setEPackage(this);
    getEClassifiers().push_back(m_FileStatListEClass);
    m_FileStatList__fileStats = new ::ecore::EReference();
    m_FileStatList__fileStats->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILESTATLIST__FILESTATS);
    m_FileStatListEClass->getEStructuralFeatures().push_back(
            m_FileStatList__fileStats);

    // StopTransferOptions
    m_StopTransferOptionsEClass = new ::ecore::EClass();
    m_StopTransferOptionsEClass->setClassifierID(STOPTRANSFEROPTIONS);
    m_StopTransferOptionsEClass->setEPackage(this);
    getEClassifiers().push_back(m_StopTransferOptionsEClass);
    m_StopTransferOptions__transferId = new ::ecore::EAttribute();
    m_StopTransferOptions__transferId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::STOPTRANSFEROPTIONS__TRANSFERID);
    m_StopTransferOptionsEClass->getEStructuralFeatures().push_back(
            m_StopTransferOptions__transferId);
    m_StopTransferOptions__fromMachineId = new ::ecore::EAttribute();
    m_StopTransferOptions__fromMachineId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::STOPTRANSFEROPTIONS__FROMMACHINEID);
    m_StopTransferOptionsEClass->getEStructuralFeatures().push_back(
            m_StopTransferOptions__fromMachineId);
    m_StopTransferOptions__userId = new ::ecore::EAttribute();
    m_StopTransferOptions__userId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::STOPTRANSFEROPTIONS__USERID);
    m_StopTransferOptionsEClass->getEStructuralFeatures().push_back(
            m_StopTransferOptions__userId);

    // FileTransfer
    m_FileTransferEClass = new ::ecore::EClass();
    m_FileTransferEClass->setClassifierID(FILETRANSFER);
    m_FileTransferEClass->setEPackage(this);
    getEClassifiers().push_back(m_FileTransferEClass);
    m_FileTransfer__transferId = new ::ecore::EAttribute();
    m_FileTransfer__transferId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__TRANSFERID);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__transferId);
    m_FileTransfer__status = new ::ecore::EAttribute();
    m_FileTransfer__status->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__STATUS);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__status);
    m_FileTransfer__userId = new ::ecore::EAttribute();
    m_FileTransfer__userId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__USERID);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__userId);
    m_FileTransfer__clientMachineId = new ::ecore::EAttribute();
    m_FileTransfer__clientMachineId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__CLIENTMACHINEID);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__clientMachineId);
    m_FileTransfer__sourceMachineId = new ::ecore::EAttribute();
    m_FileTransfer__sourceMachineId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__SOURCEMACHINEID);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__sourceMachineId);
    m_FileTransfer__destinationMachineId = new ::ecore::EAttribute();
    m_FileTransfer__destinationMachineId->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__DESTINATIONMACHINEID);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__destinationMachineId);
    m_FileTransfer__sourceFilePath = new ::ecore::EAttribute();
    m_FileTransfer__sourceFilePath->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__SOURCEFILEPATH);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__sourceFilePath);
    m_FileTransfer__destinationFilePath = new ::ecore::EAttribute();
    m_FileTransfer__destinationFilePath->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__DESTINATIONFILEPATH);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__destinationFilePath);
    m_FileTransfer__size = new ::ecore::EAttribute();
    m_FileTransfer__size->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__SIZE);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__size);
    m_FileTransfer__start_time = new ::ecore::EAttribute();
    m_FileTransfer__start_time->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__START_TIME);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__start_time);
    m_FileTransfer__trCommand = new ::ecore::EAttribute();
    m_FileTransfer__trCommand->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__TRCOMMAND);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__trCommand);
    m_FileTransfer__errorMsg = new ::ecore::EAttribute();
    m_FileTransfer__errorMsg->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFER__ERRORMSG);
    m_FileTransferEClass->getEStructuralFeatures().push_back(
            m_FileTransfer__errorMsg);

    // FileTransferList
    m_FileTransferListEClass = new ::ecore::EClass();
    m_FileTransferListEClass->setClassifierID(FILETRANSFERLIST);
    m_FileTransferListEClass->setEPackage(this);
    getEClassifiers().push_back(m_FileTransferListEClass);
    m_FileTransferList__fileTransfers = new ::ecore::EReference();
    m_FileTransferList__fileTransfers->setFeatureID(
            ::FMS_Data::FMS_DataPackage::FILETRANSFERLIST__FILETRANSFERS);
    m_FileTransferListEClass->getEStructuralFeatures().push_back(
            m_FileTransferList__fileTransfers);

    // HeadOfFileOptions
    m_HeadOfFileOptionsEClass = new ::ecore::EClass();
    m_HeadOfFileOptionsEClass->setClassifierID(HEADOFFILEOPTIONS);
    m_HeadOfFileOptionsEClass->setEPackage(this);
    getEClassifiers().push_back(m_HeadOfFileOptionsEClass);
    m_HeadOfFileOptions__nline = new ::ecore::EAttribute();
    m_HeadOfFileOptions__nline->setFeatureID(
            ::FMS_Data::FMS_DataPackage::HEADOFFILEOPTIONS__NLINE);
    m_HeadOfFileOptionsEClass->getEStructuralFeatures().push_back(
            m_HeadOfFileOptions__nline);

    // TailOfFileOptions
    m_TailOfFileOptionsEClass = new ::ecore::EClass();
    m_TailOfFileOptionsEClass->setClassifierID(TAILOFFILEOPTIONS);
    m_TailOfFileOptionsEClass->setEPackage(this);
    getEClassifiers().push_back(m_TailOfFileOptionsEClass);
    m_TailOfFileOptions__nline = new ::ecore::EAttribute();
    m_TailOfFileOptions__nline->setFeatureID(
            ::FMS_Data::FMS_DataPackage::TAILOFFILEOPTIONS__NLINE);
    m_TailOfFileOptionsEClass->getEStructuralFeatures().push_back(
            m_TailOfFileOptions__nline);

    // RmFileOptions
    m_RmFileOptionsEClass = new ::ecore::EClass();
    m_RmFileOptionsEClass->setClassifierID(RMFILEOPTIONS);
    m_RmFileOptionsEClass->setEPackage(this);
    getEClassifiers().push_back(m_RmFileOptionsEClass);
    m_RmFileOptions__isRecursive = new ::ecore::EAttribute();
    m_RmFileOptions__isRecursive->setFeatureID(
            ::FMS_Data::FMS_DataPackage::RMFILEOPTIONS__ISRECURSIVE);
    m_RmFileOptionsEClass->getEStructuralFeatures().push_back(
            m_RmFileOptions__isRecursive);

    // CreateDirOptions
    m_CreateDirOptionsEClass = new ::ecore::EClass();
    m_CreateDirOptionsEClass->setClassifierID(CREATEDIROPTIONS);
    m_CreateDirOptionsEClass->setEPackage(this);
    getEClassifiers().push_back(m_CreateDirOptionsEClass);
    m_CreateDirOptions__isRecursive = new ::ecore::EAttribute();
    m_CreateDirOptions__isRecursive->setFeatureID(
            ::FMS_Data::FMS_DataPackage::CREATEDIROPTIONS__ISRECURSIVE);
    m_CreateDirOptionsEClass->getEStructuralFeatures().push_back(
            m_CreateDirOptions__isRecursive);

    // DirEntry
    m_DirEntryEClass = new ::ecore::EClass();
    m_DirEntryEClass->setClassifierID(DIRENTRY);
    m_DirEntryEClass->setEPackage(this);
    getEClassifiers().push_back(m_DirEntryEClass);
    m_DirEntry__path = new ::ecore::EAttribute();
    m_DirEntry__path->setFeatureID(::FMS_Data::FMS_DataPackage::DIRENTRY__PATH);
    m_DirEntryEClass->getEStructuralFeatures().push_back(m_DirEntry__path);
    m_DirEntry__owner = new ::ecore::EAttribute();
    m_DirEntry__owner->setFeatureID(
            ::FMS_Data::FMS_DataPackage::DIRENTRY__OWNER);
    m_DirEntryEClass->getEStructuralFeatures().push_back(m_DirEntry__owner);
    m_DirEntry__group = new ::ecore::EAttribute();
    m_DirEntry__group->setFeatureID(
            ::FMS_Data::FMS_DataPackage::DIRENTRY__GROUP);
    m_DirEntryEClass->getEStructuralFeatures().push_back(m_DirEntry__group);
    m_DirEntry__perms = new ::ecore::EAttribute();
    m_DirEntry__perms->setFeatureID(
            ::FMS_Data::FMS_DataPackage::DIRENTRY__PERMS);
    m_DirEntryEClass->getEStructuralFeatures().push_back(m_DirEntry__perms);
    m_DirEntry__size = new ::ecore::EAttribute();
    m_DirEntry__size->setFeatureID(::FMS_Data::FMS_DataPackage::DIRENTRY__SIZE);
    m_DirEntryEClass->getEStructuralFeatures().push_back(m_DirEntry__size);
    m_DirEntry__creationTime = new ::ecore::EAttribute();
    m_DirEntry__creationTime->setFeatureID(
            ::FMS_Data::FMS_DataPackage::DIRENTRY__CREATIONTIME);
    m_DirEntryEClass->getEStructuralFeatures().push_back(
            m_DirEntry__creationTime);
    m_DirEntry__type = new ::ecore::EAttribute();
    m_DirEntry__type->setFeatureID(::FMS_Data::FMS_DataPackage::DIRENTRY__TYPE);
    m_DirEntryEClass->getEStructuralFeatures().push_back(m_DirEntry__type);

    // DirEntryList
    m_DirEntryListEClass = new ::ecore::EClass();
    m_DirEntryListEClass->setClassifierID(DIRENTRYLIST);
    m_DirEntryListEClass->setEPackage(this);
    getEClassifiers().push_back(m_DirEntryListEClass);
    m_DirEntryList__dirEntries = new ::ecore::EReference();
    m_DirEntryList__dirEntries->setFeatureID(
            ::FMS_Data::FMS_DataPackage::DIRENTRYLIST__DIRENTRIES);
    m_DirEntryListEClass->getEStructuralFeatures().push_back(
            m_DirEntryList__dirEntries);

    // Create enums

    m_FileTypeEEnum = new ::ecore::EEnum();
    m_FileTypeEEnum->setClassifierID(FILETYPE);
    m_FileTypeEEnum->setEPackage(this);
    getEClassifiers().push_back(m_FileTypeEEnum);

    m_StatusEEnum = new ::ecore::EEnum();
    m_StatusEEnum->setClassifierID(STATUS);
    m_StatusEEnum->setEPackage(this);
    getEClassifiers().push_back(m_StatusEEnum);

    m_TransferCommandEEnum = new ::ecore::EEnum();
    m_TransferCommandEEnum->setClassifierID(TRANSFERCOMMAND);
    m_TransferCommandEEnum->setEPackage(this);
    getEClassifiers().push_back(m_TransferCommandEEnum);

    // Create data types


    // Initialize package
    setName("FMS_Data");
    setNsPrefix("FMS_Data");
    setNsURI("http://www.sysfera.com/emf/fms/data");

    // TODO: bounds for type parameters

    // Add supertypes to classes

    // TODO: Initialize classes and features; add operations and parameters
    // TODO: GenericTypes
    // FileStat
    m_FileStatEClass->setName("FileStat");
    m_FileStatEClass->setAbstract(false);
    m_FileStatEClass->setInterface(false);
    m_FileStat__path->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileStat__path->setName("path");
    m_FileStat__path->setDefaultValueLiteral("");
    m_FileStat__path->setLowerBound(0);
    m_FileStat__path->setUpperBound(1);
    m_FileStat__path->setTransient(false);
    m_FileStat__path->setVolatile(false);
    m_FileStat__path->setChangeable(true);
    m_FileStat__path->setUnsettable(false);
    m_FileStat__path->setID(false);
    m_FileStat__path->setUnique(true);
    m_FileStat__path->setDerived(false);
    m_FileStat__path->setOrdered(true);
    m_FileStat__owner->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileStat__owner->setName("owner");
    m_FileStat__owner->setDefaultValueLiteral("");
    m_FileStat__owner->setLowerBound(0);
    m_FileStat__owner->setUpperBound(1);
    m_FileStat__owner->setTransient(false);
    m_FileStat__owner->setVolatile(false);
    m_FileStat__owner->setChangeable(true);
    m_FileStat__owner->setUnsettable(false);
    m_FileStat__owner->setID(false);
    m_FileStat__owner->setUnique(true);
    m_FileStat__owner->setDerived(false);
    m_FileStat__owner->setOrdered(true);
    m_FileStat__group->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileStat__group->setName("group");
    m_FileStat__group->setDefaultValueLiteral("");
    m_FileStat__group->setLowerBound(0);
    m_FileStat__group->setUpperBound(1);
    m_FileStat__group->setTransient(false);
    m_FileStat__group->setVolatile(false);
    m_FileStat__group->setChangeable(true);
    m_FileStat__group->setUnsettable(false);
    m_FileStat__group->setID(false);
    m_FileStat__group->setUnique(true);
    m_FileStat__group->setDerived(false);
    m_FileStat__group->setOrdered(true);
    m_FileStat__perms->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEInt());
    m_FileStat__perms->setName("perms");
    m_FileStat__perms->setDefaultValueLiteral("");
    m_FileStat__perms->setLowerBound(0);
    m_FileStat__perms->setUpperBound(1);
    m_FileStat__perms->setTransient(false);
    m_FileStat__perms->setVolatile(false);
    m_FileStat__perms->setChangeable(true);
    m_FileStat__perms->setUnsettable(false);
    m_FileStat__perms->setID(false);
    m_FileStat__perms->setUnique(true);
    m_FileStat__perms->setDerived(false);
    m_FileStat__perms->setOrdered(true);
    m_FileStat__uid->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_FileStat__uid->setName("uid");
    m_FileStat__uid->setDefaultValueLiteral("");
    m_FileStat__uid->setLowerBound(0);
    m_FileStat__uid->setUpperBound(1);
    m_FileStat__uid->setTransient(false);
    m_FileStat__uid->setVolatile(false);
    m_FileStat__uid->setChangeable(true);
    m_FileStat__uid->setUnsettable(false);
    m_FileStat__uid->setID(false);
    m_FileStat__uid->setUnique(true);
    m_FileStat__uid->setDerived(false);
    m_FileStat__uid->setOrdered(true);
    m_FileStat__gid->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_FileStat__gid->setName("gid");
    m_FileStat__gid->setDefaultValueLiteral("");
    m_FileStat__gid->setLowerBound(0);
    m_FileStat__gid->setUpperBound(1);
    m_FileStat__gid->setTransient(false);
    m_FileStat__gid->setVolatile(false);
    m_FileStat__gid->setChangeable(true);
    m_FileStat__gid->setUnsettable(false);
    m_FileStat__gid->setID(false);
    m_FileStat__gid->setUnique(true);
    m_FileStat__gid->setDerived(false);
    m_FileStat__gid->setOrdered(true);
    m_FileStat__size->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_FileStat__size->setName("size");
    m_FileStat__size->setDefaultValueLiteral("");
    m_FileStat__size->setLowerBound(0);
    m_FileStat__size->setUpperBound(1);
    m_FileStat__size->setTransient(false);
    m_FileStat__size->setVolatile(false);
    m_FileStat__size->setChangeable(true);
    m_FileStat__size->setUnsettable(false);
    m_FileStat__size->setID(false);
    m_FileStat__size->setUnique(true);
    m_FileStat__size->setDerived(false);
    m_FileStat__size->setOrdered(true);
    m_FileStat__atime->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_FileStat__atime->setName("atime");
    m_FileStat__atime->setDefaultValueLiteral("");
    m_FileStat__atime->setLowerBound(0);
    m_FileStat__atime->setUpperBound(1);
    m_FileStat__atime->setTransient(false);
    m_FileStat__atime->setVolatile(false);
    m_FileStat__atime->setChangeable(true);
    m_FileStat__atime->setUnsettable(false);
    m_FileStat__atime->setID(false);
    m_FileStat__atime->setUnique(true);
    m_FileStat__atime->setDerived(false);
    m_FileStat__atime->setOrdered(true);
    m_FileStat__mtime->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_FileStat__mtime->setName("mtime");
    m_FileStat__mtime->setDefaultValueLiteral("");
    m_FileStat__mtime->setLowerBound(0);
    m_FileStat__mtime->setUpperBound(1);
    m_FileStat__mtime->setTransient(false);
    m_FileStat__mtime->setVolatile(false);
    m_FileStat__mtime->setChangeable(true);
    m_FileStat__mtime->setUnsettable(false);
    m_FileStat__mtime->setID(false);
    m_FileStat__mtime->setUnique(true);
    m_FileStat__mtime->setDerived(false);
    m_FileStat__mtime->setOrdered(true);
    m_FileStat__ctime->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_FileStat__ctime->setName("ctime");
    m_FileStat__ctime->setDefaultValueLiteral("");
    m_FileStat__ctime->setLowerBound(0);
    m_FileStat__ctime->setUpperBound(1);
    m_FileStat__ctime->setTransient(false);
    m_FileStat__ctime->setVolatile(false);
    m_FileStat__ctime->setChangeable(true);
    m_FileStat__ctime->setUnsettable(false);
    m_FileStat__ctime->setID(false);
    m_FileStat__ctime->setUnique(true);
    m_FileStat__ctime->setDerived(false);
    m_FileStat__ctime->setOrdered(true);
    m_FileStat__type->setEType(m_FileTypeEEnum);
    m_FileStat__type->setName("type");
    m_FileStat__type->setDefaultValueLiteral("6");
    m_FileStat__type->setLowerBound(0);
    m_FileStat__type->setUpperBound(1);
    m_FileStat__type->setTransient(false);
    m_FileStat__type->setVolatile(false);
    m_FileStat__type->setChangeable(true);
    m_FileStat__type->setUnsettable(false);
    m_FileStat__type->setID(false);
    m_FileStat__type->setUnique(true);
    m_FileStat__type->setDerived(false);
    m_FileStat__type->setOrdered(true);
    // CpFileOptions
    m_CpFileOptionsEClass->setName("CpFileOptions");
    m_CpFileOptionsEClass->setAbstract(false);
    m_CpFileOptionsEClass->setInterface(false);
    m_CpFileOptions__isRecursive->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEBoolean());
    m_CpFileOptions__isRecursive->setName("isRecursive");
    m_CpFileOptions__isRecursive->setDefaultValueLiteral("false");
    m_CpFileOptions__isRecursive->setLowerBound(0);
    m_CpFileOptions__isRecursive->setUpperBound(1);
    m_CpFileOptions__isRecursive->setTransient(false);
    m_CpFileOptions__isRecursive->setVolatile(false);
    m_CpFileOptions__isRecursive->setChangeable(true);
    m_CpFileOptions__isRecursive->setUnsettable(false);
    m_CpFileOptions__isRecursive->setID(false);
    m_CpFileOptions__isRecursive->setUnique(true);
    m_CpFileOptions__isRecursive->setDerived(false);
    m_CpFileOptions__isRecursive->setOrdered(true);
    m_CpFileOptions__trCommand->setEType(m_TransferCommandEEnum);
    m_CpFileOptions__trCommand->setName("trCommand");
    m_CpFileOptions__trCommand->setDefaultValueLiteral("2");
    m_CpFileOptions__trCommand->setLowerBound(0);
    m_CpFileOptions__trCommand->setUpperBound(1);
    m_CpFileOptions__trCommand->setTransient(false);
    m_CpFileOptions__trCommand->setVolatile(false);
    m_CpFileOptions__trCommand->setChangeable(true);
    m_CpFileOptions__trCommand->setUnsettable(false);
    m_CpFileOptions__trCommand->setID(false);
    m_CpFileOptions__trCommand->setUnique(true);
    m_CpFileOptions__trCommand->setDerived(false);
    m_CpFileOptions__trCommand->setOrdered(true);
    // LsDirOptions
    m_LsDirOptionsEClass->setName("LsDirOptions");
    m_LsDirOptionsEClass->setAbstract(false);
    m_LsDirOptionsEClass->setInterface(false);
    m_LsDirOptions__longFormat->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEBoolean());
    m_LsDirOptions__longFormat->setName("longFormat");
    m_LsDirOptions__longFormat->setDefaultValueLiteral("false");
    m_LsDirOptions__longFormat->setLowerBound(0);
    m_LsDirOptions__longFormat->setUpperBound(1);
    m_LsDirOptions__longFormat->setTransient(false);
    m_LsDirOptions__longFormat->setVolatile(false);
    m_LsDirOptions__longFormat->setChangeable(true);
    m_LsDirOptions__longFormat->setUnsettable(false);
    m_LsDirOptions__longFormat->setID(false);
    m_LsDirOptions__longFormat->setUnique(true);
    m_LsDirOptions__longFormat->setDerived(false);
    m_LsDirOptions__longFormat->setOrdered(true);
    m_LsDirOptions__allFiles->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEBoolean());
    m_LsDirOptions__allFiles->setName("allFiles");
    m_LsDirOptions__allFiles->setDefaultValueLiteral("false");
    m_LsDirOptions__allFiles->setLowerBound(0);
    m_LsDirOptions__allFiles->setUpperBound(1);
    m_LsDirOptions__allFiles->setTransient(false);
    m_LsDirOptions__allFiles->setVolatile(false);
    m_LsDirOptions__allFiles->setChangeable(true);
    m_LsDirOptions__allFiles->setUnsettable(false);
    m_LsDirOptions__allFiles->setID(false);
    m_LsDirOptions__allFiles->setUnique(true);
    m_LsDirOptions__allFiles->setDerived(false);
    m_LsDirOptions__allFiles->setOrdered(true);
    // MvFileOptions
    m_MvFileOptionsEClass->setName("MvFileOptions");
    m_MvFileOptionsEClass->setAbstract(false);
    m_MvFileOptionsEClass->setInterface(false);
    m_MvFileOptions__trCommand->setEType(m_TransferCommandEEnum);
    m_MvFileOptions__trCommand->setName("trCommand");
    m_MvFileOptions__trCommand->setDefaultValueLiteral("2");
    m_MvFileOptions__trCommand->setLowerBound(0);
    m_MvFileOptions__trCommand->setUpperBound(1);
    m_MvFileOptions__trCommand->setTransient(false);
    m_MvFileOptions__trCommand->setVolatile(false);
    m_MvFileOptions__trCommand->setChangeable(true);
    m_MvFileOptions__trCommand->setUnsettable(false);
    m_MvFileOptions__trCommand->setID(false);
    m_MvFileOptions__trCommand->setUnique(true);
    m_MvFileOptions__trCommand->setDerived(false);
    m_MvFileOptions__trCommand->setOrdered(true);
    // LsTransferOptions
    m_LsTransferOptionsEClass->setName("LsTransferOptions");
    m_LsTransferOptionsEClass->setAbstract(false);
    m_LsTransferOptionsEClass->setInterface(false);
    m_LsTransferOptions__transferId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_LsTransferOptions__transferId->setName("transferId");
    m_LsTransferOptions__transferId->setDefaultValueLiteral("");
    m_LsTransferOptions__transferId->setLowerBound(0);
    m_LsTransferOptions__transferId->setUpperBound(1);
    m_LsTransferOptions__transferId->setTransient(false);
    m_LsTransferOptions__transferId->setVolatile(false);
    m_LsTransferOptions__transferId->setChangeable(true);
    m_LsTransferOptions__transferId->setUnsettable(false);
    m_LsTransferOptions__transferId->setID(false);
    m_LsTransferOptions__transferId->setUnique(true);
    m_LsTransferOptions__transferId->setDerived(false);
    m_LsTransferOptions__transferId->setOrdered(true);
    m_LsTransferOptions__fromMachineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_LsTransferOptions__fromMachineId->setName("fromMachineId");
    m_LsTransferOptions__fromMachineId->setDefaultValueLiteral("");
    m_LsTransferOptions__fromMachineId->setLowerBound(0);
    m_LsTransferOptions__fromMachineId->setUpperBound(1);
    m_LsTransferOptions__fromMachineId->setTransient(false);
    m_LsTransferOptions__fromMachineId->setVolatile(false);
    m_LsTransferOptions__fromMachineId->setChangeable(true);
    m_LsTransferOptions__fromMachineId->setUnsettable(false);
    m_LsTransferOptions__fromMachineId->setID(false);
    m_LsTransferOptions__fromMachineId->setUnique(true);
    m_LsTransferOptions__fromMachineId->setDerived(false);
    m_LsTransferOptions__fromMachineId->setOrdered(true);
    m_LsTransferOptions__userId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_LsTransferOptions__userId->setName("userId");
    m_LsTransferOptions__userId->setDefaultValueLiteral("");
    m_LsTransferOptions__userId->setLowerBound(0);
    m_LsTransferOptions__userId->setUpperBound(1);
    m_LsTransferOptions__userId->setTransient(false);
    m_LsTransferOptions__userId->setVolatile(false);
    m_LsTransferOptions__userId->setChangeable(true);
    m_LsTransferOptions__userId->setUnsettable(false);
    m_LsTransferOptions__userId->setID(false);
    m_LsTransferOptions__userId->setUnique(true);
    m_LsTransferOptions__userId->setDerived(false);
    m_LsTransferOptions__userId->setOrdered(true);
    m_LsTransferOptions__status->setEType(m_StatusEEnum);
    m_LsTransferOptions__status->setName("status");
    m_LsTransferOptions__status->setDefaultValueLiteral("4");
    m_LsTransferOptions__status->setLowerBound(0);
    m_LsTransferOptions__status->setUpperBound(1);
    m_LsTransferOptions__status->setTransient(false);
    m_LsTransferOptions__status->setVolatile(false);
    m_LsTransferOptions__status->setChangeable(true);
    m_LsTransferOptions__status->setUnsettable(false);
    m_LsTransferOptions__status->setID(false);
    m_LsTransferOptions__status->setUnique(true);
    m_LsTransferOptions__status->setDerived(false);
    m_LsTransferOptions__status->setOrdered(true);
    // FileStatList
    m_FileStatListEClass->setName("FileStatList");
    m_FileStatListEClass->setAbstract(false);
    m_FileStatListEClass->setInterface(false);
    m_FileStatList__fileStats->setEType(m_FileStatEClass);
    m_FileStatList__fileStats->setName("fileStats");
    m_FileStatList__fileStats->setDefaultValueLiteral("");
    m_FileStatList__fileStats->setLowerBound(0);
    m_FileStatList__fileStats->setUpperBound(-1);
    m_FileStatList__fileStats->setTransient(false);
    m_FileStatList__fileStats->setVolatile(false);
    m_FileStatList__fileStats->setChangeable(true);
    m_FileStatList__fileStats->setContainment(true);
    m_FileStatList__fileStats->setResolveProxies(true);
    m_FileStatList__fileStats->setUnique(true);
    m_FileStatList__fileStats->setDerived(false);
    m_FileStatList__fileStats->setOrdered(true);
    // StopTransferOptions
    m_StopTransferOptionsEClass->setName("StopTransferOptions");
    m_StopTransferOptionsEClass->setAbstract(false);
    m_StopTransferOptionsEClass->setInterface(false);
    m_StopTransferOptions__transferId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_StopTransferOptions__transferId->setName("transferId");
    m_StopTransferOptions__transferId->setDefaultValueLiteral("");
    m_StopTransferOptions__transferId->setLowerBound(0);
    m_StopTransferOptions__transferId->setUpperBound(1);
    m_StopTransferOptions__transferId->setTransient(false);
    m_StopTransferOptions__transferId->setVolatile(false);
    m_StopTransferOptions__transferId->setChangeable(true);
    m_StopTransferOptions__transferId->setUnsettable(false);
    m_StopTransferOptions__transferId->setID(false);
    m_StopTransferOptions__transferId->setUnique(true);
    m_StopTransferOptions__transferId->setDerived(false);
    m_StopTransferOptions__transferId->setOrdered(true);
    m_StopTransferOptions__fromMachineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_StopTransferOptions__fromMachineId->setName("fromMachineId");
    m_StopTransferOptions__fromMachineId->setDefaultValueLiteral("");
    m_StopTransferOptions__fromMachineId->setLowerBound(0);
    m_StopTransferOptions__fromMachineId->setUpperBound(1);
    m_StopTransferOptions__fromMachineId->setTransient(false);
    m_StopTransferOptions__fromMachineId->setVolatile(false);
    m_StopTransferOptions__fromMachineId->setChangeable(true);
    m_StopTransferOptions__fromMachineId->setUnsettable(false);
    m_StopTransferOptions__fromMachineId->setID(false);
    m_StopTransferOptions__fromMachineId->setUnique(true);
    m_StopTransferOptions__fromMachineId->setDerived(false);
    m_StopTransferOptions__fromMachineId->setOrdered(true);
    m_StopTransferOptions__userId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_StopTransferOptions__userId->setName("userId");
    m_StopTransferOptions__userId->setDefaultValueLiteral("");
    m_StopTransferOptions__userId->setLowerBound(0);
    m_StopTransferOptions__userId->setUpperBound(1);
    m_StopTransferOptions__userId->setTransient(false);
    m_StopTransferOptions__userId->setVolatile(false);
    m_StopTransferOptions__userId->setChangeable(true);
    m_StopTransferOptions__userId->setUnsettable(false);
    m_StopTransferOptions__userId->setID(false);
    m_StopTransferOptions__userId->setUnique(true);
    m_StopTransferOptions__userId->setDerived(false);
    m_StopTransferOptions__userId->setOrdered(true);
    // FileTransfer
    m_FileTransferEClass->setName("FileTransfer");
    m_FileTransferEClass->setAbstract(false);
    m_FileTransferEClass->setInterface(false);
    m_FileTransfer__transferId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileTransfer__transferId->setName("transferId");
    m_FileTransfer__transferId->setDefaultValueLiteral("");
    m_FileTransfer__transferId->setLowerBound(0);
    m_FileTransfer__transferId->setUpperBound(1);
    m_FileTransfer__transferId->setTransient(false);
    m_FileTransfer__transferId->setVolatile(false);
    m_FileTransfer__transferId->setChangeable(true);
    m_FileTransfer__transferId->setUnsettable(false);
    m_FileTransfer__transferId->setID(false);
    m_FileTransfer__transferId->setUnique(true);
    m_FileTransfer__transferId->setDerived(false);
    m_FileTransfer__transferId->setOrdered(true);
    m_FileTransfer__status->setEType(m_StatusEEnum);
    m_FileTransfer__status->setName("status");
    m_FileTransfer__status->setDefaultValueLiteral("4");
    m_FileTransfer__status->setLowerBound(0);
    m_FileTransfer__status->setUpperBound(1);
    m_FileTransfer__status->setTransient(false);
    m_FileTransfer__status->setVolatile(false);
    m_FileTransfer__status->setChangeable(true);
    m_FileTransfer__status->setUnsettable(false);
    m_FileTransfer__status->setID(false);
    m_FileTransfer__status->setUnique(true);
    m_FileTransfer__status->setDerived(false);
    m_FileTransfer__status->setOrdered(true);
    m_FileTransfer__userId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileTransfer__userId->setName("userId");
    m_FileTransfer__userId->setDefaultValueLiteral("");
    m_FileTransfer__userId->setLowerBound(0);
    m_FileTransfer__userId->setUpperBound(1);
    m_FileTransfer__userId->setTransient(false);
    m_FileTransfer__userId->setVolatile(false);
    m_FileTransfer__userId->setChangeable(true);
    m_FileTransfer__userId->setUnsettable(false);
    m_FileTransfer__userId->setID(false);
    m_FileTransfer__userId->setUnique(true);
    m_FileTransfer__userId->setDerived(false);
    m_FileTransfer__userId->setOrdered(true);
    m_FileTransfer__clientMachineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileTransfer__clientMachineId->setName("clientMachineId");
    m_FileTransfer__clientMachineId->setDefaultValueLiteral("");
    m_FileTransfer__clientMachineId->setLowerBound(0);
    m_FileTransfer__clientMachineId->setUpperBound(1);
    m_FileTransfer__clientMachineId->setTransient(false);
    m_FileTransfer__clientMachineId->setVolatile(false);
    m_FileTransfer__clientMachineId->setChangeable(true);
    m_FileTransfer__clientMachineId->setUnsettable(false);
    m_FileTransfer__clientMachineId->setID(false);
    m_FileTransfer__clientMachineId->setUnique(true);
    m_FileTransfer__clientMachineId->setDerived(false);
    m_FileTransfer__clientMachineId->setOrdered(true);
    m_FileTransfer__sourceMachineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileTransfer__sourceMachineId->setName("sourceMachineId");
    m_FileTransfer__sourceMachineId->setDefaultValueLiteral("");
    m_FileTransfer__sourceMachineId->setLowerBound(0);
    m_FileTransfer__sourceMachineId->setUpperBound(1);
    m_FileTransfer__sourceMachineId->setTransient(false);
    m_FileTransfer__sourceMachineId->setVolatile(false);
    m_FileTransfer__sourceMachineId->setChangeable(true);
    m_FileTransfer__sourceMachineId->setUnsettable(false);
    m_FileTransfer__sourceMachineId->setID(false);
    m_FileTransfer__sourceMachineId->setUnique(true);
    m_FileTransfer__sourceMachineId->setDerived(false);
    m_FileTransfer__sourceMachineId->setOrdered(true);
    m_FileTransfer__destinationMachineId->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileTransfer__destinationMachineId->setName("destinationMachineId");
    m_FileTransfer__destinationMachineId->setDefaultValueLiteral("");
    m_FileTransfer__destinationMachineId->setLowerBound(0);
    m_FileTransfer__destinationMachineId->setUpperBound(1);
    m_FileTransfer__destinationMachineId->setTransient(false);
    m_FileTransfer__destinationMachineId->setVolatile(false);
    m_FileTransfer__destinationMachineId->setChangeable(true);
    m_FileTransfer__destinationMachineId->setUnsettable(false);
    m_FileTransfer__destinationMachineId->setID(false);
    m_FileTransfer__destinationMachineId->setUnique(true);
    m_FileTransfer__destinationMachineId->setDerived(false);
    m_FileTransfer__destinationMachineId->setOrdered(true);
    m_FileTransfer__sourceFilePath->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileTransfer__sourceFilePath->setName("sourceFilePath");
    m_FileTransfer__sourceFilePath->setDefaultValueLiteral("");
    m_FileTransfer__sourceFilePath->setLowerBound(0);
    m_FileTransfer__sourceFilePath->setUpperBound(1);
    m_FileTransfer__sourceFilePath->setTransient(false);
    m_FileTransfer__sourceFilePath->setVolatile(false);
    m_FileTransfer__sourceFilePath->setChangeable(true);
    m_FileTransfer__sourceFilePath->setUnsettable(false);
    m_FileTransfer__sourceFilePath->setID(false);
    m_FileTransfer__sourceFilePath->setUnique(true);
    m_FileTransfer__sourceFilePath->setDerived(false);
    m_FileTransfer__sourceFilePath->setOrdered(true);
    m_FileTransfer__destinationFilePath->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileTransfer__destinationFilePath->setName("destinationFilePath");
    m_FileTransfer__destinationFilePath->setDefaultValueLiteral("");
    m_FileTransfer__destinationFilePath->setLowerBound(0);
    m_FileTransfer__destinationFilePath->setUpperBound(1);
    m_FileTransfer__destinationFilePath->setTransient(false);
    m_FileTransfer__destinationFilePath->setVolatile(false);
    m_FileTransfer__destinationFilePath->setChangeable(true);
    m_FileTransfer__destinationFilePath->setUnsettable(false);
    m_FileTransfer__destinationFilePath->setID(false);
    m_FileTransfer__destinationFilePath->setUnique(true);
    m_FileTransfer__destinationFilePath->setDerived(false);
    m_FileTransfer__destinationFilePath->setOrdered(true);
    m_FileTransfer__size->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_FileTransfer__size->setName("size");
    m_FileTransfer__size->setDefaultValueLiteral("");
    m_FileTransfer__size->setLowerBound(0);
    m_FileTransfer__size->setUpperBound(1);
    m_FileTransfer__size->setTransient(false);
    m_FileTransfer__size->setVolatile(false);
    m_FileTransfer__size->setChangeable(true);
    m_FileTransfer__size->setUnsettable(false);
    m_FileTransfer__size->setID(false);
    m_FileTransfer__size->setUnique(true);
    m_FileTransfer__size->setDerived(false);
    m_FileTransfer__size->setOrdered(true);
    m_FileTransfer__start_time->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_FileTransfer__start_time->setName("start_time");
    m_FileTransfer__start_time->setDefaultValueLiteral("0");
    m_FileTransfer__start_time->setLowerBound(0);
    m_FileTransfer__start_time->setUpperBound(1);
    m_FileTransfer__start_time->setTransient(false);
    m_FileTransfer__start_time->setVolatile(false);
    m_FileTransfer__start_time->setChangeable(true);
    m_FileTransfer__start_time->setUnsettable(false);
    m_FileTransfer__start_time->setID(false);
    m_FileTransfer__start_time->setUnique(true);
    m_FileTransfer__start_time->setDerived(false);
    m_FileTransfer__start_time->setOrdered(true);
    m_FileTransfer__trCommand->setEType(m_TransferCommandEEnum);
    m_FileTransfer__trCommand->setName("trCommand");
    m_FileTransfer__trCommand->setDefaultValueLiteral("2");
    m_FileTransfer__trCommand->setLowerBound(0);
    m_FileTransfer__trCommand->setUpperBound(1);
    m_FileTransfer__trCommand->setTransient(false);
    m_FileTransfer__trCommand->setVolatile(false);
    m_FileTransfer__trCommand->setChangeable(true);
    m_FileTransfer__trCommand->setUnsettable(false);
    m_FileTransfer__trCommand->setID(false);
    m_FileTransfer__trCommand->setUnique(true);
    m_FileTransfer__trCommand->setDerived(false);
    m_FileTransfer__trCommand->setOrdered(true);
    m_FileTransfer__errorMsg->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_FileTransfer__errorMsg->setName("errorMsg");
    m_FileTransfer__errorMsg->setDefaultValueLiteral("");
    m_FileTransfer__errorMsg->setLowerBound(0);
    m_FileTransfer__errorMsg->setUpperBound(1);
    m_FileTransfer__errorMsg->setTransient(false);
    m_FileTransfer__errorMsg->setVolatile(false);
    m_FileTransfer__errorMsg->setChangeable(true);
    m_FileTransfer__errorMsg->setUnsettable(false);
    m_FileTransfer__errorMsg->setID(false);
    m_FileTransfer__errorMsg->setUnique(true);
    m_FileTransfer__errorMsg->setDerived(false);
    m_FileTransfer__errorMsg->setOrdered(true);
    // FileTransferList
    m_FileTransferListEClass->setName("FileTransferList");
    m_FileTransferListEClass->setAbstract(false);
    m_FileTransferListEClass->setInterface(false);
    m_FileTransferList__fileTransfers->setEType(m_FileTransferEClass);
    m_FileTransferList__fileTransfers->setName("fileTransfers");
    m_FileTransferList__fileTransfers->setDefaultValueLiteral("");
    m_FileTransferList__fileTransfers->setLowerBound(0);
    m_FileTransferList__fileTransfers->setUpperBound(-1);
    m_FileTransferList__fileTransfers->setTransient(false);
    m_FileTransferList__fileTransfers->setVolatile(false);
    m_FileTransferList__fileTransfers->setChangeable(true);
    m_FileTransferList__fileTransfers->setContainment(true);
    m_FileTransferList__fileTransfers->setResolveProxies(true);
    m_FileTransferList__fileTransfers->setUnique(true);
    m_FileTransferList__fileTransfers->setDerived(false);
    m_FileTransferList__fileTransfers->setOrdered(true);
    // HeadOfFileOptions
    m_HeadOfFileOptionsEClass->setName("HeadOfFileOptions");
    m_HeadOfFileOptionsEClass->setAbstract(false);
    m_HeadOfFileOptionsEClass->setInterface(false);
    m_HeadOfFileOptions__nline->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEInt());
    m_HeadOfFileOptions__nline->setName("nline");
    m_HeadOfFileOptions__nline->setDefaultValueLiteral("10");
    m_HeadOfFileOptions__nline->setLowerBound(0);
    m_HeadOfFileOptions__nline->setUpperBound(1);
    m_HeadOfFileOptions__nline->setTransient(false);
    m_HeadOfFileOptions__nline->setVolatile(false);
    m_HeadOfFileOptions__nline->setChangeable(true);
    m_HeadOfFileOptions__nline->setUnsettable(false);
    m_HeadOfFileOptions__nline->setID(false);
    m_HeadOfFileOptions__nline->setUnique(true);
    m_HeadOfFileOptions__nline->setDerived(false);
    m_HeadOfFileOptions__nline->setOrdered(true);
    // TailOfFileOptions
    m_TailOfFileOptionsEClass->setName("TailOfFileOptions");
    m_TailOfFileOptionsEClass->setAbstract(false);
    m_TailOfFileOptionsEClass->setInterface(false);
    m_TailOfFileOptions__nline->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEInt());
    m_TailOfFileOptions__nline->setName("nline");
    m_TailOfFileOptions__nline->setDefaultValueLiteral("10");
    m_TailOfFileOptions__nline->setLowerBound(0);
    m_TailOfFileOptions__nline->setUpperBound(1);
    m_TailOfFileOptions__nline->setTransient(false);
    m_TailOfFileOptions__nline->setVolatile(false);
    m_TailOfFileOptions__nline->setChangeable(true);
    m_TailOfFileOptions__nline->setUnsettable(false);
    m_TailOfFileOptions__nline->setID(false);
    m_TailOfFileOptions__nline->setUnique(true);
    m_TailOfFileOptions__nline->setDerived(false);
    m_TailOfFileOptions__nline->setOrdered(true);
    // RmFileOptions
    m_RmFileOptionsEClass->setName("RmFileOptions");
    m_RmFileOptionsEClass->setAbstract(false);
    m_RmFileOptionsEClass->setInterface(false);
    m_RmFileOptions__isRecursive->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEBoolean());
    m_RmFileOptions__isRecursive->setName("isRecursive");
    m_RmFileOptions__isRecursive->setDefaultValueLiteral("false");
    m_RmFileOptions__isRecursive->setLowerBound(0);
    m_RmFileOptions__isRecursive->setUpperBound(1);
    m_RmFileOptions__isRecursive->setTransient(false);
    m_RmFileOptions__isRecursive->setVolatile(false);
    m_RmFileOptions__isRecursive->setChangeable(true);
    m_RmFileOptions__isRecursive->setUnsettable(false);
    m_RmFileOptions__isRecursive->setID(false);
    m_RmFileOptions__isRecursive->setUnique(true);
    m_RmFileOptions__isRecursive->setDerived(false);
    m_RmFileOptions__isRecursive->setOrdered(true);
    // CreateDirOptions
    m_CreateDirOptionsEClass->setName("CreateDirOptions");
    m_CreateDirOptionsEClass->setAbstract(false);
    m_CreateDirOptionsEClass->setInterface(false);
    m_CreateDirOptions__isRecursive->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEBoolean());
    m_CreateDirOptions__isRecursive->setName("isRecursive");
    m_CreateDirOptions__isRecursive->setDefaultValueLiteral("false");
    m_CreateDirOptions__isRecursive->setLowerBound(0);
    m_CreateDirOptions__isRecursive->setUpperBound(1);
    m_CreateDirOptions__isRecursive->setTransient(false);
    m_CreateDirOptions__isRecursive->setVolatile(false);
    m_CreateDirOptions__isRecursive->setChangeable(true);
    m_CreateDirOptions__isRecursive->setUnsettable(false);
    m_CreateDirOptions__isRecursive->setID(false);
    m_CreateDirOptions__isRecursive->setUnique(true);
    m_CreateDirOptions__isRecursive->setDerived(false);
    m_CreateDirOptions__isRecursive->setOrdered(true);
    // DirEntry
    m_DirEntryEClass->setName("DirEntry");
    m_DirEntryEClass->setAbstract(false);
    m_DirEntryEClass->setInterface(false);
    m_DirEntry__path->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_DirEntry__path->setName("path");
    m_DirEntry__path->setDefaultValueLiteral("");
    m_DirEntry__path->setLowerBound(0);
    m_DirEntry__path->setUpperBound(1);
    m_DirEntry__path->setTransient(false);
    m_DirEntry__path->setVolatile(false);
    m_DirEntry__path->setChangeable(true);
    m_DirEntry__path->setUnsettable(false);
    m_DirEntry__path->setID(false);
    m_DirEntry__path->setUnique(true);
    m_DirEntry__path->setDerived(false);
    m_DirEntry__path->setOrdered(true);
    m_DirEntry__owner->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_DirEntry__owner->setName("owner");
    m_DirEntry__owner->setDefaultValueLiteral("");
    m_DirEntry__owner->setLowerBound(0);
    m_DirEntry__owner->setUpperBound(1);
    m_DirEntry__owner->setTransient(false);
    m_DirEntry__owner->setVolatile(false);
    m_DirEntry__owner->setChangeable(true);
    m_DirEntry__owner->setUnsettable(false);
    m_DirEntry__owner->setID(false);
    m_DirEntry__owner->setUnique(true);
    m_DirEntry__owner->setDerived(false);
    m_DirEntry__owner->setOrdered(true);
    m_DirEntry__group->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_DirEntry__group->setName("group");
    m_DirEntry__group->setDefaultValueLiteral("");
    m_DirEntry__group->setLowerBound(0);
    m_DirEntry__group->setUpperBound(1);
    m_DirEntry__group->setTransient(false);
    m_DirEntry__group->setVolatile(false);
    m_DirEntry__group->setChangeable(true);
    m_DirEntry__group->setUnsettable(false);
    m_DirEntry__group->setID(false);
    m_DirEntry__group->setUnique(true);
    m_DirEntry__group->setDerived(false);
    m_DirEntry__group->setOrdered(true);
    m_DirEntry__perms->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEInt());
    m_DirEntry__perms->setName("perms");
    m_DirEntry__perms->setDefaultValueLiteral("");
    m_DirEntry__perms->setLowerBound(0);
    m_DirEntry__perms->setUpperBound(1);
    m_DirEntry__perms->setTransient(false);
    m_DirEntry__perms->setVolatile(false);
    m_DirEntry__perms->setChangeable(true);
    m_DirEntry__perms->setUnsettable(false);
    m_DirEntry__perms->setID(false);
    m_DirEntry__perms->setUnique(true);
    m_DirEntry__perms->setDerived(false);
    m_DirEntry__perms->setOrdered(true);
    m_DirEntry__size->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getELong());
    m_DirEntry__size->setName("size");
    m_DirEntry__size->setDefaultValueLiteral("");
    m_DirEntry__size->setLowerBound(0);
    m_DirEntry__size->setUpperBound(1);
    m_DirEntry__size->setTransient(false);
    m_DirEntry__size->setVolatile(false);
    m_DirEntry__size->setChangeable(true);
    m_DirEntry__size->setUnsettable(false);
    m_DirEntry__size->setID(false);
    m_DirEntry__size->setUnique(true);
    m_DirEntry__size->setDerived(false);
    m_DirEntry__size->setOrdered(true);
    m_DirEntry__creationTime->setEType(
            dynamic_cast< ::ecore::EcorePackage* > (::ecore::EcorePackage::_instance())->getEString());
    m_DirEntry__creationTime->setName("creationTime");
    m_DirEntry__creationTime->setDefaultValueLiteral("");
    m_DirEntry__creationTime->setLowerBound(0);
    m_DirEntry__creationTime->setUpperBound(1);
    m_DirEntry__creationTime->setTransient(false);
    m_DirEntry__creationTime->setVolatile(false);
    m_DirEntry__creationTime->setChangeable(true);
    m_DirEntry__creationTime->setUnsettable(false);
    m_DirEntry__creationTime->setID(false);
    m_DirEntry__creationTime->setUnique(true);
    m_DirEntry__creationTime->setDerived(false);
    m_DirEntry__creationTime->setOrdered(true);
    m_DirEntry__type->setEType(m_FileTypeEEnum);
    m_DirEntry__type->setName("type");
    m_DirEntry__type->setDefaultValueLiteral("6");
    m_DirEntry__type->setLowerBound(0);
    m_DirEntry__type->setUpperBound(1);
    m_DirEntry__type->setTransient(false);
    m_DirEntry__type->setVolatile(false);
    m_DirEntry__type->setChangeable(true);
    m_DirEntry__type->setUnsettable(false);
    m_DirEntry__type->setID(false);
    m_DirEntry__type->setUnique(true);
    m_DirEntry__type->setDerived(false);
    m_DirEntry__type->setOrdered(true);
    // DirEntryList
    m_DirEntryListEClass->setName("DirEntryList");
    m_DirEntryListEClass->setAbstract(false);
    m_DirEntryListEClass->setInterface(false);
    m_DirEntryList__dirEntries->setEType(m_DirEntryEClass);
    m_DirEntryList__dirEntries->setName("dirEntries");
    m_DirEntryList__dirEntries->setDefaultValueLiteral("");
    m_DirEntryList__dirEntries->setLowerBound(0);
    m_DirEntryList__dirEntries->setUpperBound(-1);
    m_DirEntryList__dirEntries->setTransient(false);
    m_DirEntryList__dirEntries->setVolatile(false);
    m_DirEntryList__dirEntries->setChangeable(true);
    m_DirEntryList__dirEntries->setContainment(true);
    m_DirEntryList__dirEntries->setResolveProxies(true);
    m_DirEntryList__dirEntries->setUnique(true);
    m_DirEntryList__dirEntries->setDerived(false);
    m_DirEntryList__dirEntries->setOrdered(true);

    // TODO: Initialize data types


    // FileType
    m_FileTypeEEnum->setName("FileType");
    m_FileTypeEEnum->setSerializable(true);

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // BLOCK
        _el->setName("BLOCK");
        _el->setValue(0);
        _el->setLiteral("BLOCK");
        _el->setEEnum(m_FileTypeEEnum);
        m_FileTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // CHARACTER
        _el->setName("CHARACTER");
        _el->setValue(1);
        _el->setLiteral("CHARACTER");
        _el->setEEnum(m_FileTypeEEnum);
        m_FileTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // DIRECTORY
        _el->setName("DIRECTORY");
        _el->setValue(2);
        _el->setLiteral("DIRECTORY");
        _el->setEEnum(m_FileTypeEEnum);
        m_FileTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // SYMBOLICLINK
        _el->setName("SYMBOLICLINK");
        _el->setValue(3);
        _el->setLiteral("SYMBOLICLINK");
        _el->setEEnum(m_FileTypeEEnum);
        m_FileTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // SCKT
        _el->setName("SCKT");
        _el->setValue(4);
        _el->setLiteral("SCKT");
        _el->setEEnum(m_FileTypeEEnum);
        m_FileTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // FIFO
        _el->setName("FIFO");
        _el->setValue(5);
        _el->setLiteral("FIFO");
        _el->setEEnum(m_FileTypeEEnum);
        m_FileTypeEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // REGULAR
        _el->setName("REGULAR");
        _el->setValue(6);
        _el->setLiteral("REGULAR");
        _el->setEEnum(m_FileTypeEEnum);
        m_FileTypeEEnum->getELiterals().push_back(_el);
    }

    // Status
    m_StatusEEnum->setName("Status");
    m_StatusEEnum->setSerializable(true);

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // INPROGRESS
        _el->setName("INPROGRESS");
        _el->setValue(0);
        _el->setLiteral("INPROGRESS");
        _el->setEEnum(m_StatusEEnum);
        m_StatusEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // COMPLETED
        _el->setName("COMPLETED");
        _el->setValue(1);
        _el->setLiteral("COMPLETED");
        _el->setEEnum(m_StatusEEnum);
        m_StatusEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // CANCELLED
        _el->setName("CANCELLED");
        _el->setValue(2);
        _el->setLiteral("CANCELLED");
        _el->setEEnum(m_StatusEEnum);
        m_StatusEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // FAILED
        _el->setName("FAILED");
        _el->setValue(3);
        _el->setLiteral("FAILED");
        _el->setEEnum(m_StatusEEnum);
        m_StatusEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // UNDEFINED
        _el->setName("UNDEFINED");
        _el->setValue(4);
        _el->setLiteral("UNDEFINED");
        _el->setEEnum(m_StatusEEnum);
        m_StatusEEnum->getELiterals().push_back(_el);
    }

    // TransferCommand
    m_TransferCommandEEnum->setName("TransferCommand");
    m_TransferCommandEEnum->setSerializable(true);

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // SCP
        _el->setName("SCP");
        _el->setValue(0);
        _el->setLiteral("SCP");
        _el->setEEnum(m_TransferCommandEEnum);
        m_TransferCommandEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // RSYNC
        _el->setName("RSYNC");
        _el->setValue(1);
        _el->setLiteral("RSYNC");
        _el->setEEnum(m_TransferCommandEEnum);
        m_TransferCommandEEnum->getELiterals().push_back(_el);
    }

    {
        ::ecore::EEnumLiteral_ptr _el = new ::ecore::EEnumLiteral();
        // UNDEFINED
        _el->setName("UNDEFINED");
        _el->setValue(2);
        _el->setLiteral("UNDEFINED");
        _el->setEEnum(m_TransferCommandEEnum);
        m_TransferCommandEEnum->getELiterals().push_back(_el);
    }

    _initialize();
}

::ecore::EEnum_ptr FMS_DataPackage::getFileType()
{
    return m_FileTypeEEnum;
}
::ecore::EEnum_ptr FMS_DataPackage::getStatus()
{
    return m_StatusEEnum;
}
::ecore::EClass_ptr FMS_DataPackage::getFileStat()
{
    return m_FileStatEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getCpFileOptions()
{
    return m_CpFileOptionsEClass;
}
::ecore::EEnum_ptr FMS_DataPackage::getTransferCommand()
{
    return m_TransferCommandEEnum;
}
::ecore::EClass_ptr FMS_DataPackage::getLsDirOptions()
{
    return m_LsDirOptionsEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getMvFileOptions()
{
    return m_MvFileOptionsEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getLsTransferOptions()
{
    return m_LsTransferOptionsEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getFileStatList()
{
    return m_FileStatListEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getStopTransferOptions()
{
    return m_StopTransferOptionsEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getFileTransfer()
{
    return m_FileTransferEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getFileTransferList()
{
    return m_FileTransferListEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getHeadOfFileOptions()
{
    return m_HeadOfFileOptionsEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getTailOfFileOptions()
{
    return m_TailOfFileOptionsEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getRmFileOptions()
{
    return m_RmFileOptionsEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getCreateDirOptions()
{
    return m_CreateDirOptionsEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getDirEntry()
{
    return m_DirEntryEClass;
}
::ecore::EClass_ptr FMS_DataPackage::getDirEntryList()
{
    return m_DirEntryListEClass;
}

::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__path()
{
    return m_FileStat__path;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__owner()
{
    return m_FileStat__owner;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__group()
{
    return m_FileStat__group;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__perms()
{
    return m_FileStat__perms;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__uid()
{
    return m_FileStat__uid;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__gid()
{
    return m_FileStat__gid;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__size()
{
    return m_FileStat__size;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__atime()
{
    return m_FileStat__atime;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__mtime()
{
    return m_FileStat__mtime;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__ctime()
{
    return m_FileStat__ctime;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileStat__type()
{
    return m_FileStat__type;
}
::ecore::EAttribute_ptr FMS_DataPackage::getCpFileOptions__isRecursive()
{
    return m_CpFileOptions__isRecursive;
}
::ecore::EAttribute_ptr FMS_DataPackage::getCpFileOptions__trCommand()
{
    return m_CpFileOptions__trCommand;
}
::ecore::EAttribute_ptr FMS_DataPackage::getLsDirOptions__longFormat()
{
    return m_LsDirOptions__longFormat;
}
::ecore::EAttribute_ptr FMS_DataPackage::getLsDirOptions__allFiles()
{
    return m_LsDirOptions__allFiles;
}
::ecore::EAttribute_ptr FMS_DataPackage::getMvFileOptions__trCommand()
{
    return m_MvFileOptions__trCommand;
}
::ecore::EAttribute_ptr FMS_DataPackage::getLsTransferOptions__transferId()
{
    return m_LsTransferOptions__transferId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getLsTransferOptions__fromMachineId()
{
    return m_LsTransferOptions__fromMachineId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getLsTransferOptions__userId()
{
    return m_LsTransferOptions__userId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getLsTransferOptions__status()
{
    return m_LsTransferOptions__status;
}
::ecore::EReference_ptr FMS_DataPackage::getFileStatList__fileStats()
{
    return m_FileStatList__fileStats;
}
::ecore::EAttribute_ptr FMS_DataPackage::getStopTransferOptions__transferId()
{
    return m_StopTransferOptions__transferId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getStopTransferOptions__fromMachineId()
{
    return m_StopTransferOptions__fromMachineId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getStopTransferOptions__userId()
{
    return m_StopTransferOptions__userId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__transferId()
{
    return m_FileTransfer__transferId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__status()
{
    return m_FileTransfer__status;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__userId()
{
    return m_FileTransfer__userId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__clientMachineId()
{
    return m_FileTransfer__clientMachineId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__sourceMachineId()
{
    return m_FileTransfer__sourceMachineId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__destinationMachineId()
{
    return m_FileTransfer__destinationMachineId;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__sourceFilePath()
{
    return m_FileTransfer__sourceFilePath;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__destinationFilePath()
{
    return m_FileTransfer__destinationFilePath;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__size()
{
    return m_FileTransfer__size;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__start_time()
{
    return m_FileTransfer__start_time;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__trCommand()
{
    return m_FileTransfer__trCommand;
}
::ecore::EAttribute_ptr FMS_DataPackage::getFileTransfer__errorMsg()
{
    return m_FileTransfer__errorMsg;
}
::ecore::EReference_ptr FMS_DataPackage::getFileTransferList__fileTransfers()
{
    return m_FileTransferList__fileTransfers;
}
::ecore::EAttribute_ptr FMS_DataPackage::getHeadOfFileOptions__nline()
{
    return m_HeadOfFileOptions__nline;
}
::ecore::EAttribute_ptr FMS_DataPackage::getTailOfFileOptions__nline()
{
    return m_TailOfFileOptions__nline;
}
::ecore::EAttribute_ptr FMS_DataPackage::getRmFileOptions__isRecursive()
{
    return m_RmFileOptions__isRecursive;
}
::ecore::EAttribute_ptr FMS_DataPackage::getCreateDirOptions__isRecursive()
{
    return m_CreateDirOptions__isRecursive;
}
::ecore::EAttribute_ptr FMS_DataPackage::getDirEntry__path()
{
    return m_DirEntry__path;
}
::ecore::EAttribute_ptr FMS_DataPackage::getDirEntry__owner()
{
    return m_DirEntry__owner;
}
::ecore::EAttribute_ptr FMS_DataPackage::getDirEntry__group()
{
    return m_DirEntry__group;
}
::ecore::EAttribute_ptr FMS_DataPackage::getDirEntry__perms()
{
    return m_DirEntry__perms;
}
::ecore::EAttribute_ptr FMS_DataPackage::getDirEntry__size()
{
    return m_DirEntry__size;
}
::ecore::EAttribute_ptr FMS_DataPackage::getDirEntry__creationTime()
{
    return m_DirEntry__creationTime;
}
::ecore::EAttribute_ptr FMS_DataPackage::getDirEntry__type()
{
    return m_DirEntry__type;
}
::ecore::EReference_ptr FMS_DataPackage::getDirEntryList__dirEntries()
{
    return m_DirEntryList__dirEntries;
}

