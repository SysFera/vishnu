#include "Watcher.hpp"
#include "ToolFactory.hpp"

Watcher::Watcher(int toolT, int argc, char** argv){
  mtool = ToolFactory::getInstance()->getTool(toolT, argc, argv);
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
