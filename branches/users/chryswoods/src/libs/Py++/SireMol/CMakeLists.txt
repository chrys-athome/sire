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

      QSet_AtomIndex_.pypp.cpp
     )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireMol ${WRAPMOL_SOURCES})

set_target_properties (PyWrap_SireMol PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
           )

target_link_libraries (PyWrap_SireMol
                       SireMol
                       SireMaths
                       PyWrap_SireMaths
                       PyWrap_SireQt
                       ${BOOST_PYTHON_LIBRARY}
          )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireMol
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireMol _Mol.main.cpp)

target_link_libraries (PythonWrapping_SireMol
                       PyWrap_SireMol
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireMol
                       PROPERTIES
                         OUTPUT_NAME "Mol"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Mol )

install( TARGETS PythonWrapping_SireMol
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )
