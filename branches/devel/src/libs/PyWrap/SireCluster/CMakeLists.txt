########################################
#
# CMake file for Python wrapper:_Cluster
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

# Define the sources in SireCluster
set ( WRAPCLUSTER_SOURCES
      processor.py.cpp
      threadworker.py.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireCluster ${WRAPCLUSTER_SOURCES})

set_target_properties (PyWrap_SireCluster PROPERTIES
                       VERSION ${SIRE_VERSION}
		           SOVERSION ${SIRE_VERSION_MAJOR}
		       )
		       
target_link_libraries (PyWrap_SireCluster
                       SireCluster
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
		      )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireCluster
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireCluster wrap_SireCluster.cpp)

target_link_libraries (PythonWrapping_SireCluster
                       PyWrap_SireCluster
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireCluster
                       PROPERTIES
                         OUTPUT_NAME "Cluster"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Cluster )

install( TARGETS PythonWrapping_SireCluster
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
