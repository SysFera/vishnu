import VISHNU
import os
import unittest




class testUMS(unittest.TestCase):
    def setUp(self):
        self.sess = VISHNU.Session()
        self.op = VISHNU.ConnectOptions()
        VISHNU.vishnuInitialize(os.getenv("VISHNU_CONFIG_FILE"))
        VISHNU.connect('root', 'vishnu_user', self.sess, self.op)
        self.k = self.sess.getSessionKey()

    def test_bad_key(self):
        try:
            VISHNU.listMachines(self.k+"b'ad")
        except VISHNU.VishnuException, e :
            self.assertEqual(28, e.getMsgI())
        try:
            VISHNU.listUsers(self.k+"b'ad")
        except VISHNU.VishnuException, e :
            self.assertEqual(28, e.getMsgI())

# Test add_machine in all way with sql injections
    def test_add_machine(self):
        ma = VISHNU.Machine()
        ma.setName("bunny")
        ma.setSite("test")
        ma.setLanguage("fr")
        ma.setMachineDescription("desc")
# Check we can add a machine 
        r = VISHNU.addMachine(self.k, ma)
        self.assertEqual(0, r)
        VISHNU.deleteMachine(self.k, ma.getMachineId())
# Check with bad session key : fail
        try:
            VISHNU.addMachine(self.k+"b'ad", ma)
        except VISHNU.VishnuException, e :
            self.assertEqual(28, e.getMsgI())
# For each in param check injection
        ma.setName("bunn'y")
        r = VISHNU.addMachine(self.k, ma)
        self.assertEqual(0, r)
        VISHNU.deleteMachine(self.k, ma.getMachineId())
        ma.setSite("t'est")
        r = VISHNU.addMachine(self.k, ma)
        self.assertEqual(0, r)
        VISHNU.deleteMachine(self.k, ma.getMachineId())
        ma.setLanguage("f'r")
        r = VISHNU.addMachine(self.k, ma)
        self.assertEqual(0, r)
        VISHNU.deleteMachine(self.k, ma.getMachineId())
        ma.setMachineDescription("d'esc")
        r = VISHNU.addMachine(self.k, ma)
        self.assertEqual(0, r)

# Test inserted objects contain quotes
        opm = VISHNU.ListMachineOptions()
        opm.setListAllMachine(1)
        r, li = VISHNU.listMachines(self.k, opm)
        tmp = li[len(li)-1]
        self.assertEqual(tmp.getMachineDescription(), "d'esc")
        self.assertEqual(tmp.getLanguage(), "f'r")
        self.assertEqual(tmp.getSite(), "t'est")
        self.assertEqual(tmp.getName(), "bunn'y")
        VISHNU.deleteMachine(self.k, ma.getMachineId())

# Test update_machine in all way with sql injections
    def test_update_machine(self):
        ma = VISHNU.Machine()
        ma.setName("bunny")
        ma.setSite("test")
        ma.setLanguage("fr")
        ma.setMachineDescription("desc")
        ma.setStatus(1)
        r = VISHNU.addMachine(self.k, ma)
# Check with bad session key : fail
        try:
            VISHNU.updateMachine(self.k+"b'ad", ma)
        except VISHNU.VishnuException, e :
            self.assertEqual(28, e.getMsgI())
# For each in param check injection
        ma.setName("bunn'y")
        r = VISHNU.updateMachine(self.k, ma)
        self.assertEqual(0, r)
        ma.setSite("t'est")
        r = VISHNU.updateMachine(self.k, ma)
        self.assertEqual(0, r)
        ma.setLanguage("f'r")
        r = VISHNU.updateMachine(self.k, ma)
        self.assertEqual(0, r)
        ma.setMachineDescription("d'esc")
        r = VISHNU.updateMachine(self.k, ma)
        self.assertEqual(0, r)

# Test inserted objects contain quotes
        opm = VISHNU.ListMachineOptions()
        opm.setListAllMachine(1)
        r, li = VISHNU.listMachines(self.k, opm)
        tmp = li[len(li)-1]
        self.assertEqual(tmp.getMachineDescription(), "d'esc")
        self.assertEqual(tmp.getLanguage(), "f'r")
        self.assertEqual(tmp.getSite(), "t'est")
        self.assertEqual(tmp.getName(), "bunn'y")
        VISHNU.deleteMachine(self.k, ma.getMachineId())
        # Test bad machine id
        try:
            ma.setMachineId("ba'd")
            r = VISHNU.updateMachine(self.k, ma)
        except VISHNU.VishnuException, e :
            self.assertEqual(32, e.getMsgI())

