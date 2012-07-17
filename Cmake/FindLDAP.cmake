# Find OpenLDAP libraries

# This module defines:
# LDAP_FOUND             - whether libraries were found
# LDAP_INCLUDE_DIR       - include path
# LDAP_LIBRARIES         - libraries to link against

find_path(LDAP_INCLUDE_DIR ldap.h
  PATHS ${LDAP_INSTALL_DIR}/include)

find_library(LDAP_LIBRARY
  NAMES ldap
  PATHS ${LDAP_INSTALL_DIR}/lib)
find_library(LBER_LIBRARY
  NAMES lber
  PATHS ${LDAP_INSTALL_DIR}/lib)

set(LDAP_LIBRARIES LDAP_LIBRARY LBER_LIBRARY)

# defines our variable
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LDAP DEFAULT_MSG
  LDAP_LIBRARIES LDAP_LIBRARY LBER_LIBRARY LDAP_INCLUDE_DIR)


