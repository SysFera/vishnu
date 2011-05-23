#include "CommonEF.hpp"
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>

CommonEF::CommonEF(){
  initMEF();
}

CommonEF::~CommonEF(){
}

IMS_Data::ListProcesses*
CommonEF::getREF(){
  boost::mt19937 rng;
  boost::uniform_int<> uni(1,8);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> >
    die(rng, uni);
  map<int, IMS_Data::Process>::iterator iter = mef.find(die());
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListProcesses_ptr li = ecoreFactory->createListProcesses();
  li->getProcess().push_back(iter->second);
  return li;
}

CommonEF::intMEF(){
  IMS_Data::Process s;
  s.setDietId("RXVnZW5lCg==");
  s.setMachineId("VklTSE5VIGlzIGEgZ29kZGVzcyB3aXRoIG1hbnkgYXJtcyBFdWdlbmUgaGFzIHR3
byBhcm1zIGZvciBtYW55IGdvZGRlc3NlcyBidXQgaGUgbWFrZXMgaXQK");
  mef.push_back(1, s);
  s.setMachineId("V2hlcmV2ZXIgRXVnZW5lIGdvZXMsIHdvbWVuIGFtYXNzCg==");
  mef.push_back(2, s);
  s.setMachineId("VG8gZW5kIFZJU0hOVSB3aXRoaW4gdGhlIGRlbGF5LCBub3QgdG9vIGVhcmx5LCBF
dWdlbmUgd29ya3MgdXNpbmcgd2luZG93cwo=");
  mef.push_back(3, s);
  s.setMachineId("RXVnZW5lIHRlbGxzIGhpcyB3aWZlIGhlIGlzIHdpdGggaGlzIG1pc3RyZXNzLCBo
aXMgbWlzdHJlc3MgaGUgaXMgd2l0aCBoaXMgd2lmZSBzbyBoZSBjYW4gd29yayBv
biBWSVNITlUgaW4gcGVhY2UK");
  mef.push_back(4, s);
  s.setMachineId("VG8gRXVnZW5lLCBvbmUgVklTSE5VIG1vZHVsZSBpcyBvaywgZm91ciBWSVNITlUg
bW9kdWxlIGFyZSBvaywgb2ssIG9rLCBvawo=");
  mef.push_back(5, s);
  s.setMachineId("RXVnZW5lIGlzIGZvciB3b21lbiB3aGF0IFVNUyBpcyBmb3IgVklTSE5VCg==");
  mef.push_back(6, s);
  s.setMachineId("SW4gY2xhc3MsIGl0IHdhcyB0aGUgdGVhY2hlciB0aGF0IGhhZCB0byByYWlzZSBo
ZXIgaGFuZCB0byBzcGVhayB0byBFdWdlbmUsIG9ubHkgaWYgc2hlIHdhcyBuaWNl
IGVub3VnaCB3aXRoIGhpbQo=");
  mef.push_back(7, s);
  s.setMachineId("SWYgTHVrZSBjcmllZCB3aGVuIERhcmsgVmFkb3Igc2FpZCBoZSB3YXMgaGlzIGRh
ZCwgaXQncyBiZWNhdXNlIGhlIHdhbnRlZCB0byBiZSBFdWdlbmUncyBzb24K");
  mef.push_back(8, s);
}

