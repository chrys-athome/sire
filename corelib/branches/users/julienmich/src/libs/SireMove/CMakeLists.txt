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
      dlmrigidbody.h
      dynamics.h
      ensemble.h
      errors.h
      getpoint.h
      flexibility.h
      gibbsmove.h
      hybridmc.h
      integrator.h
      integratorworkspace.h
      #integratorworkspacejm.h
      internalmove.h
      internalmovesingle.h
      moldeleter.h
      molinserter.h
      moleculardynamics.h
      montecarlo.h
      move.h
      moves.h
      mtsmc.h
      #openmmintegrator.h	
      prefsampler.h
      rbworkspace.h 
      #rbworkspacejm.h
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
      volumechanger.h
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

      dlmrigidbody.cpp
      dynamics.cpp
      ensemble.cpp
      errors.cpp
      getpoint.cpp
      flexibility.cpp
      gibbsmove.cpp
      hybridmc.cpp
      integrator.cpp
      integratorworkspace.cpp
      #integratorworkspacejm.cpp
      internalmove.cpp
      internalmovesingle.cpp
      moldeleter.cpp
      molinserter.cpp
      moleculardynamics.cpp
      montecarlo.cpp
      move.cpp
      moves.cpp
      mtsmc.cpp
      #openmmintegrator.cpp
      prefsampler.cpp
      rbworkspace.cpp
      #rbworkspacejm.cpp
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
      volumechanger.cpp
      volumemove.cpp
      zmatmove.cpp
      zmatrix.cpp   

      deprecated/replicas.cpp
      deprecated/repexreplicas.cpp

      ${SIREMOVE_HEADERS}
      ${SIREMOVE_DEPRECATED_HEADERS}
    )

add_library (SireMove ${SIREMOVE_SOURCES})

#set (Path "/home/julien/local/openmm-4.0/")

#find_library(OPENMM_LIBRARY 
#			NAMES 
#			OpenMM 
#			PATHS
#			${Path}/lib)
#
#message ("Dir = ${OPENMM_LIBRARY}")


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
                       #${OPENMM_LIBRARY}
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
