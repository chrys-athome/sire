########################################
#
# CMake file for library:SireError
#
########################################

include_directories(${CMAKE_SOURCE_DIR}/src/libs)

add_library (SireError
             errors.cpp
             exception.cpp
             getbacktrace.cpp)

target_link_libraries (SireError
                       ${QT_QTCORE_LIBRARY}
                      )

# installation
install( TARGETS SireError
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

