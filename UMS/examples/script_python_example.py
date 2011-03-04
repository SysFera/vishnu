#
# Small python example of how to use the API for the UMS module
# Date 04/03/2011
# Author : B. Isnard, K. Coulomb
#


### UMS examples

# Import the UMS package
import VISHNU_UMS

# Initialisation of vishnu, the parameter is the path to the configuration file (cf shell script for the content of this file)
VISHNU_UMS.vishnuInitialize("~/test/vishnu/client.cfg")

# Connection of the user root with the password vishnu_user, getting in return the error code r and the session key k
r, k =VISHNU_UMS.connect("root","vishnu_user")

# Creation of a listsession object
l = VISHNU_UMS.ListSessions()

# Calling the vishnu service listSession for the session key k, filling the list l
VISHNU_UMS.listSessions(k,l)
# Getting the size of the list
l.getSessions().size()
# Getting the userid of the 24th session of the list
l.getSessions().get(24).getUserId()
# Getting the session id of the 24th session of the list
id = l.getSessions().get(24).getSessionId()

# Creating a user object
u = VISHNU_UMS.User()
# Setting the user namefirst
u.setFirstname("Benjamin")
# Setting the user lastname
u.setLastname("Isnard")
# Setting the user email
u.setEmail("benjamin.isnard@sysfera.com")
# Calling the vishnu add user service
VISHNU_UMS.addUser(k,u)
# Getting the user privilege
u.getPrivilege()
# Getting the password 
u.getPassword()
# Setting the status of the user as not admin
u.setStatus(0)
# Calling the vishnu service to update the user u
VISHNU_UMS.updateUser(k,u)

# Creation a list of local account
l = VISHNU_UMS.ListLocalAccounts()
# Listing the account
VISHNU_UMS.listLocalAccount(k,l)
# Getting the account in the first position in the list
l.getAccounts().get(1)

# Creation of a list machine object
l = VISHNU_UMS.ListMachines()
# Listing machines 
VISHNU_UMS.listMachine(k,l)
# Creation of the option for the machines
op = VISHNU_UMS.ListMachineOptions()
# Setting the option to list all machines
op.setListAllmachine(1)
# Listing with the options
VISHNU_UMS.listMachine(k,l,op)
# Showing the name of the first machine
l.getMachines().get(1).getName()


# Reconnecting to the 24th session of the list, getting the return code and the session key of the 24th session of the list
r, k =VISHNU_UMS.reconnect("root","vishnu_user",id)
# Closing the connection to the 24th session
VISHNU_UMS.close(k)


