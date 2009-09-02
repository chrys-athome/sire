########################################
#
# CMake file for library:SireMaths
#
########################################

# We first create a library (SireBLASPACK) that provides an interface
# to BLAS, LAPACK and LINPACK. This library either links
# to those libraries, or it provides the necessary functions
# from those libraries if they are not available.

set( SIRE_BLAS_FUNCTIONS
     DGEMM
     DGEMV
   )

set( SIRE_LAPACK_FUNCTIONS
   )

set( SIRE_LINPACK_FUNCTIONS
     DSYEV
     DGECO
     DGEDI
   )

if ( SIRE_USE_BLAS )
  list(APPEND SIREBLASPACK_LIBRARIES ${BLAS_LIBRARIES})
  set( SIREBLASPACK_HAS_LIBRARIES TRUE )
else()
  set( SIRE_BLAS_SOURCES
       ThirdParty/BLAS/dgemm.f
       ThirdParty/BLAS/dgemv.f
     )
endif()

if ( SIRE_USE_LINPACK )
  list(APPEND SIREBLASPACK_LIBRARIES ${LINPACK_LIBRARIES})
  set( SIREBLASPACK_HAS_LIBRARIES TRUE )
else()
  set( SIRE_LINPACK_SOURCES
       ThirdParty/LINPACK/dgeco.f
       ThirdParty/LINPACK/dgedi.f
       ThirdParty/LINPACK/dgefa.f
     )
endif()

if ( SIRE_USE_LAPACK )
  list(APPEND SIREBLASPACK_LIBRARIES ${LAPACK_LIBRARIES})
  set( SIREBLASPACK_HAS_LIBRARIES TRUE )
else()
  set( SIRE_LAPACK_SOURCES
     )
endif()

if (NOT CMAKE_GENERATOR STREQUAL "Xcode" )
  # We need to sort out the naming convention for the
  # fortran blas, lapack and linpack libraries
  include( FortranCInterface )
  create_fortran_c_interface( Sire SIRE_BLAS_FUNCTIONS sire_blas_f.h )
  create_fortran_c_interface( Sire SIRE_LAPACK_FUNCTIONS sire_lapack_f.h )
  create_fortran_c_interface( Sire SIRE_LINPACK_FUNCTIONS sire_linpack_f.h )
endif()

# SireMaths depends on >= GSL 1.6
find_package(GSL 1.6 REQUIRED)

message( STATUS "GSL paths ${GSL_LINK_DIRECTORIES} "
                "${GSL_LIBRARIES} | ${GSL_INCLUDE_DIR}" )

list(APPEND SIREMATHS_EXTRA_LIBRARIES ${GSL_LIBRARIES})

include_directories( BEFORE ${GSL_INCLUDE_DIR})

save_sire_variable( "SIRE_GSL_INCLUDE_PATH" "${GSL_INCLUDE_DIR}" )

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireBLASPACK
set( SIREBLASPACK_SOURCES
  
     ${SIRE_BLAS_SOURCES}
     ${SIRE_LAPACK_SOURCES}
     ${SIRE_LINPACK_SOURCES}

     sire_blaspack.f
   )

# Define the headers in SireMaths
set ( SIREMATHS_HEADERS
      accumulator.h
      axisset.h
      boys.h
      complex.h
      constants.h
      distvector.h
      errors.h
      freeenergyaverage.h
      gamma.h
      histogram.h
      line.h
      linearap.h
      maths.h
      matrix.h
      nmatrix.h
      nvector.h
      n4matrix.h
      plane.h
      quaternion.h
      rangenerator.h
      rational.h
      rotate.h
      sphere.h
      torsion.h
      triangle.h
      trigmatrix.h
      vector.h

      sire_blas.h
      sire_lapack.h
      sire_linpack.h
    )

# Define the sources in SireMaths
set ( SIREMATHS_SOURCES

      register_siremaths.cpp

      accumulator.cpp
      axisset.cpp
      boys.cpp
      complex.cpp
      distvector.cpp
      errors.cpp
      freeenergyaverage.cpp
      gamma.cpp
      histogram.cpp
      line.cpp
      linearap.cpp
      matrix.cpp
      nmatrix.cpp
      nvector.cpp
      n4matrix.cpp
      plane.cpp
      quaternion.cpp
      rangenerator.cpp
      rational.cpp
      sphere.cpp
      torsion.cpp
      triangle.cpp
      trigmatrix.cpp
      vector.cpp

      sire_blas.cpp
      sire_lapack.cpp
      sire_linpack.cpp

      ${SIREMATHS_HEADERS}
    )

add_library (SireBLASPACK ${SIREBLASPACK_SOURCES})

set_target_properties (SireBLASPACK PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

if ( SIREBLASPACK_HAS_LIBRARIES )
  target_link_libraries (SireBLASPACK
                         ${SIREBLASPACK_LIBRARIES}
                        )
endif()

add_library (SireMaths ${SIREMATHS_SOURCES})

set_target_properties (SireMaths PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries, and GSL
target_link_libraries (SireMaths
                       SireID
                       SireBase
                       SireUnits
                       SireStream
                       SireBLASPACK
                       ${SIREMATHS_EXTRA_LIBRARIES}
                      )

save_sire_variable( "SIREMATHS_EXTRA_LIBRARIES" "${SIREMATHS_EXTRA_LIBRARIES}" )

# installation
install( TARGETS SireMaths EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( TARGETS SireBLASPACK EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMATHS_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMaths )
