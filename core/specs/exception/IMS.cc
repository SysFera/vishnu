#include "VishnuException.hh"
#include "IMS.hh"

IMS::IMS(){
}

void IMS::getCPU(){
  e.setMsgComp ("The get number of cpu failed due to a problem in the diet cori module");
  e.setMsg(5);
  throw e;
}
void IMS::getTMem(){
  e.setMsgComp ("The get total memory failed to get the result from the database with the error code 564");
  e.setMsg(1);

  throw e;
}
void IMS::getFMem(){
  e.setMsgComp ("Cannot get free memory due to the failure of a system call");
  e.setMsg(2);

  throw e;
}
void IMS::getTD(){
  e.setMsgComp ("Cannot get the total memory due to invalid parameter given");
  e.setMsg(3);
  throw e;
}
void IMS::getFD(){
  e.setMsgComp ("Cannot get the free memory because the component is invalid");
  e.setMsg(4);

  throw e;
}
void IMS::call (int val){
  switch (val){
  case 0 :
    try{
      getCPU();
    }
    catch (IMSVishnuException& e){
      throw e;
    }
    break;
  case 1 :
    try{
      getFMem();
    }
    catch (IMSVishnuException& e){
      throw e;
    }
    break;
  case 2 :
    try{
      getTMem();
    }
    catch (IMSVishnuException& e){
      throw e;
    }
    break;
  case 3 :
    try{
      getTD();
    }
    catch (IMSVishnuException& e){
      throw e;
    }
    break;
  case 4 :
    try{
      getFD();
    }
    catch (IMSVishnuException& e){
      throw e;
    }
    break;

  }
}
