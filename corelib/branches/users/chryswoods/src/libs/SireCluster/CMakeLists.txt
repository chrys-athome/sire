########################################
#
# CMake file for library:SireCluster
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

if ( SIRE_USE_MPI )
  message( STATUS " ** Adding MPI support to SireCluster ** " )
  message( STATUS "MPI headers: ${MPI_INCLUDE_PATH}" )
  message( STATUS "MPI libraries: ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY}" )
  message( STATUS "MPI definitions: ${MPI_DEFINITIONS}" )

  include_directories( BEFORE SYSTEM ${MPI_INCLUDE_PATH} )

  add_definitions( -DSIRE_USE_MPI ${MPI_DEFINITIONS} )

  set ( SIRECLUSTER_EXTRA_LIBRARIES ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY} )

  save_sire_variable( "SIRE_MPI_INCLUDE_PATH" "${MPI_INCLUDE_PATH}" )
  save_sire_variable( "SIRE_MPI_DEFINITIONS" "-DSIRE_USE_MPI ${MPI_DEFINITIONS}" )

  set ( SIRECLUSTER_MPI_HEADERS
        mpi/mpicluster.h
      )

  set ( SIRECLUSTER_MPI_SOURCES
        mpi/mpicluster.cpp
      )

else()
  message( STATUS "SireCluster will be compiled without MPI support." )   
  save_sire_variable( "SIRE_MPI_DEFINITIONS" "" )
endif()

save_sire_variable( "SIRE_USE_MPI" "${MPI_FOUND}" )

# Define the headers in SireCluster
set ( SIRECLUSTER_HEADERS

      cluster.h
      errors.h
      node.h
      nodes.h
      promise.h      
      promises.h
      workpacket.h

      network/communicator.h
      network/envelope.h
      network/hostinfo.h
      network/message.h
      network/netbackend.h
      network/netfrontend.h
      network/netresourcemanager.h
      network/nodemessages.h
      network/receivequeue.h
      network/sendqueue.h

      resources/backend.h
      resources/frontend.h
      resources/resourcemanager.h
      resources/workqueue.h
    )

# Define the sources in SireCluster
set ( SIRECLUSTER_SOURCES

      cluster.cpp
      errors.cpp
      node.cpp
      nodes.cpp
      promise.cpp
      promises.cpp
      workpacket.cpp

      network/communicator.cpp
      network/envelope.cpp
      network/hostinfo.cpp
      network/message.cpp
      network/netbackend.cpp
      network/netfrontend.cpp
      network/netresourcemanager.cpp
      network/nodemessages.cpp
      network/receivequeue.cpp
      network/sendqueue.cpp

      resources/backend.cpp
      resources/frontend.cpp
      resources/resourcemanager.cpp
      resources/workqueue.cpp

      ${SIRECLUSTER_MPI_SOURCES}
      ${SIRECLUSTER_MPI_HEADERS}

      ${SIRECLUSTER_HEADERS}
    )

add_library (SireCluster ${SIRECLUSTER_SOURCES})

set_target_properties (SireCluster PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireCluster
                       SireSec
                       SireMaths
                       SireBase
                       Siren
                       ${SIRECLUSTER_EXTRA_LIBRARIES}
                       )

save_sire_variable( "SIRECLUSTER_EXTRA_LIBRARIES" "${SIRECLUSTER_EXTRA_LIBRARIES}" )

# installation
install( TARGETS SireCluster EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIRECLUSTER_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireCluster )

