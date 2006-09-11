########################################
#
# CMake file for library:SireStream
#
########################################

include_directories(${CMAKE_SOURCE_DIR}/src/libs)

add_library (SireStream
             getmagic.cpp
             magic_error.cpp
             version_error.cpp)

target_link_libraries (SireStream
                       ThirdParty
                       ${QT_QTCORE_LIBRARY}
                      )

# installation
install( TARGETS SireStream
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
