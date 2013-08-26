########################################
#
# CMake file for application:test_system
#
########################################

# these are all simple executables that will be 
# used to test for features in a machine before install

check_include_files( gnu/libc-version.h HAVE_LIBC_VERSION_H )

if (HAVE_LIBC_VERSION_H)
  add_executable( get_glibc_version get_glibc_version.c )
  install( TARGETS get_glibc_version RUNTIME DESTINATION ${SIRE_BIN}/support )
endif()

check_include_files( sys/utsname.h HAVE_UTSNAME_H )

if (HAVE_UTSNAME_H)
  add_executable( get_uname get_uname.c )
  install( TARGETS get_uname RUNTIME DESTINATION ${SIRE_BIN}/support )
endif()

if (SIRE_FOUND_CPUID)
  add_definitions( "-DSIRE_FOUND_CPUID" )
  add_definitions( "${CPUID_DEFINITIONS}" )
  include_directories( ${CPUID_INCLUDE_DIR} )

  add_executable( get_cpuid get_cpuid.c )

  set_target_properties(get_cpuid PROPERTIES LINK_SEARCH_START_STATIC 1)
  set_target_properties(get_cpuid PROPERTIES LINK_SEARCH_END_STATIC 1)

  target_link_libraries( get_cpuid ${CPUID_LIBRARY} )

  install( TARGETS get_cpuid RUNTIME DESTINATION ${SIRE_BIN}/support )

endif()

