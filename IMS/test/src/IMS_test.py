import VISHNU
import os
from datetime import date

def displayProc(lip):
  for i in range(len(lip[1])):
    print "Name: ", lip[1][i].getProcessName()
    print "machine: ", lip[1][i].getMachineId()
    print "Diet Id: ", lip[1][i].getDietId()


def getMetricType(mtype):
  values = {
    1: "Free CPU (%)",
    2: "Free diskspace (Mb)",
    3: "Free memory (Mb)",
  }
  return values.get(mtype)
  
def displayMetric(lic, machineId):
  try :
    for i in range(len(lic[1])):
      print "Type: ", getMetricType(lic[1][i].getType())
      mtype = getMetricType(lic[1][i].getType())
      print "Value: ", lic[1][i].getValue()
      mvalue = lic[1][i].getValue()
      mtime = (date.fromtimestamp(lic[1][i].getTime())).strftime('%d-%b-%Y')
      print "time : ", mtime
       
  except VISHNU.VishnuException, e:
	  print e.what()

VISHNU.vishnuInitialize(os.getenv("VISHNU_CONFIG_FILE"))

mid = "machine_1"
pop = VISHNU.ProcessOp()
lip = []
lic = []
opc = VISHNU.CurMetricOp()
lih = []
oph = VISHNU.MetricHistOp()
sess = VISHNU.Session()

try :
  VISHNU.connect("root", "vishnu_user", sess)
  k = sess.getSessionKey()
  print "Session key: ",k 
  lip = VISHNU.getProcesses(k, pop)
  print "###################Processes#######################"
  displayProc(lip);
  
  print "###################Metrics#######################"
  for i in range(len(lip[1])):
    lic = VISHNU.getMetricCurrentValue(k, lip[1][i].getMachineId(), opc)
    displayMetric(lic, lip[1][i].getMachineId())    
	
  VISHNU.close(k)
except VISHNU.VishnuException, e:
    print e.what()

