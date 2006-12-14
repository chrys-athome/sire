########################################
#
# CMake file for library:SireBase
#
########################################

# Third Party dependencies of this module

# Qt4
include(${QT_USE_FILE})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireBase
set ( SIREBASE_SOURCES
      idpair.cpp
      idtriple.cpp
      majminversion.cpp
      md5sum.cpp
    )

add_library (SireBase ${SIREBASE_SOURCES})

set_target_properties (SireBase PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

# Link to other Sire libraries, and GSL
target_link_libraries (SireBase 
                       ThirdParty
                       SireError
                       )

# installation
install( TARGETS SireBase
         RUNTIME DESTINATION ${SIRE_BIN}
	 LIBRARY DESTINATION ${SIRE_LIBS}
	 ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

