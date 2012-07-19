/**
 * @file   SharedLibrary.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  Abstract platform specific shared libraries loading
 * @section License
 *   |LICENSE|
 *
 */

#ifndef _SHAREDLIBRARY_HH_
#define _SHAREDLIBRARY_HH_

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace dadi {

class SharedLibraryImpl;

/**
 * @class SharedLibrary
 * @brief hides platform specific shared libraries loading
 */
class SharedLibrary : public boost::noncopyable {
public:
  /**
   * @brief default constructor
   */
  SharedLibrary();
  /**
   * @brief load shared library
   * @param path path to shared library
   */
  explicit SharedLibrary(const std::string& path);
  /**
   * @brief destructor
   */
  ~SharedLibrary();

  /**
   * @brief load shared library
   * @param path path to shared library
   */
  void
  load(const std::string& path);
  /**
   * @brief unload loaded shared library
   */
  void
  unload();

  /**
   * @brief test if any library has been loaded
   * @return boolean
   */
  bool
  isLoaded() const;
  /**
   * @brief test presence of a symbol
   * @param symbol
   * @return boolean
   */
  bool
  hasSymbol(const std::string& symbol);
  /**
   * @brief get symbol from shared library
   * @param symbol
   * @return symbol or NULL
   */
  void *
  symbol(const std::string& symbol);

  /**
   * @brief get shared library path
   * @return library native path (if any library loaded)
   */
  const std::string&
  path() const;

  /**
   * @brief return platform default library prefix
   * @return default library suffix ("lib" under Unix, "" under Windows)
   */
  static std::string
  prefix();

  /**
   * @brief return platform default library suffix
   * @return default library suffix (".so" under Unix, ".dylib" under mac,
   * ".dll" under Windows)
   */
  static std::string
  suffix();

private:
  /**
   *Platform-specific private implementation
   */
  boost::scoped_ptr<SharedLibraryImpl> pimpl;
};

/**
 * @typedef SharedLibraryPtr
 * @brief shared pointer on a SharedLibrary
 */
typedef boost::shared_ptr<SharedLibrary> SharedLibraryPtr;

} /* namespace dadi */

#endif  /* _SHAREDLIBRARY_HH_ */

