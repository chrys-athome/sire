########################################
#
# CMake file for library:SireError
#
########################################

include_directories(${CMAKE_SOURCE_DIR}/src/libs)

add_library (SireError SHARED
             errors.cpp
             exception.cpp
             getbacktrace.cpp)

target_link_libraries (SireError
                       SireStream
                       ${QT_QTCORE_LIBRARY}
                      )

