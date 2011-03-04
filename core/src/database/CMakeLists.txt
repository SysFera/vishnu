# DATABASE SOURCES & INCLUDES

SET (DB_SRC
  DbFactory.cpp
  Database.cpp
  DatabaseResult.cpp
)

IF (POSTGRESQL_FOUND)
  SET (DB_INCLUDE_DIRECTORIES
    ${DB_INCLUDE_DIRECTORIES}
    ${POSTGRESQL_INCLUDE_DIR}
  )
  SET (DB_LIBS
    ${DB_LIBS}
    ${POSTGRESQL_LIB}
  )
  SET (DB_SRC
    ${DB_SRC}
    POSTGREDatabase.cpp
  )
ENDIF (POSTGRESQL_FOUND)

IF (ORACLE_FOUND)
  SET (DB_INCLUDE_DIRECTORIES
    ${DB_INCLUDE_DIRECTORIES}
    ${ORACLE_INCLUDE_DIR}
  )
  SET (DB_LIBS
    ${DB_LIBS}
    ${ORACLE_OCCI_LIB}
    ${ORACLE_CLNTSH_LIB}
  )
  SET (DB_SRC
    ${DB_SRC}
    OracleDatabase.cpp
  )
ENDIF (ORACLE_FOUND)

INCLUDE_DIRECTORIES(
  ${Boost_INCLUDE_DIRS}
  ${DB_INCLUDE_DIRECTORIES}
 ../exception/
 ../utils
)

LINK_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/../exception
)

# DATABASE LIBRARY

ADD_LIBRARY(database-vishnu STATIC
  ${DB_SRC}
)

TARGET_LINK_LIBRARIES(database-vishnu  ${DB_LIBS})

# Compiler avec oracle :
# - Recuperer dans le depot svn les libs et les includes du dossier :
# /home/keo/SVN/SYSFERA/startup/projets/1026-EDF/tech
# Dezipper les dossiers
# Regler le include directories a <oracle>/sdk/include
# Regler le link sur <oracle>
# Faire un lien symbolique pour la lib OCCI :  ln -nfs libocci.so.11.1 libocci.so
# Verifier que la lib aio1 est presente (l'installer le cas echeant)
# sudo apt-get install libaio1
# Compiler
# Le test effectif n'a pas ete realisé car pas de base oracle ici

