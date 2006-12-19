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
      editmol.py.cpp
      editres.py.cpp
      idmolatom.py.cpp
      molecule.py.cpp
      property.py.cpp
      residue.py.cpp

      qlist_siremol.py.cpp

      wrap_angles.cpp         
      wrap_atom.cpp           
      wrap_atomlist.cpp       
      wrap_atomnumlist.cpp    
      wrap_atomnumset.cpp     
      wrap_atomnumvector.cpp  
      wrap_atomset.cpp        
      wrap_atomvector.cpp       
      wrap_bonds.cpp            
      wrap_cutgroup.cpp         
      wrap_cutgroupset.cpp      
      wrap_dihedrals.cpp        
      wrap_idtypes.cpp      
      wrap_impropers.cpp    
      wrap_indexlist.cpp
      wrap_indexset.cpp        
      wrap_indexvector.cpp     
      wrap_internals.cpp       
      wrap_moleculebonds.cpp
      wrap_moleculeinfo.cpp
      wrap_residuebonds.cpp
      wrap_residueinfo.cpp
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
add_library (PythonWrapping_SireMol wrap_SireMol.cpp)

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
