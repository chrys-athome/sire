########################################
#
# CMake file for application:testmulti
#
########################################

# Third Party dependencies of this module
include_directories(${BOOST_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

set ( TESTMULTI_SOURCES
      main.cpp
    )

# generate the application
add_executable ( sire_testmulti ${TESTMULTI_SOURCES} )

set_target_properties (sire_testmulti PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (sire_testmulti
                       SireMaths
                      )

# installation
install( TARGETS sire_testmulti
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
