
import VISHNU_UMS
import VISHNU_TMS
import time

def displayQueue (q):
  print "Queue number 1:"
  print "Queue name ", q.getQueues().get(0).getName()
  print "Queue memory ", q.getQueues().get(0).getMemory()
  print "Queue desc ", q.getQueues().get(0).getDescription()
  print "Queue number 2:"
  print "Queue name ", q.getQueues().get(1).getName()
  print "Queue memory ", q.getQueues().get(1).getMemory()
  print "Queue desc ", q.getQueues().get(1).getDescription()

def displayJob(j):
  print "jobinfo"
  print "jobinfo id", j.getJobId()
  print "jobinfo name", j.getJobName()
  print "jobinfo submit date", j.getSubmitDate()

def displayProg(li):
  print "progress number 1:"
  print "progress jib", li.getProgress().get(0).getJobId()
  print "progress jname", li.getProgress().get(0).getJobName()
  print "progress jpercent", li.getProgress().get(0).getPercent()
#  print "progress number 2:"
#  print "progress jib", li.getProgress().get(1).getJobId()
#  print "progress jname", li.getProgress().get(1).getJobName()
#  print "progress jpercent", li.getProgress().get(1).getPercent()
#  print "progress number 3:"
#  print "progress jib", li.getProgress().get(2).getJobId()
#  print "progress jname", li.getProgress().get(2).getJobName()
#  print "progress jpercent", li.getProgress().get(2).getPercent()
#  print "progress number 4:"
#  print "progress jib", li.getProgress().get(3).getJobId()
#  print "progress jname", li.getProgress().get(3).getJobName()
#  print "progress jpercent", li.getProgress().get(3).getPercent()

def displayOut(jr):
  print "JobId: ", jr.getJobId()
  print "output: ", jr.getOutputPath()
  print "error: ", jr.getErrorPath()

def displayComp(lijr):
  displayOut(lijr.getResults().get(0))

def displayListJ(lij):
  displayJob(lij.getJobs().get(0))

VISHNU_UMS.vishnuInitialize("/home/keo/Bureau/depot_git_edf/vishnu/UMS/test/src/sql/client.cfg")

j = VISHNU_TMS.Job()
op = VISHNU_TMS.SubmitOptions()
mid = "machine_1"
path = "/home/keo/Bureau/launched"
r, k = VISHNU_UMS.connect("root", "vishnu_user")
q = VISHNU_TMS.ListQueues()
opp = VISHNU_TMS.ProgressOptions()
li = VISHNU_TMS.ListProgression()
jr = VISHNU_TMS.JobResult()
lijr = VISHNU_TMS.ListJobResults()
out = "/tmp"
lij = VISHNU_TMS.ListJobs()
opj = VISHNU_TMS.ListJobsOptions()

try :
  VISHNU_TMS.submitJob(k, mid, path, j, op)
  print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-3"
  VISHNU_TMS.listJobs(k, mid, lij, opj)
  print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-3.1"
  VISHNU_TMS.listQueues(k, mid, q)
  print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-2"
  VISHNU_TMS.getJobInfo(k, mid, j.getJobId(), j)
  print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-1"
  VISHNU_TMS.getJobProgress(k, mid, li, opp)
  print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@0"
  print "sleeping to wait for completion"
  time.sleep(20)
  VISHNU_TMS.getJobOutput(k, mid, j.getJobId(), jr, out)
  print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@0.1"
  VISHNU_TMS.getCompletedJobsOutput(k, mid, lijr, out)
  print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@0.2"
  VISHNU_TMS.submitJob(k, mid, path, j, op)
  print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@0.3"
  VISHNU_TMS.cancelJob(k, mid, j.getJobId())
  print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@0.4"
except VISHNU_TMS.TMSVishnuException, e:
  print e.what()
except VISHNU_UMS.UMSVishnuException, e:
  print e.what()
except VISHNU_UMS.SystemException, e:
  print e.what()


print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@1"
displayQueue(q)
print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2"
displayJob(j)
print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@3"
displayProg(li)
print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@4"
displayOut(jr)
print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@5"
displayComp(lijr)
print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@6"
displayListJ(lij)

VISHNU_TMS.getJobProgress(k, mid, li, opp)

# command execfile("/home/keo/Bureau/depot_git_edf/vishnu/TMS/test/src/toto.py")
