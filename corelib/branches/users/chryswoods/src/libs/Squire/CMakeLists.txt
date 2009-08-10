########################################
#
# CMake file for library:Squire
#
########################################

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# We need LAPACK to compile the HF test program
set( BLA_STATIC OFF )
find_package( BLAS )
find_package( LAPACK )

if (BLAS_FOUND AND LAPACK_FOUND)
  message( STATUS "Activating squire_lapack and squire_blas" )
  message( STATUS "Using BLAS libraries ${BLAS_LIBRARIES}" )
  message( STATUS "Using LAPACK libraries ${LAPACK_LIBRARIES}" )
  set( SQUIRE_HAVE_LAPACK_AND_BLAS ON )
else()
  message( STATUS "BLAS or LAPACK are not available. squire_lapack and squire_blas cannot be built." )
  set( SQUIRE_HAVE_LAPACK_AND_BLAS OFF )
endif()

if (SQUIRE_HAVE_LAPACK_AND_BLAS)

  set ( SQUIRE_BLAS_FUNCTIONS
        DGEMM
      )

  set ( SQUIRE_LAPACK_FUNCTIONS
        DSYEV
      )

  include( FortranCInterface )
  create_fortran_c_interface( Squire SQUIRE_LAPACK_FUNCTIONS squire_lapack_f.h )
  create_fortran_c_interface( Squire SQUIRE_BLAS_FUNCTIONS squire_blas_f.h )

  add_definitions( -DSQUIRE_HAVE_BLAS_AND_LAPACK )

  set( SQUIRE_BLAS_LAPACK_LIBRARIES ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} )

endif()

set ( SQUIRE_HEADERS

      qmff.h
      qmmmelecembedpotential.h
      qmmmff.h
      qmmmpotential.h
      qmpotential.h
      qmprogram.h

      hf.h
      latticecharges.h

      molpro.h

      squire_blas.h
      squire_lapack.h
    )

set ( SQUIRE_SOURCES

      register_squire.cpp

      qmff.cpp
      qmmmelecembedpotential.cpp
      qmmmff.cpp
      qmmmpotential.cpp
      qmpotential.cpp
      qmprogram.cpp

      hf.cpp
      latticecharges.cpp
      
      molpro.cpp

      squire_blas.cpp
      squire_lapack.cpp

      ${SQUIRE_HEADERS}
    )

#add the additional include files needed by Squire
include_directories( ${SQUIRE_INCLUDE_DIRS} )
  
add_library (Squire ${SQUIRE_SOURCES})

set_target_properties (Squire PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (Squire
                       SireMM
                       SireFF
                       SireMol
                       SireCAS
                       SireBase
                       SireStream
                       SireError
                       ${SQUIRE_BLAS_LAPACK_LIBRARIES}
                      )

# installation
install( TARGETS Squire EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SQUIRE_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/Squire )
