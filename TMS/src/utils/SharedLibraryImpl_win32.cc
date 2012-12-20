/**
 * @file   SharedLibraryImpl_win32.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  SharedLibrary win32 implementation
 * @section License
 *   |LICENSE|
 *
 */

#include "SharedLibraryImpl_win32.hh"
#include <windows.h>

namespace dadi {

SharedLibraryImpl::SharedLibraryImpl() : handle_(0) {}

SharedLibraryImpl::SharedLibraryImpl(const std::string& path) : handle_(0) {
  loadImpl(path);
}

SharedLibraryImpl::~SharedLibraryImpl() {
  unloadImpl();
}

void
SharedLibraryImpl::loadImpl(const std::string& path) {
  DWORD flags(0);

  handle_ = LoadLibraryEx(path.c_str(), 0, flags);

  path_ = path;
}

void
SharedLibraryImpl::unloadImpl() {
  if (handle_) {
    FreeLibrary((HMODULE) handle_);
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
    result = (void *)(GetProcAddress((HMODULE)handle_, symbol.c_str()));
  }

  return result;
}

const std::string&
SharedLibraryImpl::pathImpl() const {
  return path_;
}

} /* namespace dadi */

