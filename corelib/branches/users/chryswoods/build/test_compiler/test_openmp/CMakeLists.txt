########################################
#
# CMake file for test_stream
#
########################################

include_directories(${CMAKE_SOURCE_DIR}/src/libs)

#create the library
add_executable ( test_openmp main.cpp)

set_target_properties (test_openmp PROPERTIES
                       LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

target_link_libraries (test_openmp
                       ${QT_QTCORE_LIBRARY}
                      )

