/**
 * @file   SharedLibrary.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  SharedLibrary generic implementation
 * @section License
 *   |LICENSE|
 *
 */

#include "SharedLibrary.hh"
#if defined(WIN32)
#include "SharedLibraryImpl_win32.hh"
#else /* WIN32 */
#include "SharedLibraryImpl_posix.hh"
#endif

namespace dadi {

SharedLibrary::SharedLibrary() {}

SharedLibrary::SharedLibrary(const std::string& path)
  : pimpl(new SharedLibraryImpl(path)) {}

SharedLibrary::~SharedLibrary() {}

void
SharedLibrary::load(const std::string& path) {
  pimpl->loadImpl(path);
}

void
SharedLibrary::unload() {
  pimpl->unloadImpl();
}

bool
SharedLibrary::isLoaded() const {
  return pimpl->isLoadedImpl();
}

bool
SharedLibrary::hasSymbol(const std::string& symbol) {
  return pimpl->hasSymbolImpl(symbol);
}


void *
SharedLibrary::symbol(const std::string& symbol) {
  return pimpl->symbolImpl(symbol);
}

const std::string&
SharedLibrary::path() const {
  return pimpl->pathImpl();
}

std::string
SharedLibrary::prefix() {
  return MODULE_PREFIX;
}

std::string
SharedLibrary::suffix() {
  return MODULE_SUFFIX;
}

} /* namespace dadi */
