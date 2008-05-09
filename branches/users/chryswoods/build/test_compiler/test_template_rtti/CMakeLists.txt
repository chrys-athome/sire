########################################
#
# CMake file for test_template_rtti
#
########################################

#create the library
add_library (libFoo libfoo.h libfoo.cpp)

set_target_properties (libFoo PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

#now create the test application
add_executable( test_template_rtti main.cpp )

target_link_libraries( test_template_rtti libFoo )

