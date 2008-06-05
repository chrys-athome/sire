########################################
#
# CMake file for library:ThirdPartyGUI
#
########################################

# need to link to openGL library
include (FindOpenGL)

IF (NOT OPENGL_FOUND)
   message( FATAL_ERROR "Require openGL!!!")
ENDIF (NOT OPENGL_FOUND)

add_library (ThirdPartyGUI
             glew.c)

set_target_properties (ThirdPartyGUI PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       )
include_directories ( ${OPENGL_INCLUDE_DIR} )

target_link_libraries ( ThirdPartyGUI 
                        ${OPENGL_LIBRARIES} )

# installation
install( TARGETS ThirdPartyGUI
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
