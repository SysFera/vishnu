
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

VISHNU.vishnuInitialize("/home/capo-chichi/Sysfera/vishnu/build/test_files/cfg/client_testing.cfg")

path = "machine_1:/tmp/testFMSpython"
DIR = "machine_1:/tmp/testFMSDirpython"
path2 = "machine_1:/tmp/omninames-Paco-Sysfera.log"
DIR2 = "machine_1:/tmp/"
path3 = "machine_1:/tmp/testCpFms"
path4 = "machine_1:/tmp/testGrp"
path5 = "machine_1:/tmp/testGrpMoved"
DIR3 = "machine_1:/tmp/TmpTestFMS"
headOpt = VISHNU.HeadOfFileOptions()
tailOpt = VISHNU.TailOfFileOptions()
dirContent = VISHNU.StringList()
dirContent2 = VISHNU.StringList()
lsOpt = VISHNU.LsDirOptions()
contentOfFile = "machine_1"
fileInfo = VISHNU.FileStat()
mode = 755

try :
  r, k = VISHNU.connect("root", "vishnu_user")
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
  print "createFile testCpFms:"
  VISHNU.createFile(k, path3)
  print "copyFile testGrp:"
  VISHNU.copyFile(k, path3, path4)
  print "===================listDir - LongFormat====================:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  print "===================listDir - LongFormat with testGrp file====================:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  print "chGrp:"
  VISHNU.chGrp(k, "adm", path3)
  print "===================listDir - LongFormat with change group on testGrp file============:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  #print "chMod:"
  #VISHNU.chMod(k, 755, path4) pb avec chMod
  print "===================listDir - LongFormat with change mode on testGrp file============:"
  lsOpt.setLongFormat(True)
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  print "moveFile"
  VISHNU.moveFile(k, path4, path5)
  print "===================listDir============:"
  VISHNU.listDir(k, DIR2, dirContent, lsOpt)
  displayDirContent(dirContent)
  print "===================contentOfFile======:"
  #r,contentOfFile=VISHNU.headOfFile(k, path2)
  #contentOfFile=VISHNU.tailOfFile(k, path2, contentOfFile)
  #dispalyContentOfFile(contentOfFile)
  print "removeFile:"
  VISHNU.removeFile(k, path3)
  VISHNU.removeFile(k, path5)

except VISHNU.FMSVishnuException, e:
  print e.what()
except VISHNU.UMSVishnuException, e:
  print e.what()
except VISHNU.SystemException, e:
  print e.what()
except VISHNU.UserException, e:
  print e.what()

VISHNU.close(k)

#execfile("/home/capo-chichi/Sysfera/vishnu/FMS/test/src/testFMS_Python.py")
#pbs : chMod : mode_t, headOfFile, contentOfFile, tailOfFile: Bad string mapping (solution cf Connect dans UMS)