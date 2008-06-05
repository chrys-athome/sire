########################################
#
# CMake file for application:testlink
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in siretest
set ( TESTLINK_SOURCES
      main.cpp
    )

# generate the application
add_executable ( testlink ${TESTLINK_SOURCES} )

set_target_properties (testlink PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (testlink
                       SireCAS
                       SireUnits
                       SireMol
                       SireError
                       SireVol
                       SireDB
                       SireFF
                       SireIO
                       SireMM
                       ThirdParty
                       SirePy
                       SireTest
                       SireMaths
                       #Spier
                       SireStream
                       SqliteDriver
                       SireUnitTest
                       ThirdPartyGUI
                       SireBase
                       )
