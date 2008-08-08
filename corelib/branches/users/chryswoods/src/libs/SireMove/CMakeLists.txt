########################################
#
# CMake file for library:SireMove
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

if (SIRE_USE_MPI)
  include ( FindMPI )
  find_package(MPI REQUIRED)

  include_directories( ${MPI_INCLUDE_PATH} )

  add_definitions( -D__SIRE_USE_MPI__ )

  set ( SIREMOVE_EXTRA_LIBRARIES  ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY} )
endif()

# Define the headers in SireMove
set ( SIREMOVE_HEADERS
      montecarlo.h
      move.h
      moves.h
      mpinode.h
      mpinodes.h
      mpisim.h
      mtsmc.h
      prefsampler.h
      rigidbodymc.h
      sampler.h
      simcontroller.h
      simulation.h
      threadsim.h
      uniformsampler.h
      volumemove.h
      weightedmoves.h
    )

# Define the sources in SireMove
set ( SIREMOVE_SOURCES

      montecarlo.cpp
      move.cpp
      moves.cpp
      mpinode.cpp
      mpinodes.cpp
      mpisim.cpp
      mtsmc.cpp
      prefsampler.cpp
      rigidbodymc.cpp
      sampler.cpp
      simcontroller.cpp
      simulation.cpp
      threadsim.cpp
      uniformsampler.cpp
      weightedmoves.cpp
      volumemove.cpp
   
      detail/mpidetail.h
 
      ${SIREMOVE_HEADERS}
    )

add_library (SireMove ${SIREMOVE_SOURCES})

set_target_properties (SireMove PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

# Link to other Sire libraries
target_link_libraries (SireMove
                       SireSystem
                       SireMaths

                       ${SIREMOVE_EXTRA_LIBRARIES}
                       )

# installation
install( TARGETS SireMove
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMOVE_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMove )

