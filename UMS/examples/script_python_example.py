#
# Small python example of how to use the API for the UMS module
# Date 04/03/2011
# Author : B. Isnard, K. Coulomb
#

### UMS examples

# Import the UMS package
import VISHNU
import os

try:
  sess = VISHNU.Session()
  # Initialisation of vishnu, the parameter is the path to the configuration file (cf shell script for the content of this file)
  VISHNU.vishnuInitialize(os.getenv("VISHNU_CONFIG_FILE"))
  # Connection of the user root with the password vishnu_user, getting in return the error code r and the session key k
  VISHNU.connect("root", "vishnu_user", sess)
  k = sess.getSessionKey()
  print "Session key: ",k

  # Creation of a listsession object
  l = VISHNU.ListSessions()

  # Calling the vishnu service listSession for the session key k, filling the list l
  VISHNU.listSessions(k,l)
  # Getting the size of the list
  l.getSessions().size()
  # Getting the userid of the 24th session of the list
  print "Getting the userid of the first session of the list:", l.getSessions().get(0).getUserId()
  # Getting the session id of the 24th session of the list
  print "Getting the session id of the first session of the list:", l.getSessions().get(0).getSessionId()
  id = l.getSessions().get(1).getSessionId()
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
  lacc = VISHNU.ListLocalAccounts()
  # Listing the account
  VISHNU.listLocalAccounts(k,lacc)
  # Getting the account in the first position in the list
  print "Number of local accounts", lacc.getAccounts().size()
  if lacc.getAccounts().size() > 0:
    print "Getting the account in the first position in the list", lacc.getAccounts().get(0)

  # Creation of a list machine object
  l = VISHNU.ListMachines()
  # Listing machines
  VISHNU.listMachines(k,l)
  # Creation of the option for the machines
  op = VISHNU.ListMachineOptions()
  # Setting the option to list all machines
  op.setListAllmachine(1)
  # Listing with the options
  VISHNU.listMachines(k,l,op)
  # Showing the name of the first machine
  if l.getMachines().size() > 0:
    print "first machine name:", l.getMachines().get(0).getName()
  # Reconnecting to the previous session of the list
  k =VISHNU.reconnect("root","vishnu_user",id, sess)
  k = sess.getSessionKey()
  print "Session key: ",k
  # Closing the connection
  VISHNU.close(k)

except VISHNU.VishnuException, e:
    print e.what()

