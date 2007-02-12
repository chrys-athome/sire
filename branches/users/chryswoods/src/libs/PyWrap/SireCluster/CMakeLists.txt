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
      
      _Cluster.main.cpp
      
      Processor.pypp.cpp
      ThreadWorker.pypp.cpp
      WorkerBase.pypp.cpp
      
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireCluster ${WRAPCLUSTER_SOURCES})

set_target_properties (PythonWrapping_SireCluster 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
		             SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Cluster"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )
		       
target_link_libraries (PythonWrapping_SireCluster
                       SireCluster
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
		          )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Cluster )

install( TARGETS PythonWrapping_SireCluster
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
