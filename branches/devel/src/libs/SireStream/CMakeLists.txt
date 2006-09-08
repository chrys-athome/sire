########################################
#
# CMake file for library:SireStream
#
########################################

include_directories(${CMAKE_SOURCE_DIR}/src/libs)

add_library (SireStream SHARED
             getmagic.cpp
             magic_error.cpp
             version_error.cpp)

target_link_libraries (SireStream
                       ThirdParty
                       ${QT_QTCORE_LIBRARY}
                      )
