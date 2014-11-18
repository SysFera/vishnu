#!/usr/bin/python

import mysql.connector
from mysql.connector import errorcode 
import sys

RUNNING_STATE = 4
QUEUED_STATE = 2

class DbHandler():

  def __init__(self):
    self.host = 'sop3'
    self.database = 'sysferads'
    self.user = 'sysferads_user'
    self.password = 'sysferads_db_pass'
    self.raise_on_warnings = True
    self.handler = None
    self.connected = False

    self.connect()


  def __del__(self):
    if self.connected:
      self.handler.close()
      self.connected = False



  def connect(self):
    self.connected = False
    try:
      self.handler = mysql.connector.connect(user=self.user,
                                         password=self.password,
                                         host=self.host,
                                         database=self.database,
                                         raise_on_warnings=self.raise_on_warnings)
      self.connected = True

    except mysql.connector.Error as err:
      if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
        print("Something is wrong with your user name or password")
      elif err.errno == errorcode.ER_BAD_DB_ERROR:
        print("Database does not exists")
      else:
        print(err)
    finally:
      if self.connected:
        print 'DEBUG: connected to database'
      else: 
        sys.exit('ERROR: Connection to database failed')





  def suspendJob(self, machineId, vmId):

    query = 'UPDATE job SET status=%s WHERE submitmachineid=\'%s\' AND vmId=\'%s\'' % (str(QUEUED_STATE), str(machineId), str(vmId))

    print 'DEBUG: executing query: %s ' % query

    cursor = self.handler.cursor()

    cursor.execute(query)

    self.handler.commit()



  def resumeJob(self, newMachineId, newVmId, oldMachineId, oldVmId):

    query = 'UPDATE job SET status=%s,submitmachineid=\'%s\',vmId=\'%s\' WHERE submitmachineid=\'%s\' AND vmId=\'%s\'' % (str(RUNNING_STATE),str(newMachineId), str(newVmId), str(oldMachineId), str(oldVmId))

    print 'DEBUG: executing query: %s ' % query
    
    cursor = self.handler.cursor()

    cursor.execute(query)

    self.handler.commit()

if __name__ == '__main__':

  SUSPEND_USAGE = '%s SUSPEND_TASK <machineId> <vmId>' % sys.argv[0]
  RESUME_USAGE = '%s RESUME_TASK <newMachineId> <newVmIid> <oldMachineId> <oldVmId>' % sys.argv[0]
  if len(sys.argv) <= 1:
    sys.exit('usage 1:\n\t%s SUSPEND_TASK <machineId> <vmId>\n'
    	     'usage 2:\n\t%s RESUME_TASK <newMachineId> <newVmIid> <oldMachineId> <oldVmId>' % (SUSPEND_USAGE, RESUME_USAGE))

  if sys.argv[1] == "SUSPEND_TASK":
    if len(sys.argv) != 4:
      sys.exit( SUSPEND_USAGE )
    else:
      machineId = sys.argv[2]
      vmId = sys.argv[3]
      dbHandler = DbHandler()
      dbHandler.suspendJob(machineId, vmId)
  elif sys.argv[1] == "RESUME_TASK":
    if len(sys.argv) != 6:
      sys.exit( RESUME_USAGE )
    else:
      newMachineId = sys.argv[2]
      newVmId = sys.argv[3]
      oldMachineId = sys.argv[4]
      oldVmId = sys.argv[5]   
      dbHandler = DbHandler()   
      dbHandler.resumeJob(newMachineId, newVmId, oldMachineId, oldVmId)
  else:
    sys.exit( 'Unknown operation {0}'.format(sys.argv[1]) )

      

  