########################################
#
# CMake file for library:SireCluster
#
########################################

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireCluster
set ( SIRECLUSTER_HEADERS
      errors.h
      processor.h
      threadworker.h
    )

# Define the sources in SireCluster
set ( SIRECLUSTER_SOURCES
      errors.cpp      
      processor.cpp   
      threadworker.cpp
      
      ${SIRECLUSTER_HEADERS}
    )

add_library (SireCluster ${SIRECLUSTER_SOURCES})

set_target_properties (SireCluster PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other libraries
target_link_libraries (SireCluster
                       SireStream
                      )

# installation
install( TARGETS SireCluster
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIRECLUSTER_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireCluster )
