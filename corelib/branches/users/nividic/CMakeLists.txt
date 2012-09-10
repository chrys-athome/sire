################################
#
#  CMake Project file for Sire
#
#   (C) Christopher Woods
#
################################

# require cmake >= 2.6.4
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

# however, ensure we are compatible with cmake 2.8
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 2.6)
   set ( USING_CMAKE_2_6 OFF )
   cmake_policy(VERSION 2.8)
else()
   set ( USING_CMAKE_2_6 ON )
endif()

# use loose loop syntax in CMakeLists files
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

# Absolutely can't run cmake in the source directory!
if ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
  message( FATAL_ERROR "You must run CMake in a different directory to the source!" ) 
endif ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )

# name the project
project (SIRE CXX C Fortran)

# Create a file in which we can save the values of all useful variables.
# This will mean that projects using Sire won't have to set these variables
set (SIRE_VARIABLES_FILE "${CMAKE_BINARY_DIR}/SireCompileVariables.cmake")
file ( REMOVE ${SIRE_VARIABLES_FILE} )

# This macro is used to save the names of variable to SireCompileVariables.cmake
macro( SAVE_SIRE_VARIABLE _var _value )
  file(APPEND ${SIRE_VARIABLES_FILE}
        "set( ${_var} \"${_value}\" )\n")
endmacro (SAVE_SIRE_VARIABLE)

# project version
set (SIRE_VERSION_MAJOR "0")
set (SIRE_VERSION_MINOR "0")
set (SIRE_VERSION_PATCH "1")

set (SIRE_VERSION "${SIRE_VERSION_MAJOR}.${SIRE_VERSION_MINOR}.${SIRE_VERSION_PATCH}")

math (EXPR SIRE_VERSION_NUMBER 
      "${SIRE_VERSION_MAJOR}*100000 + ${SIRE_VERSION_MINOR}*100 + ${SIRE_VERSION_PATCH}")

set (SIRE_VERSION_STRING "${SIRE_VERSION_MAJOR}_${SIRE_VERSION_MINOR}_${SIRE_VERSION_PATCH}")

save_sire_variable( "SIRE_VERSION_MAJOR" "${SIRE_VERSION_MAJOR}" )
save_sire_variable( "SIRE_VERSION_MINOR" "${SIRE_VERSION_MINOR}" )
save_sire_variable( "SIRE_VERSION_PATCH" "${SIRE_VERSION_PATCH}" )
save_sire_variable( "SIRE_VERSION" "${SIRE_VERSION}" )
save_sire_variable( "SIRE_VERSION_STRING" "${SIRE_VERSION_STRING}" )

# Ensure that the Sire's directories are searched for header files
# before the system directories - this prevents problems in case
# an older version of Sire is already installed on the system
INCLUDE_DIRECTORIES( BEFORE ${SIRE_SOURCE_DIR} )

# Turn off FLOP counting (timing) by default - this adds a small runtime penalty
option (SIRE_TIME_FLOPS "Turn on floating point timing" OFF)
save_sire_variable( "SIRE_TIME_FLOPS" "${SIRE_TIME_FLOPS}" )

# Build shared libraries by default
set (BUILD_SHARED_LIBS ON)

# This is where you set your own compiler flags
set (SIRE_USER_CFLAGS "" CACHE STRING "User supplied C compiler flags" )
set (SIRE_USER_CPPFLAGS "" CACHE STRING "User supplied C++ compiler flags" )

save_sire_variable( "SIRE_USER_CFLAGS" "${SIRE_USER_CFLAGS}" )
save_sire_variable( "SIRE_USER_CPPFLAGS", "${SIRE_USER_CPPFLAGS}" )

# Name of executable install dir
set (SIRE_BIN "bin")
# Shared library install dir
set (SIRE_LIBS "lib")
# Static library install dir
set (SIRE_ARCHIVES "lib/static")
# Include (header) files install dir
set (SIRE_INCLUDES "include/Sire")
# CMake install files install dir
set (SIRE_CMAKEFILES "include/Sire/cmake")
# Sire share directory (for parameters etc.)
set (SIRE_SHARE "share/Sire")

save_sire_variable( "SIRE_INSTALL_PREFIX" "${CMAKE_INSTALL_PREFIX}" )
save_sire_variable( "SIRE_BIN" "${SIRE_BIN}" )
save_sire_variable( "SIRE_LIBS" "${SIRE_LIBS}" )
save_sire_variable( "SIRE_ARCHIVES" "${SIRE_ARCHIVES}" )
save_sire_variable( "SIRE_INCLUDES" "${SIRE_INCLUDES}" )
save_sire_variable( "SIRE_SHARE" "${SIRE_SHARE}" )
save_sire_variable( "SIRE_CMAKEFILES" "${SIRE_CMAKEFILES}" )

# Uncomment for more verbose compiling/linking
# (or run 'make VERBOSE=1')
# set (CMAKE_VERBOSE_MAKEFILE ON)

# Path to the project's extra cmake files
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build/cmake)

# Options to pass to 'strip' when stripping the libraries
# (on linux, aix and mac need to use '-x' to ensure we leave
#  the global symbols)
set ( SIRE_STRIP_OPTIONS "-x" CACHE STRING "Options to pass to 'strip'" )
save_sire_variable( "SIRE_STRIP_OPTIONS" "${SIRE_STRIP_OPTIONS}" )

set ( SIRE_STRIP_COMMAND "${CMAKE_STRIP} ${SIRE_STRIP_OPTIONS}" )

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/cmake_strip.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_strip"
  IMMEDIATE @ONLY)

set ( CMAKE_STRIP "${CMAKE_CURRENT_BINARY_DIR}/cmake_strip" )

# Add option to turn on or off symbol hiding
option ( SIRE_SYMBOL_HIDING "Turn on library symbol hiding" ON )
save_sire_variable( "SIRE_SYMBOL_HIDING" "${SIRE_SYMBOL_HIDING}" )

# Add option to turn on or off OpenMP compilation
option ( SIRE_OPENMP "Turn on OpenMP support" ON )
save_sire_variable( "SIRE_OPENMP" "${SIRE_OPENMP}" )

# Add option to turn on or off vectorisation of the code
option ( SIRE_VECTORISE "Turn on code vectorisation" ON )
save_sire_variable( "SIRE_VECTORISE" "${SIRE_VECTORISE}" )

# Add an option to force off Fortran compilation and linking
# (it may not work on this platform!)
option ( SIRE_DISABLE_FORTRAN "Turn off Fortran compilation and linking" OFF )
save_sire_variable( "SIRE_DISABLE_FORTRAN" "${SIRE_DISABLE_FORTRAN}" )

if (SIRE_DISABLE_FORTRAN)
  message( STATUS "Disabling any code that requires compilation of linking to Fortran" )
endif()

# Add an option to force the compilation to ignore any MPI libraries
# (this is useful on clusters where the MPI-enabled build cannot run
#  on the head node, and yet you still want to run tests!)
option( SIRE_DISABLE_MPI "Turn off MPI compilation and linking" OFF )

# Sire depends on >= Qt 4.2
find_package( Qt4 4.2.0 COMPONENTS QtCore QtXml QtGui QtOpenGL QtWebKit )

if ( ${QT_QTCORE_FOUND} )
   include (${QT_USE_FILE})
   message( STATUS "Using Qt version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} "
                   "(${QT_LIBRARIES})" )

   save_sire_variable( "SIRE_QT_VERSION_MAJOR" "${QT_VERSION_MAJOR}" )
   save_sire_variable( "SIRE_QT_VERSION_MINOR" "${QT_VERSION_MINOR}" )
   save_sire_variable( "SIRE_QT_VERSION_PATCH" "${QT_VERSION_PATCH}" )
else()
   message( FATAL_ERROR "Qt >= 4.2 is required by Sire" )
endif()

# Sire depends on >= boost 1.31 - all libraries must be dynamically linked
set (BOOST_ALL_DYN_LINK "YES")
set (Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0")
FIND_PACKAGE ( Boost 1.31 REQUIRED )

if (Boost_FOUND)
  message(STATUS "Boost paths ${Boost_LIBRARY_DIRS} | ${Boost_INCLUDE_DIR}" )
  set ( BOOST_INCLUDE_DIRS "${Boost_INCLUDE_DIR}" )
  include_directories( ${Boost_INCLUDE_DIR} )

  #save the path to this include directory so that it can be
  #used by anything compiling against Sire
  save_sire_variable( "SIRE_Boost_INCLUDE_DIR" "${Boost_INCLUDE_DIR}" )
else()
  message(FATAL_ERROR "Cannot find the boost libraries.")
endif()  

# Now look for OpenMM
option (OPENMM_HOME "The location of the OpenMM installation" "$ENV{HOME}/local")
FIND_PACKAGE( OpenMM )

save_sire_variable( "SIRE_OpenMM_FOUND" "${OpenMM_FOUND}" )

if ( ${OpenMM_FOUND} )
    message( STATUS "Found OpenMM at ${OpenMM_ROOT_DIR}" )
    save_sire_variable( "SIRE_OpenMM_INCLUDE_DIR" "${OpenMM_INCLUDE_DIR}" )
    save_sire_variable( "SIRE_OpenMM_LIBRARIES" "${OpenMM_LIBRARIES}" )
    save_sire_variable( "SIRE_OpenMM_LIBRARY_DIR" "${OpenMM_LIBRARY_DIR}" )
else()
    message( STATUS "Could not find OpenMM. OpenMM support is disabled." )
endif()

# Now look for BLAS and LAPACK - if these are not found then
# a local version will have to be used instead
set( BLA_STATIC OFF )
find_package( BLAS )
find_package( LAPACK )

if (BLAS_FOUND)
  message( STATUS "Found BLAS library ${BLAS_LIBRARIES}" )
  set( SIRE_USE_BLAS ON )
  save_sire_variable( "SIRE_BLAS_LIBRARIES" "${BLAS_LIBRARIES}" )
else()
  message( STATUS "Using internal (non-optimal) BLAS functions." )
  save_sire_variable( SIRE_USE_BLAS OFF )
endif()

save_sire_variable( "SIRE_USE_BLAS" "${SIRE_USE_BLAS}" )

if (LAPACK_FOUND)
  message( STATUS "Found LAPACK library ${LAPACK_LIBRARIES}" )
  set( SIRE_USE_LAPACK ON )
  save_sire_variable( "SIRE_LAPACK_LIBRARIES" "${LAPACK_LIBRARIES}" )
else()
  message( STATUS "Using internal (non-optimal) LAPACK functions." )
  set( SIRE_USE_LAPACK OFF )
endif()

save_sire_variable( "SIRE_USE_LAPACK" "${SIRE_USE_LAPACK}" )

if (SIRE_DISABLE_MPI)
  message( STATUS "Disabling MPI support in Sire" )

else()
  # Now look for MPI libraries
  find_package( MPI )

  if (MPI_FOUND)
    message( STATUS "Found MPI, so switching MPI support on" )
    set ( SIRE_USE_MPI ON )
  else()
    message( STATUS "MPI could not be found. MPI support switched off" )
    set ( SIRE_USE_MPI OFF )
  endif()
endif()

# Now run some platform tests - the results will be placed in config.h
include (CheckIncludeFiles)
include (CheckSymbolExists)
include (CheckFunctionExists)

check_include_files( unistd.h HAVE_UNISTD_H )
check_function_exists( lseek64 HAVE_LSEEK64 )

check_include_files( malloc/malloc.h HAVE_MALLOC_MALLOC_H )
check_include_files( emmintrin.h HAVE_EMMINTRIN_H )
check_include_files( execinfo.h HAVE_EXECINFO_H )
check_function_exists( mstats HAVE_MSTATS )
check_function_exists( mallinfo HAVE_MALLINFO )
check_function_exists( sysctl HAVE_SYSCTL )
check_function_exists( cbrt HAVE_CUBEROOT )
check_symbol_exists( backtrace "execinfo.h" HAVE_BACKTRACE )

#create config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/sire_config.h.in 
               ${CMAKE_CURRENT_BINARY_DIR}/sire_config.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/siren_config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/siren_config.h)

#  We need to get the compiler flags for different options
set( SIRE_SMALL_FLAGS "" CACHE INTERNAL "Compiler flags to produce small code" )
set( SIRE_WARNALL_FLAGS "" CACHE INTERNAL "Compiler flags to turn on all warnings" )
set( SIRE_DEBUG_FLAGS "" CACHE INTERNAL "Compiler flags to produce debugging code" )
set( SIRE_RELEASE_FLAGS "" CACHE INTERNAL "Compiler flags to produce optimised release code" )
set( SIRE_OPENMP_FLAGS "" CACHE INTERNAL "Compiler flags to activate OpenMP" )
set( SIRE_VECTOR_FLAGS "" CACHE INTERNAL "Compiler flags to vectorise code (e.g. SSE)" )
set( SIRE_VISIBILITY_FLAGS "" CACHE INTERNAL "Compiler flags to activate symbol visibility" )
set( SIRE_PLATFORM_FLAGS "" CACHE INTERNAL "Additional flags needed by the compiler" )
set( SIRE_SHARE_LINK_FLAGS "" CACHE INTERNAL "Flags needed to link shared libraries" )
set( SIRE_STATIC_LINK_FLAGS "" CACHE INTERNAL "Flags needed to link static libraries" )
set( SIRE_EXE_LINK_FLAGS "" CACHE INTERNAL "Flags needed to link executables" )

include (CheckCXXCompilerFlag)

# Add compiler specific flags
if ( CMAKE_COMPILER_IS_GNUCC )

    message( STATUS "Detected and using GCC C and C++ compilers" )

    # get the compiler version (via gcc -dumpversion)
    # (I'll assume that we are using the same version
    # of compiler for C and C++...)
    EXEC_PROGRAM( ${CMAKE_CXX_COMPILER}
                  ARGS -dumpversion
                  OUTPUT_VARIABLE exe_output )

    # These two regexps are copied from the Chicken CMake
    # files (which provide a very useful cmake tutorial - thanks :-)                  
    SET( VERSION_BUILD_REGEX "^([0-9]+)[.]([0-9]+).*$" )
    STRING( REGEX REPLACE ${VERSION_BUILD_REGEX} "\\1"
            GCC_MAJOR_VERSION ${exe_output} )
    STRING( REGEX REPLACE ${VERSION_BUILD_REGEX} "\\2"
            GCC_MINOR_VERSION ${exe_output} )
        
    message( STATUS "Using GCC version "
                    "${GCC_MAJOR_VERSION}.${GCC_MINOR_VERSION}" )

    # First, we need at least GCC 3.0, as I don't think that
    # GCC 2.x can handle the template code that I use
    if ( GCC_MAJOR_VERSION LESS 3 )
       message( FATAL_ERROR "Sire requires GCC >= 3.0. Please upgrade "
                      "your version of GCC." )
    endif()

    set ( SIRE_SMALL_FLAGS  "-Os" )
    set ( SIRE_WARNALL_FLAGS  "-Wall" )
    set ( SIRE_DEBUG_FLAGS   "-g" )

    set ( SIRE_RELEASE_FLAGS "-O3 -ffast-math" )

    if ( NOT APPLE )
      # -fomit-frame-pointer breaks backtrace on an apple
      set ( SIRE_RELEASE_FLAGS "${SIRE_RELEASE_FLAGS} -fomit-frame-pointer" )
    endif()

    set ( SIRE_OPENMP_FLAGS "-fopenmp" )
    set ( SIRE_PLATFORM_FLAGS "-pipe" )
    set ( SIRE_STATIC_LINK_FLAGS "-static" )

    if ( HAVE_EMMINTRIN_H )
      # we have emmintrin.h so we have SSE
      set ( SIRE_VECTOR_FLAGS "-msse2 -mfpmath=sse -DSIRE_USE_SSE" )
    endif()
        
    # Now gcc 4 specific options
    if ( GCC_MAJOR_VERSION GREATER 3 )
      # remove warnings about strict aliasing (lots from Qt4
      # when compiling with GCC 4, and they are annoying!)
      set( SIRE_PLATFORM_FLAGS "${SIRE_PLATFORM_FLAGS} -Wno-strict-aliasing" )

      # gcc-4.2 and above has openMP - turn it on (except for Mac as libgomp is broken!)
      if (GCC_MINOR_VERSION GREATER 1)
          if (SIRE_OPENMP)
              if (APPLE)
                message( STATUS "OpenMP support is unavailable as OpenMP + pthreads "
                                "is broken in Apple gcc4" )
                set( SIRE_OPENMP_FLAGS "-fno-openmp" )
              endif()
          endif()
      endif()

      set ( SIRE_VISIBILITY_FLAGS "-DSIRE_VISIBILITY_AVAILABLE -fvisibility=hidden -fvisibility-inlines-hidden" )
  
    else()
      set ( SIRE_VISIBILITY_FLAGS "-DSIRE_NO_VISIBILITY_AVAILABLE" ) 
    endif()

    # Ensure that the libraries don't contain any undefined symbols
    if(APPLE)
        set ( SIRE_SHARE_LINK_FLAGS "${SIRE_SHARE_LINK_FLAGS} -Wl" )
    else(UNIX)
        set ( SIRE_SHARE_LINK_FLAGS "${SIRE_SHARE_LINK_FLAGS} -rdynamic -Wl,--no-undefined" )
    endif()

