########################################
#
# CMake file for sire_python
# (mpi-aware python executable)
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
set ( SIREPYTHON_SOURCES

      main.cpp

    )

# Create the library that holds all of the class wrappers
add_executable (sire_python ${SIREPYTHON_SOURCES})

set_target_properties (sire_python
                       PROPERTIES
                       VERSION ${SIRE_VERSION}
                      )
		       
target_link_libraries (sire_python
                       SIRE_SireMPI
                       ${PYTHON_LIBRARIES}
                      )

# installation
install( TARGETS sire_python
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
