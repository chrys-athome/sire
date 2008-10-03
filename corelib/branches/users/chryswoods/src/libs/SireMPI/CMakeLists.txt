########################################
#
# CMake file for library:SireMPI
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

find_package( MPI REQUIRED )

if (MPI_FOUND)
  message( STATUS " ** Adding proper MPI support ** " )
  message( STATUS "MPI headers: ${MPI_INCLUDE_PATH}" )
  message( STATUS "MPI libraries: ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY}" )

  include_directories( ${MPI_INCLUDE_PATH} )

  add_definitions( -D__SIRE_USE_MPI__ )

  set ( SIREMPI_EXTRA_LIBRARIES  ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY} )

  #Save the path to the MPI headers
  save_sire_extra_include(${MPI_INCLUDE_PATH})

else()
  message( STATUS "SireMPI will be compiled without MPI support." )   

endif()

# Define the headers in SireMPI
set ( SIREMPI_HEADERS

      mpinode.h
      mpinodes.h
      mpipromise.h
      mpiworker.h
    
      detail/mpidetail.h
 
    )

# Define the sources in SireMPI
set ( SIREMPI_SOURCES

      register_sirempi.cpp

      mpinode.cpp
      mpinodes.cpp
      mpipromise.cpp
      mpiworker.cpp  
 
      ${SIREMPI_HEADERS}
    )

add_library (SireMPI ${SIREMPI_SOURCES})

set_target_properties (SireMPI PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

# Link to other Sire libraries
target_link_libraries (SireMPI
                       SireError
                       SireStream
                       ${SIREMPI_EXTRA_LIBRARIES}
                       )

# installation
install( TARGETS SireMPI EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMPI_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMPI )
