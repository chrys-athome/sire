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
include_directories( BEFORE ${SIRE_INCLUDE_DIR} )

# Other python wrapping directories
include_directories(${CMAKE_SOURCE_DIR})

if (SIRE_USE_MPI)
  add_definitions( ${SIRE_MPI_DEFINITIONS} )
  include_directories( BEFORE SYSTEM ${SIRE_MPI_INCLUDE_PATH} )
endif()

# Define the sources in Helpers
set ( SIREPYTHON_SOURCES

      main.cpp
      pythonpacket.cpp
      pythonpacket.h

    )

# Create the library that holds all of the class wrappers
add_executable (sire_python ${SIREPYTHON_SOURCES})

set_target_properties (sire_python
                       PROPERTIES
                       VERSION ${SIRE_VERSION}
                      )
		       
target_link_libraries (sire_python
                       SIRE_SireCluster
                       SIRE_SireBase
                       SIRE_Siren
                       ${SIRECLUSTER_EXTRA_LIBRARIES}
                       ${PYTHON_LIBRARIES}
                      )

# installation
install( TARGETS sire_python
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
