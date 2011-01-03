#include "VishnuException.hh"
#include "UMS.hh"

UMS::UMS(){
  e = new UMSVishnuException();
  e->setType(VishnuException::UMS);
}
void UMS::connect(){
  e->setMsgComp ("The connection failed with a database error code 666. ");
  e->setMsg(1);
  throw e;
}

void UMS::disconnect(){
  e->setMsgComp ("The disconnection failed with a diet error code 69. ");
  e->setMsg(2);
  throw e;
}

void UMS::displaySession(){
  e->setMsgComp ("The session display failed because of a bad parameter. ");
  e->setMsg(4);
  throw e;
}

void UMS::addMachine(){
  e->setMsgComp ("The add machine failed because a malloc failed. ");
  e->setMsg(0);
  throw e;
}

void UMS::removeMachine(){
  e->setMsgComp ("The remove failed because the component toto is invalid. ");
  e->setMsg(3);
  throw e;
}
 
void UMS::saveConfiguration(){
  e->setMsgComp ("The save failed because no space left on device. ");
  e->setMsg(0);
  throw e;
}
 
void UMS::displayLog(){
  e->setMsgComp ("The display log failed because the diet call to get log failed. ");
  e->setMsg(2);
  throw e;
}
 
void UMS::call (int val){
  
  switch (val){
  case 0 :
    try{
      connect();
    }
    catch (UMSVishnuException* e){
      e->appendMsgComp ("In the call \\o/ \n");
      throw e;
    }
    break;
  case 1 :
    try{
      disconnect();
    }
    catch (UMSVishnuException* e){
      e->appendMsgComp ("In the call \\o/ \n");
      throw e;
    }
    break;
  case 2 :
    try{
      displayLog();
    }
    catch (UMSVishnuException* e){
      e->appendMsgComp ("In the call \\o/ \n");
      throw e;
    }
    break;
  case 3 :
    try{
      addMachine();
    }
    catch (UMSVishnuException* e){
      e->appendMsgComp ("In the call \\o/ \n");
      throw e;
    }
    break;
  case 4 :
    try{
      removeMachine();
    }
    catch (UMSVishnuException* e){
      e->appendMsgComp ("In the call \\o/ \n");
      throw e;
    }
    break;
  case 5 :
    try{
      saveConfiguration();
    }
    catch (UMSVishnuException* e){
      e->appendMsgComp ("In the call \\o/ \n");
      throw e;
    }
    break;
  case 6 :
    try{
      displaySession();
    }
    catch (UMSVishnuException* e){
      e->appendMsgComp ("In the call \\o/ \n");
      throw e;
    }
    break;
  }
}

