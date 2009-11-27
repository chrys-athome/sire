########################################
#
# CMake file for library:SireMaths
#
########################################

if (SIRE_DISABLE_FORTRAN)
  add_definitions( "-DSIRE_DISABLE_FORTRAN" )
  message( STATUS "Disabling SireMaths BLAS/LAPACK/LINPACK interfaces.")
  message( STATUS "Have to do this as Fortran code has been disabled.")

else()
  # We first create a library (SireBLASPACK) that provides an interface
  # to BLAS, LAPACK and LINPACK. This library either links
  # to those libraries, or it provides the necessary functions
  # from those libraries if they are not available.

  set( SIRE_BLAS_FUNCTIONS
       DGEMM
       DGEMV
     )

  set( SIRE_LAPACK_FUNCTIONS
       DSYEV
     )

  set( SIRE_LINPACK_FUNCTIONS
       DGECO
       DGEDI
     )

  if ( SIRE_USE_BLAS )
    list(APPEND SIREBLASPACK_LIBRARIES ${BLAS_LIBRARIES})
    set( SIREBLASPACK_HAS_LIBRARIES TRUE )
  else()
    set( SIRE_BLAS_SOURCES
         third_party/BLAS/idamax.f
         third_party/BLAS/dasum.f
         third_party/BLAS/daxpy.f
         third_party/BLAS/dcopy.f
         third_party/BLAS/ddot.f
         third_party/BLAS/dgemm.f
         third_party/BLAS/dgemv.f
         third_party/BLAS/dger.f
         third_party/BLAS/dnrm2.f
         third_party/BLAS/dscal.f
         third_party/BLAS/dswap.f
         third_party/BLAS/dsymv.f
         third_party/BLAS/dsyr2.f
         third_party/BLAS/dsyr2k.f
         third_party/BLAS/dtrmm.f
         third_party/BLAS/dtrmv.f
         third_party/BLAS/lsame.f
         third_party/BLAS/xerbla.f
       )
  endif()

  if ( SIRE_USE_LINPACK )
    list(APPEND SIREBLASPACK_LIBRARIES ${LINPACK_LIBRARIES})
    set( SIREBLASPACK_HAS_LIBRARIES TRUE )
  else()
    set( SIRE_LINPACK_SOURCES
         third_party/LINPACK/dgeco.f
         third_party/LINPACK/dgedi.f
         third_party/LINPACK/dgefa.f
       )
  endif()

  if ( SIRE_USE_LAPACK )
    list(APPEND SIREBLASPACK_LIBRARIES ${LAPACK_LIBRARIES})
    set( SIREBLASPACK_HAS_LIBRARIES TRUE )
  else()
    set( SIRE_LAPACK_SOURCES
         third_party/LAPACK/disnan.f
         third_party/LAPACK/dlae2.f
         third_party/LAPACK/dlaev2.f
         third_party/LAPACK/dlaisnan.f
         third_party/LAPACK/dlamch.f
         third_party/LAPACK/dlanst.f
         third_party/LAPACK/dlansy.f
         third_party/LAPACK/dlapy2.f
         third_party/LAPACK/dlascl.f
         third_party/LAPACK/dlarf.f
         third_party/LAPACK/dlarfb.f
         third_party/LAPACK/dlartg.f
         third_party/LAPACK/dlarfg.f
         third_party/LAPACK/dlarft.f
         third_party/LAPACK/dlaset.f
         third_party/LAPACK/dlasr.f
         third_party/LAPACK/dlasrt.f
         third_party/LAPACK/dlassq.f
         third_party/LAPACK/dlatrd.f
         third_party/LAPACK/dorg2l.f
         third_party/LAPACK/dorg2r.f
         third_party/LAPACK/dorgql.f
         third_party/LAPACK/dorgqr.f
         third_party/LAPACK/dorgtr.f
         third_party/LAPACK/dsyev.f
         third_party/LAPACK/dsytd2.f
         third_party/LAPACK/dsytrd.f
         third_party/LAPACK/dsteqr.f
         third_party/LAPACK/dsterf.f
         third_party/LAPACK/ilaenv.f
         third_party/LAPACK/iladlc.f
         third_party/LAPACK/iladlr.f
         third_party/LAPACK/ieeeck.f
         third_party/LAPACK/iparmq.f
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
endif()

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

if ( NOT SIRE_DISABLE_FORTRAN )
  # Define the sources in SireBLASPACK
  set( SIREBLASPACK_SOURCES
   
       ${SIRE_BLAS_SOURCES}
       ${SIRE_LAPACK_SOURCES}
       ${SIRE_LINPACK_SOURCES}

       sire_blaspack.f
     )
endif()

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

      third_party/GSL/fcmp.c
      third_party/GSL/fcmp.h

      ${SIREMATHS_HEADERS}
    )

if (NOT SIRE_DISABLE_FORTRAN)
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
endif()

add_library (SireMaths ${SIREMATHS_SOURCES})

set_target_properties (SireMaths PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries, and GSL
target_link_libraries (SireMaths
                       SireID
                       SireUnits
                       Siren
                       ${SIREMATHS_EXTRA_LIBRARIES}
                      )

if (NOT SIRE_DISABLE_FORTRAN)
  target_link_libraries (SireMaths SireBLASPACK )
endif()

save_sire_variable( "SIREMATHS_EXTRA_LIBRARIES" "${SIREMATHS_EXTRA_LIBRARIES}" )

# installation
install( TARGETS SireMaths EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

if (NOT SIRE_DISABLE_FORTRAN)
  install( TARGETS SireBLASPACK EXPORT SireLibraries
           RUNTIME DESTINATION ${SIRE_BIN}
           LIBRARY DESTINATION ${SIRE_LIBS}
           ARCHIVE DESTINATION ${SIRE_ARCHIVES}
         )
endif()

install( FILES ${SIREMATHS_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMaths )
