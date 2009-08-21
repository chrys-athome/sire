########################################
#
# CMake file for library:SireMaths
#
########################################

# We need to sort out the naming convention for the 
# fortran blas and lapack libraries
if ( SIRE_USE_BLAS OR SIRE_USE_LAPACK )
  include( FortranCInterface )
endif()

if ( SIRE_USE_BLAS )
  set( SIRE_BLAS_FUNCTIONS
       DGEMM
       DGEMV
     )

  if (NOT CMAKE_GENERATOR STREQUAL "Xcode" )
    create_fortran_c_interface( Sire SIRE_BLAS_FUNCTIONS sire_blas_f.h )
  endif()

  add_definitions( -DSIRE_HAVE_BLAS )
endif()

if ( SIRE_USE_LAPACK )
  set( SIRE_LAPACK_FUNCTIONS
       DSYEV
       DGECO
       DGEDI
     )

  set( SIRE_LINPACK_SOURCES
       dgeco.f
       dgedi.f
       dgefa.f
     )

  if (NOT CMAKE_GENERATOR STREQUAL "Xcode" )
    create_fortran_c_interface( Sire SIRE_LAPACK_FUNCTIONS sire_lapack_f.h )
  endif()

  add_definitions( -DSIRE_HAVE_LAPACK )
endif()

# SireMaths depends on >= GSL 1.6
find_package(GSL 1.6 REQUIRED)

message( STATUS "GSL paths ${GSL_LINK_DIRECTORIES} "
                "${GSL_LIBRARIES} | ${GSL_INCLUDE_DIR}" )

include_directories( BEFORE ${GSL_INCLUDE_DIR})

save_sire_variable( "SIRE_GSL_INCLUDE_PATH" "${GSL_INCLUDE_DIR}" )

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

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

      ${SIRE_LINPACK_SOURCES}

      ${SIREMATHS_HEADERS}
    )

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
                       ${GSL_LIBRARIES}
                       ${LAPACK_LIBRARIES}
                       ${BLAS_LIBRARIES}
                      )

save_sire_variable( "SIREMATHS_EXTRA_LIBRARIES" "${GSL_LIBRARIES}" )

# installation
install( TARGETS SireMaths EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMATHS_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMaths )
          
