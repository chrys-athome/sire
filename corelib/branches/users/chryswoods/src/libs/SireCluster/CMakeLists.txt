########################################
#
# CMake file for library:SireCluster
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

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
                       SireError
                       SireStream
                       )

# installation
install( TARGETS SireCluster EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIRECLUSTER_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireCluster )
