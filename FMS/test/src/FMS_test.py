
import VISHNU
import time
import string
import os 
def displayDirContent(dirContent):
  for i in range(dirContent.getDirEntries().size()):
    print dirContent.getDirEntries().get(i)

def displayFileInfo(fileInfo):
  print "path:", fileInfo.getPath()
  print "owner:", fileInfo.getOwner()
  print "group:", fileInfo.getGroup()
  print "size:", fileInfo.getSize()
  print "creationTime:", fileInfo.getCreationTime()

def dispalyContentOfFile(contentOfFile):
   print "---File content:"
   print contentOfFile

def displayFileTransfer(filetransfer):
  print "TransferId:", filetransfer.getTransferId()
  print "Status:", filetransfer.getStatus()
  print "UserId:", filetransfer.getUserId()
  print "Client Machine Id:", filetransfer.getClientMachineId()
  print "Source Machine Id:", filetransfer.getSourceMachineId()
  print "get Destination MachineId:", filetransfer.getDestinationMachineId()

def displayFiletransferList(fileTransferList):
  for i in range(fileTransferList.getFileTransfers().size()):
    displayFileTransfer(fileTransferList.getFileTransfers().get(i))

vishnu_client_file_path =os.getenv("VISHNU_CONFIG_FILE") 
machineI d= "MA_1";
VISHNU.vishnuInitialize(vishnu_client_file_path)
path = machineId + ":/tmp/testFMSpython"
DIR = machineId + ":/tmp/testFMSDirpython"
DIR2 = machineId + ":/tmp/"
path3 = machineId + ":/tmp/testCpFms"
path4 = machineId + ":/tmp/testGrp"
path5 = machineId + ":/tmp/testGrpMoved"
DIR3 = machineId + ":/tmp/TmpTestFMS"
DIR4 = "/home/capo-chichi/Telechargements/"
DIR5 = machineId + ":/tmp/TransferLib/"
DIR6 = machineId + ":/tmp/MoveAsyncTest/"
contentOfFile = machineId + ""

#Objects initialization
headOpt = VISHNU.HeadOfFileOptions()
tailOpt = VISHNU.TailOfFileOptions()
dirContent = VISHNU.DirEntryList()
dirContent2 = VISHNU.DirEntryList()
lsOpt = VISHNU.LsDirOptions()
fileInfo = VISHNU.FileStat()
lscpAsyncOpt = VISHNU.CpFileOptions()
fileTransferInfo = VISHNU.FileTransfer()
fileTransferList = VISHNU.FileTransferList()
stopTransferOptions = VISHNU.StopTransferOptions()
lsTransferOptions = VISHNU.LsTransferOptions()

try :
  r, k = VISHNU.connect("root", "vishnu_user")
  print "createFile:"
  VISHNU.createFile(k, path)
  print "sessionKey:", k
  print "path:", path
  print "==================getFilesInfo===============:"
  VISHNU.getFilesInfo(k, path,  fileInfo)
  displayFileInfo(fileInfo)
  print "removeFile:"
  VISHNU.removeFile(k, path)
  print "createDir:"
  VISHNU.createDir(k, DIR)
  print "removeDir:"
  VISHNU.removeDir(k, DIR)
  print "===================listDir====================:"
  VISHNU.listDir(k, DIR2, dirContent)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getDirEntries().clear()
  print "createFile testCpFms:"
  VISHNU.createFile(k, path)
  print "copyFile testGrp:"
  VISHNU.copyFile(k, path3, path4)
  print "===================listDir - LongFormat====================:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getDirEntries().clear()
  print "===================listDir - LongFormat with testGrp file====================:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getDirEntries().clear()
  print "chGrp:"
  VISHNU.chGrp(k, "adm", path)
  print "===================listDir - LongFormat with change group on testGrp file============:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getDirEntries().clear()
  print "chMod:"
  VISHNU.chMod(k, 754, path)
  print "===================listDir - LongFormat with change mode on testGrp file============:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getDirEntries().clear()
  print "moveFile"
  VISHNU.moveFile(k, path4, path5)
  print "===================listDir============:"
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getDirEntries().clear()
  print "===================headOfFile======:"
  r, contentOfFile = VISHNU.headOfFile(k, path2)
  print contentOfFile
  print "===================tailOfFile======:"
  r, contentOfFile = VISHNU.tailOfFile(k, path2)
  print contentOfFile
  print "===================contentOfFile======:"
  r, contentOfFile = VISHNU.contentOfFile(k, path2)
  print contentOfFile
  print "removeFile:"
  VISHNU.removeFile(k, path3)
  VISHNU.removeFile(k, path5)
  print "===================listDir============:"
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getDirEntries().clear()
  print "copyAsyncFile:"
  lscpAsyncOpt.setIsRecursive(True)
  VISHNU.copyAsyncFile(k, DIR4, DIR5, fileTransferInfo,lscpAsyncOpt)
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
  #fileTransferList= []
  lsTransferOptions.setStatus(2)
  VISHNU.listFileTransfers(k, fileTransferList, lsTransferOptions)
  displayFiletransferList(fileTransferList)
  print "copyAsyncFile:"
  lscpAsyncOpt.setIsRecursive(True)
  VISHNU.copyAsyncFile(k, DIR4, DIR5, fileTransferInfo,lscpAsyncOpt)
  print "createDir:"
  VISHNU.createDir(k, DIR6)
  #print "moveAsyncFile:"
  #VISHNU.moveAsyncFile(k, DIR5, DIR6, fileTransferInfo)
  #displayFileTransfer(fileTransferInfo)

except VISHNU.FMSVishnuException, e:
  print e.what()
except VISHNU.UMSVishnuException, e:
  print e.what()
except VISHNU.SystemException, e:
  print e.what()
except VISHNU.UserException, e:
  print e.what()

VISHNU.close(k)

#execfile("/home/capo-chichi/Sysfera/vishnu/FMS/test/src/testFMS_Python_new.py")
