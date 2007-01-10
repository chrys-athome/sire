########################################
#
# CMake file for library:SireMove
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireMove
set ( SIREMOVE_SOURCES
      move.cpp

      montecarlo.cpp
      rigidbodymc.cpp

      #zmatrixmc.cpp
      #internalmc.cpp

      #nestedmc.cpp
      #hmc.cpp
      #mtsmc.cpp

      #moleculardynamics.cpp
      #rigidbodymd.cpp
      #langevinmd.cpp
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
                       )

# installation
install( TARGETS SireMove
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

