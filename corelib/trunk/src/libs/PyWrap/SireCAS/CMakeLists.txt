########################################
#
# CMake file for Python wrapper:_CAS
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

# Define the sources in SireCAS
set ( WRAPCAS_SOURCES
      wrap_complexvalues.cpp
      wrap_exbase.cpp
      wrap_expressionbase.cpp
      wrap_expression.cpp
      wrap_function.cpp
      wrap_hyperbolicfuncs.cpp
      wrap_identities.cpp
      wrap_operators.cpp
      wrap_powerfuncs.cpp
      wrap_symbol.cpp
      wrap_trigfuncs.cpp
      wrap_values.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireCAS ${WRAPCAS_SOURCES})

set_target_properties (PyWrap_SireCAS PROPERTIES
                       VERSION ${SIRE_VERSION}
		       SOVERSION ${SIRE_VERSION_MAJOR}
		       )
		       
target_link_libraries (PyWrap_SireCAS
                       SireCAS
		       PyWrap_SireMaths
		      )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireCAS
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireCAS wrap_SireCAS.cpp)

target_link_libraries (PythonWrapping_SireCAS
                       PyWrap_SireCAS
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireCAS
                       PROPERTIES
                         OUTPUT_NAME "CAS"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/CAS )

install( TARGETS PythonWrapping_SireCAS
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
