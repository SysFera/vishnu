#include "ToolFactory.hpp"
#include "SystemException.hpp"
#include "IMSVishnuTool.hpp"

ToolFactory* ToolFactory::mfact = NULL;

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
  switch (type){
  case IMSVishnuTool_v1:
    return new IMSVishnuTool(argc, argv, mid);
  default:
    break;
  }
  throw SystemException(ERRCODE_SYSTEM, "Unknown tool type");
  return NULL;
}
