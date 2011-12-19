
import VISHNU
import time
import string

def displayDirContent(dirContent):
  for i in range(dirContent.getStrings().size()):
    print dirContent.getStrings()[i]

def displayFileInfo(fileInfo):
  print "path:", fileInfo.getPath()
  print "owner:", fileInfo.getOwner()
  print "group:", fileInfo.getGroup()
  print "uid:", fileInfo.getUid()
  print "gid:", fileInfo.getGid()
  print "size:", fileInfo.getSize()

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

VISHNU.vishnuInitialize("/home/ibrahima/Brouillon/buildFMS/test_files/cfg/client_testing.cfg")

path = "MA_2:/tmp/testFMSpython"
DIR = "MA_2:/tmp/testFMSDirpython"
path2 = "MA_2:/tmp/omninames-Paco-Sysfera.log"
DIR2 = "MA_2:/tmp/"
path3 = "MA_2:/tmp/testCpFms"
path4 = "MA_2:/tmp/testGrp"
path5 = "MA_2:/tmp/testGrpMoved"
DIR3 = "MA_2:/tmp/TmpTestFMS"
DIR4 = "/home/capo-chichi/Telechargements/"
DIR5 = "MA_2:/tmp/TransferLib/"
DIR6 = "MA_2:/tmp/MoveAsyncTest/"
contentOfFile = "MA_2"

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
  r, k = VISHNU.connect("user_1", "toto")
  print "createFile:"
  VISHNU.createFile(k, path)
  print "sessionKey:", k
  print "path:", path
  print "==================getFilesInfo===============:"
  VISHNU.getFilesInfo(k, path2,  fileInfo)
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
  dirContent.getStrings().clear()
  print "createFile testCpFms:"
  VISHNU.createFile(k, path3)
  print "copyFile testGrp:"
  VISHNU.copyFile(k, path3, path4)
  print "===================listDir - LongFormat====================:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getStrings().clear()
  print "===================listDir - LongFormat with testGrp file====================:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getStrings().clear()
  print "chGrp:"
  VISHNU.chGrp(k, "adm", path3)
  print "===================listDir - LongFormat with change group on testGrp file============:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getStrings().clear()
  print "chMod:"
  VISHNU.chMod(k, 644, path4)
  print "===================listDir - LongFormat with change mode on testGrp file============:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getStrings().clear()
  print "moveFile"
  VISHNU.moveFile(k, path4, path5)
  print "===================listDir============:"
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #To clean the list
  dirContent.getStrings().clear()
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
  dirContent.getStrings().clear()
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
