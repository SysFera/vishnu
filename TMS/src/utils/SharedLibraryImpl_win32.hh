/**
 * @file   SharedLibraryImpl_win32.hh
 * @author hguemar <hguemar@sysfera.com>
 * @brief  Win32 implementation of class SharedLibrary
 * based on LoadLibrary calls
 * @section License
 *   |LICENSE|
 *
 */

#ifndef _SHAREDLIBRARYIMPL_WIN32_HH_
#define _SHAREDLIBRARYIMPL_WIN32_HH_

#include <string>

namespace dadi {

// TODO: untested
/**
 * @class SharedLibraryImpl
 * @brief shared library win32 implementation class
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
   * @brief test if a shared library has been loaded
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

#endif  /* _SHAREDLIBRARYIMPL_WIN32_HH_ */
