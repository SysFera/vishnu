
import VISHNU_UMS
import VISHNU_FMS
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

VISHNU_UMS.vishnuInitialize("/home/traore/MYCONFIGFILES/client.cfg")

path = "MA_2:testFMSpython"
DIR = "MA_2:testFMSDirpython"
path2 = "MA_2:/home/traore/VishnuProject/vishnu/README"
DIR2 = "MA_2:/home/traore/VishnuProject/vishnu/"
path3 = "MA_2:/home/traore/testCpFms"
path4 = "MA_2:TmpTestFMS/testGrp"
DIR3 = "MA_2:TmpTestFMS"
headOpt = VISHNU_FMS.HeadOfFileOptions()
tailOpt = VISHNU_FMS.TailOfFileOptions()
dirContent = VISHNU_FMS.DirEntryList()
dirContent2 = VISHNU_FMS.DirEntryList()
lsOpt = VISHNU_FMS.LsDirOptions()
contentOfFile = "MA_2"
fileInfo = VISHNU_FMS.FileStat()
mode = 755

try :
  r, k = VISHNU_UMS.connect("root", "vishnu_user")
  VISHNU_FMS.createFile(k, path)
 
  VISHNU_FMS.getFilesInfo(k, path2,  fileInfo) 
  displayFileInfo(fileInfo)

  VISHNU_FMS.removeFile(k, path)
  
  VISHNU_FMS.createDir(k, DIR)
  
  VISHNU_FMS.removeDir(k, DIR)
  
  VISHNU_FMS.listDir(k, DIR2, dirContent)
  displayDirContent(dirContent)
  
  lsOpt.setLongFormat(True)
  VISHNU_FMS.listDir(k, DIR2, dirContent2, lsOpt)
  displayDirContent(dirContent2)

  #dispalyContentOfFile(contentOfFile)
  print "Before:"
  #r, contentOfFile = VISHNU_FMS.headOfFile(k, path2)
  print "After:"
  dispalyContentOfFile(contentOfFile)


  dispalyContentOfFile(contentOfFile)
  #VISHNU_FMS.tailOfFile(k, path2, contentOfFile)
  dispalyContentOfFile(contentOfFile)

  #VISHNU_FMS.ContentOfFile(k, path2, contentOfFile)
  dispalyContentOfFile(contentOfFile)

  VISHNU_FMS.copyFile(k, path2, path3) 
  
  VISHNU_FMS.moveFile(k, path3, path4)

  VISHNU_FMS.chGrp(k, "adm", path4)

  VISHNU_FMS.chMod(k, 755, path4)

except VISHNU_FMS.FMSVishnuException, e:
  print e.what()
except VISHNU_UMS.UMSVishnuException, e:
  print e.what()
except VISHNU_UMS.SystemException, e:
  print e.what()
except VISHNU_UMS.UserException, e:
  print e.what()

VISHNU_UMS.close(k)
