#include "data/ExportFactory.hpp"
#include "data/export/shell/ShellExporter.hpp"

ExportFactory::ExportFactory() {
}

ExportFactory::~ExportFactory() {
}

ExportServer*
ExportFactory::getExporter(UserServer u, IMS_Data::ExportOp_ptr expOp) {

  int exportType = -1;
  switch (expOp->getExportType()) {
  case 0 :
    exportType = 0;
    break;
  case 1 :
    exportType = 1;
    break;
  default:
    break;
  }
 
  if(exportType==0 || exportType==1) {
   return new ShellExporter(u, expOp);
  }

  throw UserException(ERRCODE_INVALID_PARAM, "Unknown export type");
}
