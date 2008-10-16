########################################
#
# CMake file for library:SireMove
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireMove
set ( SIREMOVE_HEADERS
      ensemble.h
      montecarlo.h
      move.h
      moves.h
      mpisim.h
      mpisimworker.h
      mtsmc.h
      prefsampler.h
      repexmove.h
      repexreplicas.h
      replicarunner.h
      replicas.h
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

      register_siremove.cpp

      ensemble.cpp
      montecarlo.cpp
      move.cpp
      moves.cpp
      mpisim.cpp
      mpisimworker.cpp
      mtsmc.cpp
      prefsampler.cpp
      repexmove.cpp
      repexreplicas.cpp
      replicarunner.cpp
      replicas.cpp
      rigidbodymc.cpp
      sampler.cpp
      simcontroller.cpp
      simulation.cpp
      threadsim.cpp
      uniformsampler.cpp
      weightedmoves.cpp
      volumemove.cpp
   
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
                       SireMPI
                       SireStream
                       )

# installation
install( TARGETS SireMove EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMOVE_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMove )

