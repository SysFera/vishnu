#include "TMS.hh"

TMS::TMS(){
}

void TMS::submitJob(){
  e.setMsgComp ("The submission failed with a database error code 666");
  e.setMsg(3);
  throw e;
}
void TMS::getJob(){
  e.setMsgComp ("Cannot get job on an invalid component");
  e.setMsg(4);
  throw e;
}

void TMS::cancelJob(){
  e.setMsgComp ("Cannot cancel a job because the kill failed");
  e.setMsg(5);
  throw e;
}
void TMS::listQueue(){
  e.setMsgComp ("Cannot list the given queue because the corresponding queue is unknown");
  e.setMsg(1);
  throw e;
}
void TMS::listJobs(){
  e.setMsgComp ("Cannot list jobs because the diet list job call returned a transient error");
  e.setMsg(2);
  throw e;
}
void TMS::call (int val){
  switch (val){
  case 0 :
    try{
      getJob();
    }
    catch (TMSVishnuException& e){
      throw e;
    }
    break;
  case 1 :
    try{
      listJobs();
    }
    catch (TMSVishnuException& e){
      throw e;
    }
    break;
  case 3 :
    try{
      cancelJob();
    }
    catch (TMSVishnuException& e){
      throw e;
    }
    break;
  case 4 :
    try{
      listQueue();
    }
    catch (TMSVishnuException& e){
      throw e;
    }
    break;
  case 2 :
    try{
      submitJob();
    }
    catch (TMSVishnuException& e){
      throw e;
    }
    break;

  }
}
