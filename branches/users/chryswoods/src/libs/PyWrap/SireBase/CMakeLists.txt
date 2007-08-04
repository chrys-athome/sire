########################################
#
# CMake file for Python wrapper:_Base
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

# Define the sources in SireBase
set ( WRAPBASE_SOURCES
      
      _Base.main.cpp

      sirebase_containers.cpp
      
      sharedpolypointer.py.cpp
      
      IDMajMinVersion.pypp.cpp
      IDVersion.pypp.cpp
      MajMinVersion.pypp.cpp
      MajVersion.pypp.cpp
      MD5Sum.pypp.cpp
      Properties.pypp.cpp
      PropertyBase.pypp.cpp
      Property.pypp.cpp
      VariantProperty.pypp.cpp
      Version.pypp.cpp
      
      ConcreteProperty_Properties_PropertyBase_.pypp.cpp
      ConcreteProperty_VariantProperty_PropertyBase_.pypp.cpp
      
      _Base_free_functions.pypp.cpp
      _Base_global_variables.pypp.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireBase ${WRAPBASE_SOURCES})

set_target_properties (PythonWrapping_SireBase  
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Base"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
		       )
		       
target_link_libraries (PythonWrapping_SireBase
                       SireBase
   		           SireStream
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Base )

install( TARGETS PythonWrapping_SireBase
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
