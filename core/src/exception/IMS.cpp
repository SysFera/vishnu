#include "VishnuException.hpp"
#include "IMS.hpp"

IMS::IMS(){
}

void IMS::getCPU(){
  e.setMsgComp ("The get number of cpu failed due to a problem in the diet cori module");
  e.setMsg(4);
  throw e;
}
void IMS::getTMem(){
  e.setMsgComp ("The get total memory failed to get the result from the database with the error code 564");
  e.setMsg(4);

  throw e;
}
void IMS::getFMem(){
  e.setMsgComp ("Cannot get free memory due to the failure of a system call");
  e.setMsg(4);

  throw e;
}
void IMS::getTD(){
  e.setMsgComp ("Cannot get the total memory due to invalid parameter given");
  e.setMsg(4);
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
    catch (UserException& e){
      throw e;
    }
    break;
  case 1 :
    try{
      getFMem();
    }
    catch (UserException& e){
      throw e;
    }
    break;
  case 2 :
    try{
      getTMem();
    }
    catch (UserException& e){
      throw e;
    }
    break;
  case 3 :
    try{
      getTD();
    }
    catch (UserException& e){
      throw e;
    }
    break;
  case 4 :
    try{
      getFD();
    }
    catch (UserException& e){
      throw e;
    }
    break;

  }
}
