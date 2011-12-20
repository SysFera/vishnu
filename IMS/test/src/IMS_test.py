import VISHNU
import os
from datetime import date

def displayProc(lip):
  for i in range(lip.getProcess().size()):
    print "Name: ", lip.getProcess().get(i).getProcessName()
    print "machine: ", lip.getProcess().get(i).getMachineId()
    print "Diet Id: ", lip.getProcess().get(i).getDietId()


def getMetricType(mtype):
  values = {
    1: "Free CPU (%)",
    2: "Free diskspace (Mb)",
    3: "Free memory (Mb)",
  }
  return values.get(mtype)
  
def displayMetric(lic, machineId):
  try :
    for i in range(lic.getMetric().size()):
      print "Type: ", getMetricType(lic.getMetric().get(i).getType())
      mtype = getMetricType(lic.getMetric().get(i).getType())
      print "Value: ", lic.getMetric().get(i).getValue()
      mvalue = lic.getMetric().get(i).getValue()
      mtime = (date.fromtimestamp(lic.getMetric().get(i).getTime())).strftime('%d-%b-%Y')
      print "time : ", mtime
       
  except VISHNU.VishnuException, e:
	  print e.what()

VISHNU.vishnuInitialize(os.getenv("VISHNU_CONFIG_FILE"))

mid = "machine_1"
pop = VISHNU.ProcessOp()
lip = VISHNU.ListProcesses()
lic = VISHNU.ListMetric()
opc = VISHNU.CurMetricOp()
lih = VISHNU.ListMetric()
oph = VISHNU.MetricHistOp()
sess = VISHNU.Session()

try :
  VISHNU.connect("root", "vishnu_user", sess)
  k = sess.getSessionKey()
  print "Session key: ",k 
  VISHNU.getProcesses(k, lip, pop)
  print "###################Processes#######################"
  displayProc(lip);
  
  print "###################Metrics#######################"
  for i in range(lip.getProcess().size()):
    VISHNU.getMetricCurrentValue(k, lip.getProcess().get(i).getMachineId(), lic, opc)
    displayMetric(lic, lip.getProcess().get(i).getMachineId())    
	
  VISHNU.close(k)
except VISHNU.VishnuException, e:
    print e.what()

