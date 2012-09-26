########################################
#
# CMake file for library:SireSim
#
########################################

# Third Party dependencies of this module

if ( QT_QTXML_FOUND )
    include_directories( ${QT_QTXML_INCLUDE_DIR} )
endif ()

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the header files for SireSim
set ( SIRESIM_HEADERS

      proteinligandmc.h

      simparams.h

      dimensions.h
      dimensions.hpp

      indexvalue.h

      option.h

      tempvalue.h

      value.h
      values.h
    )

# Define the sources in SireSim
set ( SIRESIM_SOURCES

      register_siresim.cpp

      proteinligandmc.cpp

      simparams.cpp

      dimensions.cpp

      indexvalue.cpp

      option.cpp

      tempvalue.cpp

      value.cpp
      values.cpp

      ${SIRESIM_HEADERS}
    )

add_library (SireSim ${SIRESIM_SOURCES})

set_target_properties (SireSim PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireSim
                       SireSystem
                       SireMove
                       SireIO
                       ${QT_QTXML_LIBRARY}
                       )

# installation
install( TARGETS SireSim EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIRESIM_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireSim )
