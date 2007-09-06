########################################
#
# CMake file for library:SireError
#
########################################

include_directories(${CMAKE_SOURCE_DIR}/src/libs)

set ( SIREERROR_HEADERS
      errors.h
      exception.h
      getbacktrace.h
      version_error.h
    )

set ( SIREERROR_SOURCES
      errors.cpp
      exception.cpp
      getbacktrace.cpp
    
      ${SIREERROR_HEADERS}
    )

add_library ( SireError ${SIREERROR_SOURCES} )

target_link_libraries (SireError
                       ${QT_QTCORE_LIBRARY}
                      )

# installation
install( TARGETS SireError
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREERROR_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireError )
