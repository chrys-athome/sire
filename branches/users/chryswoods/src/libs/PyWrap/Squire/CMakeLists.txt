########################################
#
# CMake file for Python wrapper:_Squire
#
########################################

# Third Party dependencies of this module
include_directories( ${BOOST_INCLUDE_DIR}
                     ${PYTHON_INCLUDE_DIR} )

# Qt4
include(${QT_USE_FILE})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs
                    ${CMAKE_SOURCE_DIR}/src/libs/PyWrap)

# Define the sources in Squire
set ( WRAPSQUIRE_SOURCES

      _Squire.main.cpp

      squire_containers.cpp

      MolproCalculator.pypp.cpp
      MolproFF.pypp.cpp
      MolproProcessor.pypp.cpp
      MolproSession.pypp.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_Squire ${WRAPSQUIRE_SOURCES})

set_target_properties (PythonWrapping_Squire
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Squire"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

target_link_libraries (PythonWrapping_Squire
                       Squire
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Squire )

install( TARGETS PythonWrapping_Squire
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )
