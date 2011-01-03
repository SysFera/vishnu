#include "VishnuException.hh"
#include "FMS.hh"

FMS::FMS(){
  e = FMSVishnuException();
  e.setType(VishnuException::FMS);
}

void FMS::copy(){
  e.setMsgComp ("The copy failed due to a diet error in toto");
  e.setMsg(0);
  throw e;
}

void FMS::remove(){
  e.setMsgComp ("The remove failed due to a bad given parameter");
  e.setMsg(1);
  throw e;
}
void FMS::chmod(){
  e.setMsgComp ("The chmod failed due to an invalid component");
  e.setMsg(2);
  throw e;
}
void FMS::chown(){
  e.setMsgComp ("The chown failed due to missing rights in the database");
  e.setMsg(3);
  throw e;
}
void FMS::advancement(){
  e.setMsgComp ("The syscall failed to get the advancement");
  e.setMsg(4);
  throw e;
}
void FMS::call (int val){
  switch (val){
  case 0 :
    try{
      copy();
    }
    catch (FMSVishnuException& e){
      throw e;
    }
    break;
  case 1 :
    try{
      remove();
    }
    catch (FMSVishnuException& e){
      throw e;
    }
    break;
  case 2 :
    try{
      chmod();
    }
    catch (FMSVishnuException& e){
      throw e;
    }
    break;
  case 3 :
    try{
      chown();
    }
    catch (FMSVishnuException& e){
      throw e;
    }
    break;
  case 4 :
    try{
      advancement();
    }
    catch (FMSVishnuException& e){
      throw e;
    }
    break;
  }
}
