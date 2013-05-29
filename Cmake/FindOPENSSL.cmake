#
# Try to find libssl
#

find_library(OPENSSL_LIB
  NAMES ssl
  paths
        ${OPENSSL}/lib
        $ENV{OPENSSL}/lib
        /usr/lib
        /usr/local/lib
)


find_library(LIBCRYPTO_LIB
  NAMES crypto
  paths
        ${LIBCRYPT}/lib
        $ENV{LIBCRYPT}/lib
        /usr/lib
        /usr/local/lib
)

find_path(OPENSSL_INCLUDE_DIR
  PATH openssl/ssl.h
  paths
      ${OPENSSL}/include
      $ENV{OPENSSL}/include
      /usr/include
      /usr/local/include
)

if (OPENSSL_LIB)
  set(OPENSSL_FOUND "Yes")
  mark_as_advanced(OPENSSL_INCLUDE_DIR)
 message ("Found ${OPENSSL_LIB}")
  mark_as_advanced(OPENSSL_LIB)
endif()
