########################################
#
# CMake file for library:ThirdParty
#
########################################

# Define the sources in ThirdParty
set ( THIRDPARTY_SOURCES
      md5.c
    )

add_library (ThirdParty SHARED ${THIRDPARTY_SOURCES})

set_target_properties (ThirdParty PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