elseif ( ${CMAKE_CXX_COMPILER} MATCHES "xlC" )
   
    message( STATUS "Detected the xlc and xlC C and C++ compilers" ) 

    set ( SIRE_SMALL_FLAGS  "-qcompact" )
    set ( SIRE_WARNALL_FLAGS  "-qwarn64 -qflag=w:w" )
    set ( SIRE_RELEASE_FLAGS "-O3 -qstrict" )
    set ( SIRE_DEBUG_FLAGS   "-g" )
    set ( SIRE_VISIBILITY_FLAGS "-DSIRE_NO_VISIBILITY_AVAILABLE" )
    set ( SIRE_PLATFORM_FLAGS "-qrtti=all -qthreaded" )

    set ( SIRE_SHARE_LINK_FLAGS "-qmkshrobj" )

    # Turn off Fortran code because this produces weird link errors!
    message( STATUS "Disabling Fortran code as this leads to link errors!" )
    message( STATUS "This will disable sire_blas, sire_lapack and sire_linpack" )
    set ( SIRE_DISABLE_FORTRAN ON )

    # This is a hack to automatically turn on MPI support on HPCx
    if ( NOT SIRE_DISABLE_MPI )
      if ( NOT SIRE_USE_MPI )
        set( SIRE_USE_MPI ON )
        set( MPI_INCLUDE_PATH "/usr/lpp/ppe.poe/include;/opt/rsct/lapi/include" )
        set( MPI_LIBRARY "-L/usr/lpp/ppe.poe/lib -L/usr/lpp/ppe.poe/lib/ip -lmpi_r -lvtd_r -llapi_r -binitfini:poe_remote_main" )
        set( MPI_EXTRA_LIBRARY "-L/usr/lpp/ppe.poe/lib/threads -lpthreads" )
        set( MPI_DEFINITIONS "-D_MPI_CPP_BINDINGS" )
      endif()
    endif()

elseif( ${CMAKE_CXX_COMPILER} MATCHES "icpc" )

    message( STATUS "Detected the Intel C++ and C compilers" )

    set ( SIRE_SMALL_FLAGS  "-Os" )
    set ( SIRE_WARNALL_FLAGS  "-w1" )
    set ( SIRE_RELEASE_FLAGS  "-O3 -fomit-frame-pointer -inline-level=2" )
    set ( SIRE_VECTOR_FLAGS  "-DSIRE_USE_SSE -msse3" )
    set ( SIRE_DEBUG_FLAGS   "-g" )
    set ( SIRE_VISIBILITY_FLAGS "-DSIRE_NO_VISIBILITY_AVAILABLE" )    
    set ( SIRE_OPENMP_FLAGS "-openmp" )
    set ( SIRE_SHARE_LINK_FLAGS "-shared" )
    set ( SIRE_STATIC_LINK_FLAGS "-static" )

else()
    message( STATUS "CMAKE_SYSTEM_NAME == ${CMAKE_SYSTEM_NAME}" )
    message( STATUS "CMAKE_C_COMPILER == ${CMAKE_C_COMPILER}" )
    message( STATUS "CMAKE_CXX_COMPILER == ${CMAKE_CXX_COMPILER}" )
    message( FATAL_ERROR "Could not detect compiler type - cannot set compiler options." )
endif()

message( STATUS  "CMAKE_SYSTEM_NAME      == ${CMAKE_SYSTEM_NAME}" )
message( STATUS  "CMAKE_C_COMPILER       == ${CMAKE_C_COMPILER}" )
message( STATUS  "CMAKE_CXX_COMPILER     == ${CMAKE_CXX_COMPILER}" )
message( STATUS  "CMAKE_Fortran_COMPILER == ${CMAKE_Fortran_COMPILER}" )

message ( STATUS "SIRE_SMALL_FLAGS       == ${SIRE_SMALL_FLAGS}" )
message ( STATUS "SIRE_WARNALL_FLAGS     == ${SIRE_WARNALL_FLAGS}" )
message ( STATUS "SIRE_DEBUG_FLAGS       == ${SIRE_DEBUG_FLAGS}" )
message ( STATUS "SIRE_RELEASE_FLAGS     == ${SIRE_RELEASE_FLAGS}" )
message ( STATUS "SIRE_OPENMP_FLAGS      == ${SIRE_OPENMP_FLAGS}" )
message ( STATUS "SIRE_VECTOR_FLAGS      == ${SIRE_VECTOR_FLAGS}" )
message ( STATUS "SIRE_VISIBILITY_FLAGS  == ${SIRE_VISIBILITY_FLAGS}" )
message ( STATUS "SIRE_PLATFORM_FLAGS    == ${SIRE_PLATFORM_FLAGS}" )
message ( STATUS "SIRE_SHARE_LINK_FLAGS  == ${SIRE_SHARE_LINK_FLAGS}" )
message ( STATUS "SIRE_STATIC_LINK_FLAGS == ${SIRE_STATIC_LINK_FLAGS}" )
message ( STATUS "SIRE_EXE_LINK_FLAGS    == ${SIRE_EXE_LINK_FLAGS}" )

set( SIRE_COMPILE_FLAGS "${SIRE_WARNALL_FLAGS} ${SIRE_PLATFORM_FLAGS}" )

if ( SIRE_TIME_FLOPS )
  set ( SIRE_COMPILE_FLAGS "${SIRE_COMPILE_FLAGS} -DSIRE_TIME_ROUTINES" )
endif()

if ( SIRE_OPENMP )
  set ( SIRE_COMPILE_FLAGS "${SIRE_COMPILE_FLAGS} ${SIRE_OPENMP_FLAGS}" )
endif()

if ( SIRE_SYMBOL_HIDING )
  set ( SIRE_COMPILE_FLAGS "${SIRE_COMPILE_FLAGS} ${SIRE_VISIBILITY_FLAGS}" )
  set ( SIRE_SHARE_LINK_FLAGS "${SIRE_SHARE_LINK_FLAGS} ${SIRE_VISIBILITY_FLAGS}" )
endif()

if ( SIRE_VECTORISE )
  set ( SIRE_COMPILE_FLAGS "${SIRE_COMPILE_FLAGS} ${SIRE_VECTOR_FLAGS}" )
endif()

set( SIRE_C_FLAGS_RELEASE "${SIRE_COMPILE_FLAGS} ${SIRE_RELEASE_FLAGS} ${SIRE_USER_CFLAGS}" )
set( SIRE_CXX_FLAGS_RELEASE "${SIRE_COMPILE_FLAGS} ${SIRE_RELEASE_FLAGS}  ${SIRE_USER_CPPFLAGS}" )

set( SIRE_C_FLAGS_DEBUG "${SIRE_COMPILE_FLAGS} ${SIRE_DEBUG_FLAGS} ${SIRE_USER_CFLAGS}" )
set( SIRE_CXX_FLAGS_DEBUG "${SIRE_COMPILE_FLAGS} ${SIRE_DEBUG_FLAGS} ${SIRE_USER_CPPFLAGS}" )

set( SIRE_C_FLAGS_SMALL "${SIRE_COMPILE_FLAGS} ${SIRE_SMALL_FLAGS} ${SIRE_USER_CFLAGS}" )
set( SIRE_CXX_FLAGS_SMALL "${SIRE_COMPILE_FLAGS} ${SIRE_SMALL_FLAGS} ${SIRE_USER_CPPFLAGS}" )

# Copy the Sire compile flags to CMake
set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${SIRE_C_FLAGS_RELEASE}" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${SIRE_CXX_FLAGS_RELEASE}" )
set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${SIRE_C_FLAGS_DEBUG}" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SIRE_CXX_FLAGS_DEBUG}" )
set( CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} ${SIRE_C_FLAGS_SMALL}" )
set( CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${SIRE_CXX_FLAGS_SMALL}" )

if ( CMAKE_BUILD_TYPE )
    message( STATUS "Compiling Sire using ${CMAKE_BUILD_TYPE}" )
else()
    message( STATUS "Compiling Sire using a default (RELEASE) build" )
    set (CMAKE_C_FLAGS "${SIRE_C_FLAGS_RELEASE}" )
    set (CMAKE_CXX_FLAGS "${SIRE_CXX_FLAGS_RELEASE}" )
endif()

message ( STATUS "C compiler flags       == ${CMAKE_C_FLAGS}" )
message ( STATUS "C++ compiler flags     == ${CMAKE_CXX_FLAGS}" )

# Add this to all shared libraries
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SIRE_SHARE_LINK_FLAGS}" )
set( CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${SIRE_STATIC_LINK_FLAGS}" )
set( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${SIRE_EXE_LINK_FLAGS}" )

message( STATUS "Shared library flags    == ${CMAKE_SHARED_LINKER_FLAGS}" )
message( STATUS "Static library flags    == ${CMAKE_STATIC_LINKER_FLAGS}" )
message( STATUS "Executable link flags   == ${CMAKE_EXE_LINKER_FLAGS}" )

# Now save the compile variables so that they can be re-used by libraries that use Sire
save_sire_variable( "SIRE_C_COMPILER" "${CMAKE_C_COMPILER}" )
save_sire_variable( "SIRE_CXX_COMPILER" "${CMAKE_CXX_COMPILER}" )
save_sire_variable( "SIRE_C_FLAGS_RELEASE" "${SIRE_C_FLAGS_RELEASE}" )
save_sire_variable( "SIRE_CXX_FLAGS_RELEASE" "${SIRE_CXX_FLAGS_RELEASE}" )
save_sire_variable( "SIRE_C_FLAGS_SMALL" "${SIRE_C_FLAGS_SMALL}" )
save_sire_variable( "SIRE_CXX_FLAGS_SMALL" "${SIRE_CXX_FLAGS_SMALL}" )
save_sire_variable( "SIRE_C_FLAGS_DEBUG" "${SIRE_C_FLAGS_DEBUG}" )
save_sire_variable( "SIRE_CXX_FLAGS_DEBUG" "${SIRE_CXX_FLAGS_DEBUG}" )
save_sire_variable( "SIRE_SHARE_LINK_FLAGS" "${SIRE_SHARE_LINK_FLAGS}" )
save_sire_variable( "SIRE_STATIC_LINK_FLAGS" "${SIRE_STATIC_LINK_FLAGS}" )
save_sire_variable( "SIRE_EXE_LINK_FLAGS" "${SIRE_EXE_LINK_FLAGS}" )
save_sire_variable( "SIRE_SMALL_FLAGS" "${SIRE_SMALL_FLAGS}" )
save_sire_variable( "SIRE_WARNALL_FLAGS" "${SIRE_WARNALL_FLAGS}" )
save_sire_variable( "SIRE_DEBUG_FLAGS" "${SIRE_DEBUG_FLAGS}" )
save_sire_variable( "SIRE_RELEASE_FLAGS" "${SIRE_RELEASE_FLAGS}" )
save_sire_variable( "SIRE_OPENMP_FLAGS" "${SIRE_OPENMP_FLAGS}" )
save_sire_variable( "SIRE_VECTOR_FLAGS" "${SIRE_VECTOR_FLAGS}" )
save_sire_variable( "SIRE_VISIBILITY_FLAGS" "${SIRE_VISIBILITY_FLAGS}" )
save_sire_variable( "SIRE_PLATFORM_FLAGS" "${SIRE_PLATFORM_FLAGS}" )

#include the build directory so that we can get sire_config.h and sire_version.h
include_directories(${CMAKE_BINARY_DIR})

# Add the compiler test directory - this allows us to run some compiler
# tests to make sure that Sire will compile properly
add_subdirectory (build/test_compiler)

# Recurse into the src/libs and src/apps subdirectories
add_subdirectory (src/libs)
add_subdirectory (src/apps)

# Finally, write the version header file and finish off the component dependencies file
include( GetSvnVersion )

# install the extra includes file in the Sire/cmake directory
install (FILES ${SIRE_VARIABLES_FILE} 
         DESTINATION ${SIRE_CMAKEFILES}
        )

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/sire_config.h
         DESTINATION ${SIRE_INCLUDES})

# install the parameters file (containing forcefield parameters) into
# share/Sire
install (DIRECTORY ${CMAKE_SOURCE_DIR}/parameters
         DESTINATION ${SIRE_SHARE})

# These commands are used to create an 'uninstall' target
# (this is copied from the vtk.org wiki)
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
