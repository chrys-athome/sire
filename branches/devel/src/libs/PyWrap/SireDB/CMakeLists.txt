########################################
#
# CMake file for Python wrapper:_DB
#
########################################

# Third Party dependencies of this module
include_directories( ${BOOST_INCLUDE_DIR}
                     ${PYTHON_INCLUDE_DIR} )

# Qt4
include(${QT_USE_FILE})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs
                    ${CMAKE_SOURCE_DIR}/src/libs/PyWrap)

# Define the sources in SireDB
set ( WRAPDB_SOURCES
      wrap_assign_atoms.cpp      
      wrap_assign_parameters.cpp 
      wrap_atomtable.cpp         
      wrap_atomtype.cpp          
      wrap_atomtypedb.cpp        
      wrap_atomtypetable.cpp  
      wrap_db.cpp		
      wrap_matchatom.cpp	
      wrap_matchatomtype.cpp  
      wrap_match.cpp
      wrap_matchhelpers2.cpp  
      wrap_matchhelpers.cpp   
      wrap_matchmol.cpp	 
      wrap_matchmra.cpp	 
      wrap_matchmr.cpp	 
      wrap_matchres.cpp	 
      wrap_parameterdb.cpp
      wrap_parametertable.cpp
      wrap_relatedb.cpp
      wrap_using_database.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireDB ${WRAPDB_SOURCES})

set_target_properties (PyWrap_SireDB PROPERTIES
                       VERSION ${SIRE_VERSION}
		       SOVERSION ${SIRE_VERSION_MAJOR}
		       )
		       
target_link_libraries (PyWrap_SireDB
                       SireDB
                       PyWrap_SireCAS
		       ${BOOST_PYTHON_LIBRARY}
		      )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireDB
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireDB wrap_SireDB.cpp)

target_link_libraries (PythonWrapping_SireDB
                       PyWrap_SireDB
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireDB
                       PROPERTIES
                         OUTPUT_NAME "DB"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/DB )

install( TARGETS PythonWrapping_SireDB
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
