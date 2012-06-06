import unittest
import VISHNU
import os	

#sudo apt-get install python-nose
# nosetests -v  --with-xunit testPyUnit.py
class TestSequenceFunctions(unittest.TestCase):

  #def setUp(self):
      #TODO check the SeDs omniNames,  etc 

  def test_connection(self):
      sess = VISHNU.Session()
      VISHNU.vishnuInitialize(os.getenv("VISHNU_CONFIG_FILE"))
      VISHNU.connect("root", "vishnu_user", sess)
      k = sess.getSessionKey()
      self.assertTrue(k != "")
      
  def test_connection_bad_name(self):
      sess = VISHNU.Session()
      k = sess.getSessionKey()
      VISHNU.vishnuInitialize(os.getenv("VISHNU_CONFIG_FILE"))   
      l = ["bad", "vishnu_user", sess]
      self.assertRaises(VISHNU.VishnuException, VISHNU.connect, *l)
       
if __name__ == '__main__':
    unittest.main()

     