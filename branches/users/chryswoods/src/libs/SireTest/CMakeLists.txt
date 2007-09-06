########################################
#
# CMake file for library:SireTest
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireSystem
set ( SIRETEST_SOURCES
      runtests.cpp
      runtests.h
    )

add_library (SireTest ${SIRETEST_SOURCES})

set_target_properties (SireTest PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireTest
                       SireCAS
                       )

# installation
install( TARGETS SireTest
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
