
import VISHNU
import time
import os

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

VISHNU.vishnuInitialize(os.getenv("VISHNU_CONFIG_FILE"))

j = VISHNU.Job()
op = VISHNU.SubmitOptions()
mid = "machine_1"
#path = "/home/traore/VishnuProject/vishnu/TMS/test/src/scripts/torque_script"
path = "scripts/torque_script"
sess = VISHNU.Session()
r = VISHNU.connect("root", "vishnu_user", sess)
k = sess.getSessionKey()
q = VISHNU.ListQueues()
opp = VISHNU.ProgressOptions()
li = VISHNU.ListProgression()
jr = VISHNU.JobResult()
lijr = VISHNU.ListJobResults()
out = "/tmp"
lij = VISHNU.ListJobs()
opj = VISHNU.ListJobsOptions()

try :
  VISHNU.submitJob(k, mid, path, j, op)
  VISHNU.listJobs(k, mid, lij, opj)
  VISHNU.listQueues(k, mid, q)
  VISHNU.getJobInfo(k, mid, j.getJobId(), j)
  time.sleep(5)
  VISHNU.getJobProgress(k, mid, li, opp)
  #status = 5 => TERMINATED
  print "waiting for job completion..."
  while j.getStatus() < 5:
    time.sleep(2)
    VISHNU.getJobInfo(k, mid, j.getJobId(), j)
  print "*****status=", j.getStatus()

  VISHNU.getJobOutput(k, mid, j.getJobId(), jr, out)
  VISHNU.getCompletedJobsOutput(k, mid, lijr, out)
  VISHNU.submitJob(k, mid, path, j, op)
  VISHNU.cancelJob(k, mid, j.getJobId())

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

#VISHNU.getJobProgress(k, mid, li, opp)

except VISHNU.VishnuException, e:
  print e.what()

# command execfile("/home/keo/Bureau/depot_git_edf/vishnu/TMS/test/src/toto.py")
