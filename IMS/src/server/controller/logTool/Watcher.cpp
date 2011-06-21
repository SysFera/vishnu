#include "Watcher.hpp"
#include "ToolFactory.hpp"

Watcher::Watcher(int toolT, int argc, char** argv, string mid){
  mtool = ToolFactory::getInstance()->getTool(toolT, argc, argv, mid);
}

Watcher::~Watcher(){
  if (mtool){
    delete mtool;
  }
}

void 
Watcher::run(){
  mtool->run();
}
