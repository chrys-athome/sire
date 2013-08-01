########################################
#
# CMake file for library:Soiree
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in Soiree
set ( SOIREE_HEADERS
      bennets.h
      fep.h
      ti.h
    )

# Define the sources in SireSystem
set ( SOIREE_SOURCES

      register_soiree.cpp

      bennets.cpp
      fep.cpp
      ti.cpp

      third_party/regress.cpp

      ${SOIREE_HEADERS}
    )

add_library (Soiree ${SOIREE_SOURCES})

set_target_properties (Soiree PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (Soiree
                       SireCAS
                       SireMaths
                       SireBase
                       SireStream
                       )

# installation
install( TARGETS Soiree EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SOIREE_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/Soiree )

