########################################
#
# CMake file for application:sirentest
#
########################################

# Third Party dependencies of this module
include_directories(${BOOST_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

if ( SIRE_USE_MPI )
  message( STATUS " ** Adding MPI support to sire ** " )
  message( STATUS "MPI headers: ${MPI_INCLUDE_PATH}" )
  message( STATUS "MPI libraries: ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY}" )
  message( STATUS "MPI definitions: ${MPI_DEFINITIONS}" )

  include_directories( BEFORE SYSTEM ${MPI_INCLUDE_PATH} )

  add_definitions( -DSIRE_USE_MPI ${MPI_DEFINITIONS} )
endif()

set ( SIRENTEST_LIB_MOC_HEADERS

      client.h
      messagequeue.h
      server.h

    )

set ( SIRENTEST_LIB_HEADERS

      sharedmemory.h

      ${SIRENTEST_LIB_MOC_HEADERS}
    )

foreach ( mocfile ${SIRENTEST_LIB_MOC_HEADERS} )
    string(REGEX REPLACE "\\.h$" "_moc.cpp" mocsource ${mocfile} )
    QT4_GENERATE_MOC( ${mocfile} ${mocsource} )
    list(APPEND mocsources ${mocsource})
endforeach ( mocfile )

set ( SIRENTEST_LIB_SOURCES

      client.cpp
      messagequeue.cpp
      server.cpp
      sharedmemory.cpp

      ${SIRENTEST_LIB_HEADERS}

      ${mocsources}
    )

set ( SIRENTEST_SOURCES
      main.cpp
    )

set ( SIRENTEST_BACKEND_SOURCES
      backend_main.cpp
    )

# generate the application
add_library ( SirenTest ${SIRENTEST_LIB_SOURCES} )
add_executable ( sirentest_frontend ${SIRENTEST_SOURCES} )
add_executable ( sirentest_backend ${SIRENTEST_BACKEND_SOURCES} )

set_target_properties (SirenTest PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

set_target_properties (sirentest_frontend PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       OUTPUT_NAME "sirentest"
                      )

set_target_properties (sirentest_backend PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SirenTest
                       SireCluster
                       SireVol
                       SireCAS
                       SireBase
                       SireMaths
                       SireUnits
                       SireID
                       Siren
                      )

target_link_libraries (sirentest_frontend
                       SirenTest
                      )

target_link_libraries (sirentest_backend
                       SirenTest
                      )

# installation
install( TARGETS SirenTest sirentest_frontend sirentest_backend
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