# Test delete_machine
    def test_delete_machine(self):
        ma = VISHNU.Machine()
        ma.setName("bunny")
        ma.setSite("test")
        ma.setLanguage("fr")
        ma.setMachineDescription("desc")
        ma.setStatus(1)
        r = VISHNU.addMachine(self.k, ma)
        r = VISHNU.deleteMachine(self.k, ma.getMachineId())
        r = VISHNU.addMachine(self.k, ma)
        try:
            r = VISHNU.deleteMachine(self.k+"ba'd", ma.getMachineId())
        except VISHNU.VishnuException, e:
            self.assertEqual(28, e.getMsgI())
        try:
            r = VISHNU.deleteMachine(self.k, ma.getMachineId()+"ba'd")
        except VISHNU.VishnuException, e:
            self.assertEqual(32, e.getMsgI())

# Test add_user in all way with sql injections
    def test_add_user(self):
        u = VISHNU.User()
        u.setFirstname("bob")
        u.setLastname("bob")
        u.setEmail("bob@mail.com")
        u.setPrivilege(0)
        r = VISHNU.addUser(self.k, u)
        self.assertEqual(0, r)
        VISHNU.deleteUser(self.k, u.getUserId())
# Check with bad session key : fail
        try:
            r = VISHNU.addUser(self.k+"ba'd", u)
        except VISHNU.VishnuException, e :
            self.assertEqual(28, e.getMsgI())
# Check for parameters
        u.setFirstname("bo'b")
        r = VISHNU.addUser(self.k, u)
        self.assertEqual(0, r)
        VISHNU.deleteUser(self.k, u.getUserId())
        u.setLastname("bo'b")
        r = VISHNU.addUser(self.k, u)
        self.assertEqual(0, r)
        VISHNU.deleteUser(self.k, u.getUserId())
# Invalid mail address
        u.setEmail("bo'b@mail.com")
        try:
            r = VISHNU.addUser(self.k, u)
        except VISHNU.VishnuException, e :
            self.assertEqual(27, e.getMsgI())

# Test update_user in all way with sql injections
    def test_update_user(self):
        u = VISHNU.User()
        u.setFirstname("bob")
        u.setLastname("bob")
        u.setEmail("bob@mail.com")
        u.setPrivilege(0)
        r = VISHNU.addUser(self.k, u)
        self.assertEqual(0, r)
# Check with bad session key : fail
        try:
            r = VISHNU.updateUser(self.k+"ba'd", u)
        except VISHNU.VishnuException, e :
            self.assertEqual(28, e.getMsgI())
# Check for parameters
        u.setFirstname("bo'b")
        r = VISHNU.updateUser(self.k, u)
        self.assertEqual(0, r)
        u.setLastname("bo'b")
        r = VISHNU.updateUser(self.k, u)
        self.assertEqual(0, r)
# Invalid mail address
        u.setEmail("bo'b@mail.com")
        try:
            r = VISHNU.updateUser(self.k, u)
        except VISHNU.VishnuException, e :
            self.assertEqual(27, e.getMsgI())
        VISHNU.deleteUser(self.k, u.getUserId())
        u.setEmail("bob@mail.com")
        u.setUserId("ba'd")
        try:
            r = VISHNU.updateUser(self.k, u)
        except VISHNU.VishnuException, e :
            self.assertEqual(21, e.getMsgI())


# Test delete_user in all way with sql injections
    def test_delete_user(self):
        u = VISHNU.User()
        u.setFirstname("bob")
        u.setLastname("bob")
        u.setEmail("bob@mail.com")
        u.setPrivilege(0)
        VISHNU.addUser(self.k, u)
        VISHNU.deleteUser(self.k, u.getUserId())
        VISHNU.addUser(self.k, u)
        try:
            VISHNU.deleteUser(self.k+"ba'd", u.getUserId())
        except VISHNU.VishnuException, e:
            self.assertEqual(28, e.getMsgI())
        try:
            VISHNU.deleteUser(self.k, u.getUserId()+"ba'd")
        except VISHNU.VishnuException, e:
            self.assertEqual(21, e.getMsgI())
        VISHNU.deleteUser(self.k, u.getUserId())

# Test define user format
    def test_define_user_id(self):
# Define a format with quote
        identifier = "$NAME'voila"
        VISHNU.defineUserIdentifier(self.k,identifier)
# Create a user
        u = VISHNU.User()
        u.setFirstname("bob")
        u.setLastname("'bob")
        u.setEmail("bob@mail.com")
        u.setPrivilege(0)
        r = VISHNU.addUser(self.k, u)
        self.assertEqual(0, r)
# Delete him
        VISHNU.deleteUser(self.k, u.getUserId())


        
if __name__ == '__main__':
    unittest.main(unittest.TestCase)

