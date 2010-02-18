########################################
#
# CMake file for library:SireSec
#
########################################

include_directories(${CMAKE_SOURCE_DIR}/src/libs/SireSec
                    ${CMAKE_SOURCE_DIR}/src/libs
                    ${QT_QTXML_INCLUDE_DIR})

set( UNZIP_COMMAND unzip -a -u )
set( CRYPTLIB_MAKE_COMMAND make )

set( CRYPTLIB_ZIP "${CMAKE_SOURCE_DIR}/src/libs/SireSec/third_party/cl333.zip" )
set( CRYPTLIB_DIR "${CMAKE_BINARY_DIR}/src/libs/SireSec/third_party/cryptlib" )

set( CRYPTLIB_HEADER "${CRYPTLIB_DIR}/cryptlib.h" )
set( CRYPTLIB_LIBRARY "${CRYPTLIB_DIR}/libcl.a" )

set ( SIRESEC_PUBLIC_HEADERS

      errors.h
      key.h
      lock.h
      password.h
      privatekey.h
      publickey.h
      pubprilock.h
      signaturelock.h
    )

set ( SIRESEC_SOURCES

      ${SIRESEC_PUBLIC_HEADERS}

      errors.cpp
      key.cpp
      lock.cpp
      password.cpp
      privatekey.cpp
      publickey.cpp
      pubprilock.cpp
      signaturelock.cpp

      crypt.h
      crypt.cpp
    )


add_library ( SireSec ${SIRESEC_SOURCES} )

add_custom_command(
     OUTPUT ${CRYPTLIB_DIR}
     COMMAND ${CMAKE_COMMAND} -E make_directory ${CRYPTLIB_DIR}
     COMMENT "Creating cryptlib build directory..."
     DEPENDS ${CRYPTLIB_ZIP}
    )

add_custom_command(
     OUTPUT ${CRYPTLIB_HEADER}
     COMMAND ${UNZIP_COMMAND} ${CRYPTLIB_ZIP}
     DEPENDS ${CRYPTLIB_DIR}
     WORKING_DIRECTORY ${CRYPTLIB_DIR}
     COMMENT "Unzipping cryptlib..."
   )

add_custom_command(
     OUTPUT ${CRYPTLIB_LIBRARY}
     COMMAND ${CRYPTLIB_MAKE_COMMAND}
     DEPENDS ${CRYPTLIB_HEADER}
     WORKING_DIRECTORY ${CRYPTLIB_DIR}
     COMMENT "Compiling cryptlib..."
    )

add_custom_target( CRYPTLIB DEPENDS ${CRYPTLIB_LIBRARY} )

add_dependencies( SireSec CRYPTLIB )

include_directories( ${CRYPTLIB_DIR} )

set_target_properties (SireSec PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

target_link_libraries ( SireSec
                        Siren
                        ${CRYPTLIB_LIBRARY}
                        ${QT_QTCORE_LIBRARY} )

# installation
install( TARGETS SireSec  EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIRESEC_PUBLIC_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireSec )
