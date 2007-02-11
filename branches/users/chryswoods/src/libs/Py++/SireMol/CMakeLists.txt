########################################
#
# CMake file for Python wrapper:_Mol
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

# Define the sources in SireMol
set ( WRAPMOL_SOURCES
      AtomIndex.pypp.cpp
      MoleculeGroup.pypp.cpp
      Molecule.pypp.cpp

      QSet_AtomIndex_.py.cpp
      
      _Mol.main.cpp
     )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireMol ${WRAPMOL_SOURCES})

set_target_properties (PythonWrapping_SireMol 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Mol"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
           )

target_link_libraries (PythonWrapping_SireMol
                       SireMol
                       SireMaths
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
          )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Mol )

install( TARGETS PythonWrapping_SireMol
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )
