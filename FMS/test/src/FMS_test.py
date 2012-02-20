#!/usr/bin/env python
import VISHNU
import time
import string
import os 
import shutil 
import sys

def convertPermissionsToString(perms):
   usrR = 1 << 8;
   usrW = 1 << 7;
   usrX = 1 << 6;
   grpR = 1 << 5;
   grpW = 1 << 4;
   grpX = 1 << 3;
   othR = 1 << 2;
   othW = 1 << 1;
   othX = 1;
   permissions=""
   if perms & usrR :
     permissions+="r"
   else:
     permissions+="-"
   if perms & usrW :
     permissions+="w"
   else:
     permissions+="-"
   if perms & usrX :
     permissions+="x"
   else:
     permissions+="-"
   if perms & grpR :
     permissions+="r"
   else:
     permissions+="-"
   if perms & grpW :
     permissions+="w"
   else:
     permissions+="-"
   if perms & grpX :
     permissions+="x"
   else:
     permissions+="-"
   if perms & othR :
     permissions+="r"
   else:
     permissions+="-"
   if perms & othW :
     permissions+="w"
   else:
     permissions+="-"
   if perms & othX :
     permissions+="x"
   else:
     permissions+="-"
   return permissions

def displayFileInfo(fileInfo):
  print "path:", fileInfo.getPath()
  #print "permissions:", convertPermissionsToString(fileInfo.getPerms())
  print "permissions:", oct(fileInfo.getPerms())
  print "owner:", fileInfo.getOwner()
  print "group:", fileInfo.getGroup()
  print "uid:", fileInfo.getUid()
  print "gid:", fileInfo.getGid()
  print "size:",type(fileInfo.getSize()), fileInfo.getSize()

def displayDirEntryInfo(fileInfo):
  print "path:", fileInfo.getPath()
  #print "permissions:", convertPermissionsToString(fileInfo.getPerms())
  print "permissions:", oct(fileInfo.getPerms())
  print "owner:", fileInfo.getOwner()
  print "group:", fileInfo.getGroup()
  print "size:",type(fileInfo.getSize()), fileInfo.getSize()
  print "creationTime:", fileInfo.getCreationTime()
  print "**************************************************"

def displayDirContent(dirContent):
  for i in range(dirContent.getDirEntries().size()):
    displayDirEntryInfo( dirContent.getDirEntries().get(i))

def dispalyContentOfFile(contentOfFile):
   print "---File content:"
   print contentOfFile

def displayFileTransfer(filetransfer):
  print "TransferId:", filetransfer.getTransferId()
  print "Status:", filetransfer.getStatus()
  print "UserId:", filetransfer.getUserId()
  print "Client Machine Id:", filetransfer.getClientMachineId()
  print "Source Machine Id:", filetransfer.getSourceMachineId()
  print "source path:", filetransfer.getSourceFilePath()
  print "source file size : ", filetransfer.getSize()
  print "Destination MachineId:", filetransfer.getDestinationMachineId()
  print "Destination path:", filetransfer.getDestinationFilePath()

def displayFiletransferList(fileTransferList):
  for i in range(fileTransferList.getFileTransfers().size()):
    displayFileTransfer(fileTransferList.getFileTransfers().get(i))


# constants definition
FMSWORKINGDIR = "/tmp"
FMSDIR1=FMSWORKINGDIR + "/FMSDIR1" 
FMSDIR2=FMSWORKINGDIR + "/FMSDIR2" 
userPwd = "vishnu_user";
userId = "root";
groupTest = "adm"; # userLogin must belong to groupTest on host 1
sep = ":";
slash = "/";
machineId1 = "MA_193"; 
machineId2 = "MA_193";

#The first argument in command line corresponds to the machine name
if len(sys.argv) > 1:
  machineId1 = sys.argv[1]
  machineId2 = sys.argv[1]

newFileName = "FMS_test_file";
newDirName = "FMS_test_dir";
newSubDirName = "FMS_test_sub_dir";
#local paths
workingDirFullPath1= machineId1 + sep + FMSWORKINGDIR
workingDirFullPath2= machineId2 + sep + FMSWORKINGDIR
baseDirFullPath1 = machineId1 + sep + FMSDIR1;
baseDirFullPath2 = machineId2 + sep + FMSDIR2;
fileFullPath1 = baseDirFullPath1 + slash + newFileName;
fileFullPath2 = baseDirFullPath2 + slash + newFileName;
dirFullPath1 = baseDirFullPath1 + slash + newDirName;
recursiveDirFullPath1 = dirFullPath1 + slash +  newSubDirName;
dirFullPath2 = baseDirFullPath2 + slash + newDirName;
  
#Objects initialization
headOpt = VISHNU.HeadOfFileOptions()
tailOpt = VISHNU.TailOfFileOptions()
dirContent = VISHNU.DirEntryList()
dirContent2 = VISHNU.DirEntryList()
lsOpt = VISHNU.LsDirOptions()
fileInfo = VISHNU.FileStat()
dirEntryInfo = VISHNU.DirEntry()
transferOpt = VISHNU.CpFileOptions()
fileTransferInfo = VISHNU.FileTransfer()
fileTransferList = VISHNU.FileTransferList()
stopTransferOptions = VISHNU.StopTransferOptions()
lsTransferOptions = VISHNU.LsTransferOptions()
session=VISHNU.Session()
contentOfFile="";
# preconditions
if(os.path.exists (FMSDIR1)):
  shutil.rmtree(FMSDIR1)

if(os.path.exists (FMSDIR2)):
  shutil.rmtree(FMSDIR2)


os.makedirs(FMSDIR1)
os.makedirs(FMSDIR2)

vishnu_client_file_path = os.getenv("VISHNU_CONFIG_FILE") 
VISHNU.vishnuInitialize(vishnu_client_file_path)

##########################################################


try :

  r = VISHNU.connect(userId, userPwd,session)
  k = session.getSessionKey()
  print "createFile:",fileFullPath1
  VISHNU.createFile(k,  fileFullPath1)
  print "sessionKey:", k
  print "==================getFileInfo===============:"
  VISHNU.getFileInfo(k, fileFullPath1,  fileInfo)
  displayFileInfo(fileInfo)
  print "removeFile: ", fileFullPath1
  VISHNU.removeFile(k, fileFullPath1)
  print "createDir: ", dirFullPath1
  VISHNU.createDir(k, dirFullPath1)
  print "removeDir: ", dirFullPath1
  VISHNU.removeDir(k, dirFullPath1)
  print "===================listDir====================:"
  VISHNU.listDir(k,workingDirFullPath1, dirContent)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getDirEntries().clear()
  print "createFile : ",fileFullPath1
  VISHNU.createFile(k, fileFullPath1)
  print "copyFile: ",fileFullPath1, "to ", fileFullPath2
  VISHNU.copyFile(k, fileFullPath1, baseDirFullPath2)
  print "===================listDir - LongFormat====================:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, baseDirFullPath2, dirContent, lsOpt)
  displayDirContent(dirContent)
  VISHNU.removeFile(k,fileFullPath2)
  #To clean the list
  dirContent.getDirEntries().clear()
  print "=================== chGrp ====================:"
  VISHNU.createFile(k,fileFullPath2)
  VISHNU.getFileInfo(k,fileFullPath2,fileInfo)
  displayFileInfo(fileInfo)

  print "chGrp: ",fileFullPath2, "to adm"
  VISHNU.chGrp(k, "adm", fileFullPath2)
  VISHNU.getFileInfo(k,fileFullPath2,fileInfo)
  displayFileInfo(fileInfo)
  VISHNU.removeFile(k,fileFullPath2)
  print "=================== chMod ====================:"
  VISHNU.createFile(k,fileFullPath2)
  VISHNU.getFileInfo(k,fileFullPath2,fileInfo)
  displayFileInfo(fileInfo)
  
  print "chMod: ",fileFullPath2, "to 754"
  VISHNU.chMod(k, 754, fileFullPath2)
  VISHNU.getFileInfo(k,fileFullPath2,fileInfo)
  displayFileInfo(fileInfo)
  VISHNU.removeFile(k,fileFullPath2)

  print "===================headOfFile======:"
  command ="/bin/ls " + FMSWORKINGDIR + " > " + FMSDIR1 + "/" + newFileName
  os.system (command )
  r,contentOfFile =VISHNU.headOfFile(k,fileFullPath1)
  print contentOfFile
  print "===================tailOfFile======:"
  r, contentOfFile = VISHNU.tailOfFile(k, fileFullPath1)
  print contentOfFile
  print "===================contentOfFile======:"
  r, contentOfFile = VISHNU.contentOfFile(k, fileFullPath1)
  print contentOfFile
  VISHNU.removeFile(k,fileFullPath1)
  print contentOfFile
  print "=========================copyAsyncFile:"
  transferOpt.setIsRecursive(True)
  VISHNU.copyAsyncFile(k, workingDirFullPath1,baseDirFullPath2, fileTransferInfo,transferOpt)
  displayFileTransfer(fileTransferInfo)
  print "===========listFileTransfers:============="
  VISHNU.listFileTransfers(k, fileTransferList)
  displayFiletransferList(fileTransferList)
  #To clean the list
  fileTransferList.getFileTransfers().clear()
  print "===========stopFileTransfer===============:"
  print fileTransferInfo.getTransferId()
  stopTransferOptions.setTransferId(fileTransferInfo.getTransferId())
  VISHNU.stopFileTransfer(k, stopTransferOptions)
  print "===========listFileTransfers:============="

  VISHNU.listFileTransfers(k, fileTransferList)
  displayFiletransferList(fileTransferList)

  print "=========================MvAsyncFile:"
  VISHNU.createFile(k,fileFullPath1)
  VISHNU.moveAsyncFile(k, fileFullPath1,baseDirFullPath2, fileTransferInfo)
  displayFileTransfer(fileTransferInfo)
  print "===========listFileTransfers:============="
  VISHNU.listFileTransfers(k, fileTransferList)
  displayFiletransferList(fileTransferList)

except VISHNU.FMSVishnuException, e:
  print e.what()
except VISHNU.UMSVishnuException, e:
  print e.what()
except VISHNU.SystemException, e:
  print e.what()
except VISHNU.UserException, e:
  print e.what()

VISHNU.close(k)

