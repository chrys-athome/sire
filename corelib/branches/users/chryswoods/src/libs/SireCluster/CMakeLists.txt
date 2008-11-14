########################################
#
# CMake file for library:SireCluster
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

find_package( MPI )

if (MPI_FOUND)
  message( STATUS " ** Adding MPI support to SireCluster ** " )
  message( STATUS "MPI headers: ${MPI_INCLUDE_PATH}" )
  message( STATUS "MPI libraries: ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY}" )

  include_directories( BEFORE SYSTEM ${MPI_INCLUDE_PATH} )

  add_definitions( -D__SIRE_USE_MPI__ )

  set ( SIRECLUSTER_EXTRA_LIBRARIES ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY} )

  set ( SIRECLUSTER_MPI_HEADERS

        mpi/messages.h
        mpi/mpicluster.h
        mpi/mpifrontend.h
        mpi/receivequeue.h
        mpi/reply.h
        mpi/sendqueue.h

      )

  set ( SIRECLUSTER_MPI_SOURCES

        mpi/messages.cpp
        mpi/mpicluster.cpp
        mpi/mpifrontend.cpp
        mpi/receivequeue.cpp
        mpi/reply.cpp
        mpi/sendqueue.cpp

      )

else()
  message( STATUS "SireCluster will be compiled without MPI support." )   

endif()

# Define the headers in SireCluster
set ( SIRECLUSTER_HEADERS

      backend.h
      backends.h
      cluster.h
      frontend.h
      frontends.h
      localbackend.h
      localfrontend.h
      node.h
      promise.h      
      workpacket.h
    )

# Define the sources in SireCluster
set ( SIRECLUSTER_SOURCES

      register_sirecluster.cpp

      backend.cpp
      backends.cpp
      cluster.cpp
      frontend.cpp
      frontends.cpp
      localbackend.cpp
      localfrontend.cpp
      node.cpp
      promise.cpp
      workpacket.cpp
 
      ${SIRECLUSTER_MPI_SOURCES}
      ${SIRECLUSTER_MPI_HEADERS}

      ${SIRECLUSTER_HEADERS}
    )

add_library (SireCluster ${SIRECLUSTER_SOURCES})

set_target_properties (SireCluster PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

# Link to other Sire libraries
target_link_libraries (SireCluster
                       SireBase
                       SireError
                       SireStream
                       ${SIRECLUSTER_EXTRA_LIBRARIES}
                       )

# installation
install( TARGETS SireCluster EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIRECLUSTER_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireCluster )

