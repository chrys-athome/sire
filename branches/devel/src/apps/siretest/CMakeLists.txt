########################################
#
# CMake file for application:siretest
#
########################################

# Third Party dependencies of this module
include_directories(${BOOST_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in siretest
set ( SIRETEST_SOURCES
      init_unit_test_suite.cpp
    )

# generate the application
add_executable ( siretest ${SIRETEST_SOURCES} )

set_target_properties (siretest PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (siretest
                       SireUnitTest
		       "-lboost_unit_test_framework"
                       )
