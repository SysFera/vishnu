#
# Small python example of how to use the API for the UMS module
# Date 04/03/2011
# Author : B. Isnard, K. Coulomb
#

### UMS examples

# Import the UMS package
import VISHNU
import os


def displayAuthAcc(newAuthAcc):
  print "AuthSystemId: ", newAuthAcc.getAuthSystemId()
  print "AcLogin: ", newAuthAcc.getAcLogin()
  
def displayAuthAccounts(li):
  print "li : size()", len(li[1])
  for i in range(len(li[1])):    
    displayAuthAcc(li[1][i])
    print " " 
    
def displayAuthSys(newAuthSys):
  print "AuthSystemId: ", newAuthSys.getAuthSystemId()
  print "Name: ", newAuthSys.getName()
  print "URI: ", newAuthSys.getURI()
  print "AuthLogin: ", newAuthSys.getAuthLogin()
  print "AuthPassword: ", newAuthSys.getAuthPassword()
  print "UserPasswordEncryption: ", newAuthSys.getUserPasswordEncryption()
  print "Type: ", newAuthSys.getType()
  print "LdapBase: ", newAuthSys.getLdapBase()
 
def displayAuthSystems(li):
  print "length li:", len(li[1])
  for i in range(len(li[1])):    
    displayAuthSys(li[1][i])
    print " "  
 
