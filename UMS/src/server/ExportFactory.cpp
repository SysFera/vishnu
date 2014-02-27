#include "ExportFactory.hpp"
#include "export/shell/ShellExporter.hpp"

ExportFactory::ExportFactory() {
}

ExportFactory::~ExportFactory() {
}

ExportServer*
ExportFactory::getExporter(UserServer u) {
  return new ShellExporter(u);
}
