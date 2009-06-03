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
      dynamics.h
      ensemble.h
      errors.h
      integrator.h
      integratorworkspace.h
      moleculardynamics.h
      montecarlo.h
      move.h
      moves.h
      mtsmc.h
      prefsampler.h
      repexmove.h
      replica.h
      replicas.h
      rigidbodymc.h
      sampler.h
      simpacket.h
      simstore.h
      simulation.h
      supramove.h
      supramoves.h
      suprasim.h
      suprasimpacket.h
      suprasubmove.h
      suprasubmoves.h
      suprasubsim.h
      suprasubsimpacket.h
      suprasubsystem.h
      suprasystem.h
      uniformsampler.h
      velocitygenerator.h
      velocityverlet.h
      volumemove.h
      weightedmoves.h
      zmatmove.h
      zmatrix.h
    )

set ( SIREMOVE_DEPRECATED_HEADERS

      deprecated/replicas.h
      deprecated/repexreplicas.h
    )

# Define the sources in SireMove
set ( SIREMOVE_SOURCES

      register_siremove.cpp

      dynamics.cpp
      ensemble.cpp
      errors.cpp
      integrator.cpp
      integratorworkspace.cpp
      moleculardynamics.cpp
      montecarlo.cpp
      move.cpp
      moves.cpp
      mtsmc.cpp
      prefsampler.cpp
      repexmove.cpp
      replica.cpp
      replicas.cpp
      rigidbodymc.cpp
      sampler.cpp
      simpacket.cpp
      simstore.cpp
      simulation.cpp
      supramove.cpp
      supramoves.cpp
      suprasim.cpp
      suprasimpacket.cpp
      suprasubmove.cpp
      suprasubmoves.cpp
      suprasubsim.cpp
      suprasubsimpacket.cpp
      suprasubsystem.cpp
      suprasystem.cpp
      uniformsampler.cpp
      weightedmoves.cpp
      velocitygenerator.cpp
      velocityverlet.cpp
      volumemove.cpp
      zmatmove.cpp
      zmatrix.cpp   

      deprecated/replicas.cpp
      deprecated/repexreplicas.cpp

      ${SIREMOVE_HEADERS}
      ${SIREMOVE_DEPRECATED_HEADERS}
    )

add_library (SireMove ${SIREMOVE_SOURCES})

set_target_properties (SireMove PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireMove
                       SireSystem
                       SireMaths
                       SireCluster
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

install( FILES ${SIREMOVE_DEPRECATED_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMove/deprecated )
