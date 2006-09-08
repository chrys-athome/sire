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

add_library (ThirdPartyGUI SHARED
             glew.c)

include_directories ( ${OPENGL_INCLUDE_DIR} )

target_link_libraries ( ThirdPartyGUI 
                        ${OPENGL_LIBRARIES} )