try:
  sess = VISHNU.Session()
  # Initialisation of vishnu, the parameter is the path to the configuration file (cf shell script for the content of this file)
  VISHNU.vishnuInitialize(os.getenv("VISHNU_CONFIG_FILE"))
  # Connection of the user root with the password vishnu_user, getting in return the error code r and the session key k
  VISHNU.connect("root", "vishnu_user", sess)
  k = sess.getSessionKey()
  print "Session key: ",k

  # Creation of a listsession object
  #l = VISHNU.ListSessions()

  # Calling the vishnu service listSession for the session key k, filling the list l
  res = VISHNU.listSessions(k)
  # Getting the size of the list
  #l.getSessions().size()
  # Getting the userid of the 24th session of the list
  print "Getting the userid of the first session of the list:", res[1][0].getUserId()
  # Getting the session id of the 24th session of the list
  print "Getting the session id of the first session of the list:", res[1][0].getSessionId()
  id = res[1][0].getSessionId()
  # Creating a user object
  u = VISHNU.User()
  # Setting the user namefirst
  u.setFirstname("Benjamin")
  # Setting the user lastname
  u.setLastname("Isnard")
  # Setting the user email
  u.setEmail("benjamin.isnard@sysfera.com")
  # Calling the vishnu add user service
  VISHNU.addUser(k,u)
  # Getting the user privilege
  u.getPrivilege()
  # Getting the password
  u.getPassword()
  # Setting the status of the user as not admin
  u.setStatus(0)
  # Calling the vishnu service to update the user u
  VISHNU.updateUser(k,u)
  # Creation a list of local account
  #lacc = VISHNU.ListLocalAccounts()
  # Listing the account
  lacc = VISHNU.listLocalAccounts(k)
  # Getting the account in the first position in the list
  print "Number of local accounts", len(lacc[1])
  if len(lacc[1]) > 0:
    print "Getting the account in the first position in the list", "userId:", lacc[1][0].getUserId(), "machineId:", lacc[1][0].getMachineId() 
    
  # Creation of a list machine object
  #l = VISHNU.ListMachines()
  # Listing machines
  #VISHNU.listMachines(k,l)
  # Creation of the option for the machines
  op = VISHNU.ListMachineOptions()
  # Setting the option to list all machines
  op.setListAllMachine(1)
  # Listing with the options
  l = VISHNU.listMachines(k,op)
  # Showing the name of the first machine
  if len(l[1]) > 0:
    print "first machine name:", l[1][0].getName()
  # Reconnecting to the previous session of the list
  k =VISHNU.reconnect("root","vishnu_user",id, sess)
  k = sess.getSessionKey()
  print "Session key: ",k
  # Closing the connection
  VISHNU.close(k)

  # Test connection using account on the .netrc file
  print "Test connection using the .netrc file"
  sess2 = VISHNU.Session()
  VISHNU.connect("","", sess2)
  k = sess2.getSessionKey()
  print "Session key: ",k
  # Test addAuthSystem
  print "Test addAuthSystem"
  newAuthSys = VISHNU.AuthSystem()
  newAuthSys.setName("MyPython")
  newAuthSys.setURI("http://pythonHome.Fr")
  newAuthSys.setAuthLogin("myPythonAuth")
  newAuthSys.setAuthPassword("myPythonPwd")
  newAuthSys.setUserPasswordEncryption(0)
  newAuthSys.setType(0)
  newAuthSys.setLdapBase("uid=$USERNAME,myLdapBase,dc=sysfera,dc=com")
  VISHNU.addAuthSystem(k, newAuthSys)
  
  print "Test listAuthSystems"
  #li = VISHNU.ListAuthSystems()
  liopt = VISHNU.ListAuthSysOptions()
  liopt.setListAllAuthSystems(1)
  li = VISHNU.listAuthSystems(k,liopt)
  #li = VISHNU.listAuthSystems(k)
  displayAuthSystems(li)
  
  #displayAuthSys(newAuthSys)
  print "Test updateAuthSystem : name modified to MyPythonUpdated"
  newAuthSys.setName("MyPythonUpdated")
  VISHNU.updateAuthSystem(k, newAuthSys)
  
  print "Test listAuthSystems"
  #li = VISHNU.ListAuthSystems()
  #liopt = VISHNU.ListAuthSysOptions()
  #liopt.setListAllAuthSystems(1)
  #li = VISHNU.listAuthSystems(k, liopt)
  li = VISHNU.listAuthSystems(k)
  displayAuthSystems(li)
  
  print "Test deleteAuthSystem"
  print "AUTH_ID", newAuthSys.getAuthSystemId()
  VISHNU.deleteAuthSystem(k, newAuthSys.getAuthSystemId())
  #li = VISHNU.ListAuthSystems()
  #liopt = VISHNU.ListAuthSysOptions()
  #liopt.setListAllAuthSystems(1)
  #li = VISHNU.listAuthSystems(k, liopt)
  li = VISHNU.listAuthSystems(k)
  displayAuthSystems(li)
  
  print "Test addAuthAccount"
  VISHNU.addAuthSystem(k, newAuthSys)
  newAuthAcc = VISHNU.AuthAccount()
  newAuthAcc.setAuthSystemId(newAuthSys.getAuthSystemId())
  newAuthAcc.setAcLogin("MyPythonAcLogin")
  VISHNU.addAuthAccount(k, newAuthAcc)
  #liacc = VISHNU.ListAuthAccounts()
  liacc = VISHNU.listAuthAccounts(k)
  displayAuthAccounts(liacc)
  
  print "Test updateAuthAccount"
  liacc = []
  newAuthAcc.setAcLogin("AcLoginUpdated")
  VISHNU.updateAuthAccount(k, newAuthAcc)
  liacc = VISHNU.listAuthAccounts(k)
  displayAuthAccounts(liacc)
  
  print "Test deleteAuthAccount"
  liacc = []
  VISHNU.deleteAuthAccount(k,newAuthSys.getAuthSystemId())
  liacc = VISHNU.listAuthAccounts(k)
  displayAuthAccounts(liacc)
  
  VISHNU.deleteAuthSystem(k, newAuthSys.getAuthSystemId())
  
  print "Test Connect multi-users"
  lu = [] #= VISHNU.ListUsers()
  #lu = VISHNU.listUsers(k)
  sesslu = VISHNU.Session()
  #VISHNU.connect_m(lu, sesslu)
  #print "Session key: ",sesslu.getSessionKey()
  
  #lu.append(u)
  #VISHNU.connect_m(lu, sesslu)
  #print "Session key: ",sesslu.getSessionKey()
  u1 = VISHNU.User()
  
  u2 = VISHNU.User()
  u2.setUserId("root")
  u2.setPassword("vishnu_user")
  
  lu.append(u1)
  lu.append(u2)
  VISHNU.connect_m(lu, sesslu)
  #print lu
  print "Session key: ",sesslu.getSessionKey()
  
  
  #User u
  #u = VISHNU.User()
  #u.setUserId("s")
  #u.setPassword("s")
  #lu.getUsers().push_back(u)
  
  #User u1
  #u1 = VISHNU.User()
  #u1.setUserId("Leo")
  #u1.setPassword("Cleo")
  #lu.getUsers().push_back(u1)
  
  #User u2
  #u2 = VISHNU.User()
  #u2.setUserId("root")
  #u2.setPassword("vishnu_user")
  #lu.getUsers().push_back(u2)
  #sesslu = VISHNU.Session()
  #VISHNU.connect(lu, sesslu)
    
  #try:
    #sesslu = VISHNU.Session()
    #VISHNU.connect(lu, sesslu)
   # print "Test Multiple connect"
  #except VISHNU.VishnuException, e:
   # print e.what()
   
  VISHNU.close(k)


except VISHNU.VishnuException, e:
    print e.what()

