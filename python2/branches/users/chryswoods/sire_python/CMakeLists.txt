########################################
#
# CMake file for sire_python
# (mpi-aware python executable)
#
########################################

find_package( MPI )

if (MPI_FOUND)
  message( STATUS "Compiling sire_python MPI-aware python interpreter" )
  message( STATUS "MPI headers: ${MPI_INCLUDE_PATH}" )
  message( STATUS "MPI libraries: ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY}" )

  include_directories( ${MPI_INCLUDE_PATH} )

  add_definitions( -D__SIRE_USE_MPI__ )

  set ( SIREMPI_EXTRA_LIBRARIES  ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY} )

else()                                          
  message( STATUS "MPI wasn't found, so sire_python interpreter will not be built." )

endif()


# Third Party dependencies of this module
include_directories( ${BOOST_INCLUDE_DIR}
                     ${PYTHON_INCLUDE_DIR} )

# Sire include paths
include_directories( ${SIRE_INCLUDE_DIR} )

# Qt4
include(${QT_USE_FILE})

# Other python wrapping directories
include_directories(${CMAKE_SOURCE_DIR})

# Define the sources in Helpers
set ( SIREPYTHON_SOURCES

      main.cpp

    )

if (MPI_FOUND)
    # Create the library that holds all of the class wrappers
    add_executable (sire_python ${SIREPYTHON_SOURCES})

    set_target_properties (sire_python
                           PROPERTIES
                           VERSION ${SIRE_VERSION}
                          )
		       
    target_link_libraries (sire_python
                           ${SIREMPI_LIBRARY}
                           ${PYTHON_LIBRARIES}
                           ${SIREERROR_LIBRARY}
                           ${SIREMPI_EXTRA_LIBRARIES}
                 
                           ${SIRESYSTEM_LIBRARY}
                           ${SIREMOVE_LIBRARY}
                           ${SIREMM_LIBRARY}
                           ${SIREIO_LIBRARY}
                           ${SQUIRE_LIBRARY}

                           ${QT_QTCORE_LIBRARY}
                          )

    # installation
    install( TARGETS sire_python
             RUNTIME DESTINATION ${SIRE_BIN}
             LIBRARY DESTINATION ${SIRE_LIBS}
             ARCHIVE DESTINATION ${SIRE_ARCHIVES}
           )

endif()

