
import VISHNU_UMS
import VISHNU_TMS
import time

def displayQueue (q):
  for i in range(q.getNbQueues()):
    print "Queue number ", i,":"
    print "Queue name ", q.getQueues().get(i).getName()
    print "Queue memory ", q.getQueues().get(i).getMemory()
    print "Queue desc ", q.getQueues().get(i).getDescription()

def displayJob(j, pos):
    print "jobinfo", pos,":"
    print "jobinfo id", j.getJobId()
    print "jobinfo name", j.getJobName()
    print "job status", j.getStatus()
    print "jobinfo submit date", j.getSubmitDate()

def displayProg(li):
  for i in range(li.getNbJobs()):
    print "progress number ", i, ":"
    print "progress jib", li.getProgress().get(i).getJobId()
    print "progress jname", li.getProgress().get(i).getJobName()
    print "progress jpercent", li.getProgress().get(i).getPercent()

def displayOut(jr):
  print "JobId: ", jr.getJobId()
  print "output: ", jr.getOutputPath()
  print "error: ", jr.getErrorPath()

def displayComp(lijr):
  for i in range(lijr.getNbJobs()):
    displayOut(lijr.getResults().get(i))
    print " "

def displayListJ(lij):
  for i in range(lij.getNbJobs()):
    displayJob(lij.getJobs().get(i), i)
    print " "

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
  VISHNU_TMS.listJobs(k, mid, lij, opj)
  VISHNU_TMS.listQueues(k, mid, q)
  VISHNU_TMS.getJobInfo(k, mid, j.getJobId(), j)
  time.sleep(5)
  VISHNU_TMS.getJobProgress(k, mid, li, opp)
  #status = 5 => TERMINATED
  print "waiting for job completion..."
  while j.getStatus() < 5:
    time.sleep(2)
    VISHNU_TMS.getJobInfo(k, mid, j.getJobId(), j)
  print "*****status=", j.getStatus()

  VISHNU_TMS.getJobOutput(k, mid, j.getJobId(), jr, out)
  VISHNU_TMS.getCompletedJobsOutput(k, mid, lijr, out)
  VISHNU_TMS.submitJob(k, mid, path, j, op)
  VISHNU_TMS.cancelJob(k, mid, j.getJobId())
except VISHNU_TMS.TMSVishnuException, e:
  print e.what()
except VISHNU_UMS.UMSVishnuException, e:
  print e.what()
except VISHNU_UMS.SystemException, e:
  print e.what()


print "@@@@@@@ List of queues @@@@@@@"
displayQueue(q)
print "@@@@@@@ job information @@@@@@"
displayJob(j,0)
print "@@@@@@@ List of jobs progression @@@@@@"
if li.getNbJobs() > 0:
  displayProg(li)
print "@@@@@@@ Result of job @@@@@@@@@@@@@"
displayOut(jr)
print "@@@@@@@ List of completed jobs @@@@@@@@@"
if lijr.getNbJobs() > 0:
  displayComp(lijr)
print "@@@@@@@@@ List of jobs @@@@@@@@@@"
if lij.getNbJobs() > 0:
  displayListJ(lij)

VISHNU_TMS.getJobProgress(k, mid, li, opp)

# command execfile("/home/keo/Bureau/depot_git_edf/vishnu/TMS/test/src/toto.py")
