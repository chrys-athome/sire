########################################
#
# CMake file for library:ThirdParty
#
########################################

# Define the sources in ThirdParty
set ( THIRDPARTY_SOURCES
      md5.c
    )

add_library (ThirdParty ${THIRDPARTY_SOURCES})

set_target_properties (ThirdParty PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# installation
install( TARGETS ThirdParty
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
