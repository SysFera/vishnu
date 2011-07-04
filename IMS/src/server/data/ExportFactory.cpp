#include "data/ExportFactory.hpp"
#include "data/export/shell/ShellExporter.hpp"

ExportFactory::ExportFactory() {
}

ExportFactory::~ExportFactory() {
}

ExportServer*
ExportFactory::getExporter(UserServer u, IMS_Data::ExportOp_ptr expOp) {

  switch (expOp->getExportType()) {
  case 0 :
  case 1 :
    return new ShellExporter(u, expOp);
  default:
    break;
  }
  throw UserException(ERRCODE_INVALID_PARAM, "Unknown export type");
}
