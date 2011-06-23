
import VISHNU

def displayProc(lip):
  for i in range(lip.getProcess().size()):
    print "Name: ", lip.getProcess().get(i).getProcessName()
    print "machine: ", lip.getProcess().get(i).getMachineId()



VISHNU.vishnuInitialize("/home/keo/Bureau/client.cfg")

mid = "machine_1"
pop = VISHNU.ProcessOp()
lip = VISHNU.ListProcesses()
lic = VISHNU.ListMetric()
opc = VISHNU.CurMetricOp()
lih = VISHNU.ListMetric()
oph = VISHNU.MetricHistOp()

r, k = VISHNU.connect("root", "vishnu_user")
try :
  VISHNU.getProcesses(k, lip, pop)
#  VISHNU.getMetricHistory(k, mid, lih, oph)
except VISHNU.IMSVishnuException, e:
    print e.what()
except VISHNU.UMSVishnuException, e:
  print e.what()
except VISHNU.SystemException, e:
  print e.what()
#VISHNU.getMetricCurrentValue(k, mid, lic, opc)

#try :
#  VISHNU.getMetricCurrentValue(k, "toto", lic, opc)
#except VISHNU.IMSVishnuException, e:
#    print e.what()
#except VISHNU.UMSVishnuException, e:
#  print e.what()
#except VISHNU.SystemException, e:
#  print e.what()

displayProc(lip);

# command execfile("/home/keo/Bureau/depot_git_edf/vishnu/IMS/test/src/IMS_test.py")

