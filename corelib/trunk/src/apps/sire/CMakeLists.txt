########################################
#
# CMake file for application:sire
#
########################################

# Third Party dependencies of this module
include_directories(${BOOST_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in sire
set ( SIRE_SOURCES
      main.cpp
      mainapp.cpp
    )

# Define files that need processing with moc
set ( SIRE_MOCS mainapp.h )

# add the rules to run moc
qt4_wrap_cpp( SIRE_MOC_SRCS ${SIRE_MOCS} )

# generate the application
add_executable ( sire ${SIRE_SOURCES} ${SIRE_MOC_SRCS} )

set_target_properties (sire PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (sire
                       SirePy
                      )

# installation
install( TARGETS sire
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
