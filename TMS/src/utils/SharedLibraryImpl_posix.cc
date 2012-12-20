/**
 * @file   SharedLibraryImpl_posix.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  SharedLibrary posix implementation
 * @section License
 *   |LICENSE|
 *
 */

#include "SharedLibraryImpl_posix.hh"
#include <dlfcn.h>

namespace dadi {

SharedLibraryImpl::SharedLibraryImpl() : handle_(0) {}

SharedLibraryImpl::SharedLibraryImpl(const std::string& path) : handle_(0) {
  loadImpl(path);
};

SharedLibraryImpl::~SharedLibraryImpl() {
  unloadImpl();
}

void
SharedLibraryImpl::loadImpl(const std::string& path) {
  // TODO: check handle_ is NULL or throw an exception
  handle_ = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);

  path_ = path;
}

void
SharedLibraryImpl::unloadImpl() {
  if (handle_) {
    dlclose(handle_);
    handle_ = 0;
  }
  path_.clear();
}

bool
SharedLibraryImpl::isLoadedImpl() const {
  return (0 != handle_);
}


bool
SharedLibraryImpl::hasSymbolImpl(const std::string& symbol) {
  return (0 != symbolImpl(symbol));
}


void *
SharedLibraryImpl::symbolImpl(const std::string& symbol) {
  void *result(0);

  if (handle_) {
    result = dlsym(handle_, symbol.c_str());
  }

  return result;
}

const std::string&
SharedLibraryImpl::pathImpl() const {
  return path_;
}

} /* namespace dadi */
