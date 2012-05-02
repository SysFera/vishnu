
import VISHNU
import time
import os
import sys


def displayQueue (q):
  for i in range(len(q[1])):
    print "Queue number ", i,":"
    print "Queue name ", q[1][i].getName()
    print "Queue memory ", q[1][i].getMemory()
    print "Queue desc ", q[1][i].getDescription()

def displayJob(j, pos):
    print "jobinfo", pos,":"
    print "jobinfo id", j.getJobId()
    print "jobinfo name", j.getJobName()
    print "job status", j.getStatus()
    print "jobinfo submit date", j.getSubmitDate()
    print "machine id", j.getSubmitMachineId()

def displayProg(li):
  for i in range(len(li[1])):
    print "progress number ", i, ":"
    print "progress jib", li[1][i].getJobId()
    print "progress jname", li[1][i].getJobName()
    print "progress jpercent", li[1][i].getPercent()

def displayOut(jr):
  print "JobId: ", jr.getJobId()
  print "output: ", jr.getOutputPath()
  print "error: ", jr.getErrorPath()

def displayComp(lijr):
  for i in range(len(lijr[1])):
    displayOut(lijr[1][i])
    print " "

def displayListJ(lij):
  for i in range(len(lij[1])):
    displayJob(lij[1][i], i)
    print " "

VISHNU.vishnuInitialize(os.getenv("VISHNU_CONFIG_FILE"))

j = VISHNU.Job()
j1 = VISHNU.Job()
j2 = VISHNU.Job()
j3 = VISHNU.Job()
criterion = VISHNU.LoadCriterion()
op = VISHNU.SubmitOptions()
mid = "machine_1"
mid2 = "machine_2"
#path = "/home/traore/VishnuProject/vishnu/TMS/test/src/scripts/torque_script"
#path = "scripts/torque_script"
path = "scripts/testScriptGen"
waitingScriptPath = "scripts/testScriptGenWaiting"

#The first argument in command line corresponds to the machine id
if len(sys.argv) > 1:
  mid = sys.argv[1]
#The second argument in command line corresponds to the second machine id
if len(sys.argv) > 2:
  mid2 = sys.argv[2]
#The second argument in command line corresponds to the waiting job script path
if len(sys.argv) > 3:
  waitingScriptPath = sys.argv[3]

sess = VISHNU.Session()
r = VISHNU.connect("root", "vishnu_user", sess)
k = sess.getSessionKey()
#q = VISHNU.ListQueues()
q = []
opp = VISHNU.ProgressOptions()
#li = VISHNU.ListProgression()
jr = VISHNU.JobResult()
#lijr = VISHNU.ListJobResults()
out = "/tmp"
#lij = VISHNU.ListJobs()
opj = VISHNU.ListJobsOptions()

try :
  VISHNU.submitJob(k, mid, path, j, op)
  lij = VISHNU.listJobs(k, mid, opj)
  q = VISHNU.listQueues(k, mid)
  VISHNU.getJobInfo(k, mid, j.getJobId(), j)
  time.sleep(5)
  li = VISHNU.getJobProgress(k, mid, opp)
  #status = 5 => TERMINATED
  print "waiting for job completion..."
  while j.getStatus() < 5:
    time.sleep(2)
    VISHNU.getJobInfo(k, mid, j.getJobId(), j)
  print "*****status=", j.getStatus()

  VISHNU.getJobOutput(k, mid, j.getJobId(), jr, out)
  lijr = VISHNU.getCompletedJobsOutput(k, mid, out)
  VISHNU.submitJob(k, mid, path, j, op)
  VISHNU.cancelJob(k, mid, j.getJobId()) 

  print "@@@@@@@ List of queues @@@@@@@"
  displayQueue(q)
  print "@@@@@@@ job information @@@@@@"
  displayJob(j,0)
  print "@@@@@@@ List of jobs progression @@@@@@"
  if len(li[1]) > 0:
    displayProg(li)
  print "@@@@@@@ Result of job @@@@@@@@@@@@@"
  displayOut(jr)
  print "@@@@@@@ List of completed jobs @@@@@@@@@"
  if len(lijr[1]) > 0:
    displayComp(lijr)
  print "@@@@@@@@@ List of jobs @@@@@@@@@@"
  if len(lij[1]) > 0:
    displayListJ(lij)

  VISHNU.submitJob(k, mid, path, j, op)
  print "submit job on machine named : ", mid2
  VISHNU.submitJob(k, mid2, path, j, op)
  ALL="all"
  #allJobs=VISHNU.ListJobs()
  allJobs = VISHNU.listJobs(k, ALL, opj)
  print "@@@@@@@@@ List of jobs on all machines @@@@@@@@@@"
  if len(allJobs[1]) > 0:
    displayListJ(allJobs)
  
  AUTOM="autom"
  VISHNU.submitJob(k, mid, waitingScriptPath, j1, op)
  VISHNU.submitJob(k, mid, waitingScriptPath, j2, op)
  VISHNU.submitJob(k, mid, waitingScriptPath, j3, op)
  print "++++++++++++++++++++Three submitted waiting jobs on:", mid
  VISHNU.submitJob(k, AUTOM, path, j, op)
  print "++++++++++++++++++++Automatic job id is:", j.getSubmitMachineId() 
  VISHNU.submitJob(k, AUTOM, path, j, op)
  print "++++++++++++++++++++Automatic job id is:", j.getSubmitMachineId()
  
  VISHNU.cancelJob(k, mid, j1.getJobId())
  VISHNU.cancelJob(k, mid, j2.getJobId())
  VISHNU.cancelJob(k, mid, j3.getJobId())

  #VISHNU.submitJob(k, mid2, path, j1, op)
  #VISHNU.submitJob(k, mid2, path, j2, op)
  #criterion.setLoadType(3)
  #op.setCriterion(criterion)
  #print "++++++++++++++++++++Two running jobs on:", mid2
  #VISHNU.submitJob(k, AUTOM, path, j, op)
  #print "++++++++++++++++++++Automatic2 job id is:", j.getSubmitMachineId()
  
  #VISHNU.cancelJob(k, mid, j1.getJobId())
  #VISHNU.cancelJob(k, mid, j2.getJobId())
#VISHNU.getJobProgress(k, mid, li, opp)

except VISHNU.VishnuException, e:
  print e.what()

# command execfile("/home/keo/Bureau/depot_git_edf/vishnu/TMS/test/src/toto.py")
