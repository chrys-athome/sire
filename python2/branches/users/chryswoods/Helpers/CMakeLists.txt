########################################
#
# CMake file for Python 2 Helper Library
#   (SirePython2)
#
########################################

# Third Party dependencies of this module
include_directories( ${BOOST_INCLUDE_DIR}
                     ${PYTHON_INCLUDE_DIR} )

# Sire include paths
include_directories( ${SIRE_INCLUDE_DIR} )

# Other python wrapping directories
include_directories(${CMAKE_SOURCE_DIR})

# Define the sources in Helpers
set ( WRAPHELPERS_SOURCES

      objectregistry.cpp

    )

# Create the library that holds all of the class wrappers
add_library (SirePython2 ${WRAPHELPERS_SOURCES})

set_target_properties (SirePython2  
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
		       )
		       
target_link_libraries (SirePython2
                       SIRE_SireStream
                       SIRE_SireError
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARIES}
                      )

# installation
install( TARGETS SirePython2
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
