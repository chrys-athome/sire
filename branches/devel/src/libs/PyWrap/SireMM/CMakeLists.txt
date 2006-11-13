########################################
#
# CMake file for Python wrapper:_MM
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

# Define the sources in SireMM
set ( WRAPMM_SOURCES

      cljff.py.cpp
      intercljff.py.cpp 

      wrap_anglegenerator.cpp    
      wrap_angletable.cpp        
      wrap_assign_angles.cpp     
      wrap_assign_bonds.cpp      
      wrap_assign_dihedrals.cpp  
      wrap_bondgenerator.cpp     
      wrap_bondtable.cpp          
      wrap_chargetable.cpp        
      wrap_cljff.cpp              
      wrap_cljparameter.cpp       
      wrap_dihedralgenerator.cpp  
      wrap_dihedraltable.cpp      
      wrap_ljtable.cpp          
      wrap_mmdb2.cpp            
      wrap_mmdb.cpp             
      wrap_molangleinfo.cpp     
      wrap_molbondinfo.cpp      
      wrap_moldihedralinfo.cpp    
      wrap_protomsdb.cpp
      wrap_resangleinfo.cpp
      wrap_resbondinfo.cpp
      wrap_resdihedralinfo.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireMM ${WRAPMM_SOURCES})

set_target_properties (PyWrap_SireMM PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
		       )
		       
target_link_libraries (PyWrap_SireMM
                       SireMM
                       SireDB
                       SireFF
                       SireMol
                       PyWrap_SireFF
                       PyWrap_SireDB
                       PyWrap_SireMol
                       ${BOOST_PYTHON_LIBRARY}
		      )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireMM
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireMM wrap_SireMM.cpp)

target_link_libraries (PythonWrapping_SireMM
                       PyWrap_SireMM
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireMM
                       PROPERTIES
                         OUTPUT_NAME "MM"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/MM )

install( TARGETS PythonWrapping_SireMM
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
