#
# Try to find the TORQUE installation
#

find_library(LIBCRYPT_LIB
  crypt 
  paths
	${LIBCRYPT}/lib
        $ENV{LIBCRYPT}/lib
	/usr/lib
	/usr/local/lib
)

if (LIBCRYPT_LIB)
  set(LIBCRYPT_FOUND "Yes")
  mark_as_advanced(LIBCRYPT_DIR)
  mark_as_advanced(LIBCRYPT_LIB)
endif(LIBCRYPT_LIB)
