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
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireBase
set ( WRAPBASE_SOURCES
      wrap_md5sum.cpp
    )

add_library (_Base ${WRAPBASE_SOURCES})

# Link to other Sire libraries, and GSL
target_link_libraries (_Base
                       SireBase
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                       )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Base )

install( TARGETS _Base
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
