SET( CMAKE_INSTALL_PREFIX ../../.. )

#set(ORAC_INC "/home/keo/Bureau/instantclient_11_2/sdk/include")
#set(ORAC_LIB "/home/keo/Bureau/instantclient_11_2")

INCLUDE_DIRECTORIES(
 /usr/include/postgresql 
 ../exception/
 ${ORAC_INC}
)

LINK_DIRECTORIES(
  ../../../lib
  ${ORAC_LIB}
)

ADD_EXECUTABLE(mainDB
  main.cpp
  )

ADD_LIBRARY(database-vishnu
  DbFactory.cpp
  Database.cpp
  DatabaseResult.cpp
  POSTGREDatabase.cpp
#  OracleDatabase.cpp
  )

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


TARGET_LINK_LIBRARIES(mainDB -lpq database-vishnu exception-vishnu 
#  -lociicus -locci
)

install( TARGETS database-vishnu DESTINATION lib )
