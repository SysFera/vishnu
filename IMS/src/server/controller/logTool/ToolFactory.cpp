#include "ToolFactory.hpp"
#include "SystemException.hpp"
#include "IMSVishnuTool.hpp"

//{{RELAX<MISRA_0_1_3> Because these variables are used this class
ToolFactory* ToolFactory::mfact = NULL;
//}}RELAX<MISRA_0_1_3>

ToolFactory::ToolFactory(){
}

ToolFactory::~ToolFactory(){
}

ToolFactory*
ToolFactory::getInstance(){
  if (!mfact){
    mfact = new ToolFactory();
  }
  return mfact;
}

LogTool*
ToolFactory::getTool(int type, int argc, char** argv, string mid){
  int toolType = -1;
  switch (type){
    case IMSVishnuTool_v1:
      toolType = IMSVishnuTool_v1;
      break;
    default:
      break;
  }

  if(toolType==IMSVishnuTool_v1) {
    return new IMSVishnuTool(argc, argv, mid);
  } else {
    throw SystemException(ERRCODE_SYSTEM, "Unknown tool type");
  }
}
