########################################
#
# CMake file for library:SireUnitTest
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTSQL_INCLUDE_DIR}
                    ${BOOST_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireUnitTest
set ( SIREUNITTEST_SOURCES
      testatomselection.cpp
      testbase.cpp
      testcomplex.cpp
      testcoordgroup.cpp
      testcutgroup.cpp
      testffdb.cpp
      testforcefields.cpp
      testmatch.cpp
      testperiodicbox.cpp
      testpropertyextractor.cpp
      testsampler.cpp
      testsharedpolypointer.cpp
      testsirecas.cpp
      testunits.cpp
      testvector.cpp
      testversion.cpp

      runall.cpp
    )

# Add the resource files
set ( SIREUNITTEST_RCCS
      resources.qrc
    )

# generate rules for building source files from the resources
qt4_add_resources(SIREUNITTEST_RCC_SRCS ${SIREUNITTEST_RCCS})

# generate the library
add_library (SireUnitTest
                    ${SIREUNITTEST_SOURCES}
                    ${SIREUNITTEST_RCC_SRCS}
            )

set_target_properties (SireUnitTest PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireUnitTest
                       SireIO
                       SireMove
                       SireMM
                       SireCAS
                       SireBase
                       SireUnits
                       ${BOOST_UNITTEST_LIBRARY}
                       )

# installation
install( TARGETS SireUnitTest
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
