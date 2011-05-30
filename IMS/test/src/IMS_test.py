
import VISHNU_UMS
import VISHNU_IMS

def displayProc(lip):
  for i in range(lip.getNbProcs()):
    print "Name: ", lip.getProcessName()
    print "machine: ", lip.getMachineId()



VISHNU_UMS.vishnuInitialize("/home/keo/Bureau/client.cfg")

mid = "machine_1"
pop = VISHNU_IMS.ProcessOp()
lip = VISHNU_IMS.ListProcesses()
lic = VISHNU_IMS.ListMetric()
opc = VISHNU_IMS.CurMetricOp()
lih = VISHNU_IMS.ListMetric()
oph = VISHNU_IMS.MetricHistOp()

r, k = VISHNU_UMS.connect("root", "vishnu_user")
VISHNU_IMS.getMetricHistory(k, mid, lih, oph)
VISHNU_IMS.getMetricCurrentValue(k, mid, lic, opc)
VISHNU_IMS.getProcesses(k, lip, pop)

try :
  VISHNU_IMS.getMetricCurrentValue(k, "toto", lic, opc)
except VISHNU_IMS.IMSVishnuException, e:
    print e.what()
except VISHNU_UMS.UMSVishnuException, e:
  print e.what()
except VISHNU_UMS.SystemException, e:
  print e.what()

displayProc(lip);

# command execfile("/home/keo/Bureau/depot_git_edf/vishnu/IMS/test/src/IMS_test.py")

