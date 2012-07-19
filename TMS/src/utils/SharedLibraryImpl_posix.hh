/**
 * @file   SharedLibraryImpl_posix.hh
 * @author hguemar <hguemar@sysfera.com>
 * @brief  Posix implementation of class SharedLibrary
 * based on dlopen/dlclose/dlsym calls
 * @section License
 *   |LICENSE|
 *
 */

#ifndef _SHAREDLIBRARYIMPL_POSIX_HH_
#define _SHAREDLIBRARYIMPL_POSIX_HH_

#include <string>

namespace dadi {

/**
 * @class SharedLibraryImpl
 * @brief shared library posix implementation class
 *
 */
class SharedLibraryImpl {
public:
  /**
   * @brief constructor
   */
  SharedLibraryImpl();
  /**
   * @brief constructor
   * @param path path to shared library
   */
  explicit SharedLibraryImpl(const std::string& path);
  /**
   * @brief destructor
   */
  ~SharedLibraryImpl();

  /**
   * @brief load shared library
   * @param path path to shared library
   */
  void
  loadImpl(const std::string& path);
  /**
   * @brief unload shared library
   */
  void
  unloadImpl();

  /**
   * @brief tells if a shared library has been loaded
   * @return boolean
   */
  bool
  isLoadedImpl() const;
  /**
   * @brief test symbol existence
   * @param symbol symbol to test
   * @return boolean
   */
  bool
  hasSymbolImpl(const std::string& symbol);
  /**
   * @brief get symbol from shared library
   * @param symbol symbol to obtain
   */
  void *
  symbolImpl(const std::string& symbol);

  /**
   * @brief get path to loaded shared library
   * @return path
   */
  const std::string&
  pathImpl() const;

private:
  std::string path_; /**< shared library path */
  void *handle_; /**< shared library native handle */
};

} /* namespace dadi */

#endif  /* _SHAREDLIBRARYIMPL_POSIX_HH_ */

