########################################
#
# CMake file for application:conspire
#
########################################

# Make sure that the Conspire 'Conspire' include directory is searched first
# when looking for header files
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/SirenCore )
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/Conspire/third_party )
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR} )
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/GUI )

# Siren depends on >= Qt 4.4
find_package( Qt4 4.4.0 COMPONENTS QtCore QtGui QtOpenGL )

if ( ${QT_QTCORE_FOUND} )
   include (${QT_USE_FILE})
   message( STATUS "Using Qt version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} "
                   "(${QT_LIBRARIES})" )
else()
   message( FATAL_ERROR "Qt >= 4.4 is required by Sire" )
endif()

include_directories(${QT_QTCORE_INCLUDE_DIR}
                    ${QT_QTGUI_INCLUDE_DIR}
                    ${QT_QTOPENGL_INCLUDE_DIR})

set ( CONSPIRE_GUI_MOC_HEADERS

      Conspire/GUI/addpage.h
      Conspire/GUI/animation.h
      Conspire/GUI/button.h
      Conspire/GUI/configdocument.h
      Conspire/GUI/configpage.h
      Conspire/GUI/configview.h
      Conspire/GUI/exceptionpage.h
      Conspire/GUI/filepage.h
      Conspire/GUI/helppage.h
      Conspire/GUI/mainbar.h
      Conspire/GUI/mainwindow.h
      Conspire/GUI/optionbutton.h
      Conspire/GUI/optionpage.h
      Conspire/GUI/optionscommand.h
      Conspire/GUI/optionspage.h
      Conspire/GUI/page.h
      Conspire/GUI/pageview.h
      Conspire/GUI/slidelabel.h
      Conspire/GUI/submitpage.h
      Conspire/GUI/widgetrack.h

    )

QT4_WRAP_CPP(CONSPIRE_GUI_MOC_SOURCES ${CONSPIRE_GUI_MOC_HEADERS})

set ( CONSPIRE_GUI_PUBLIC_HEADERS
      
      Conspire/GUI/addpage.h
      Conspire/GUI/animation.h
      Conspire/GUI/button.h
      Conspire/GUI/configdocument.h
      Conspire/GUI/configpage.h  
      Conspire/GUI/configview.h
      Conspire/GUI/exceptionpage.h
      Conspire/GUI/filepage.h
      Conspire/GUI/helppage.h
      Conspire/GUI/mainbar.h
      Conspire/GUI/mainwindow.h
      Conspire/GUI/optionbutton.h
      Conspire/GUI/optionpage.h
      Conspire/GUI/optionscommand.h
      Conspire/GUI/optionspage.h
      Conspire/GUI/page.h
      Conspire/GUI/pageview.h
      Conspire/GUI/slidelabel.h
      Conspire/GUI/submitpage.h
      Conspire/GUI/widgetrack.h

    )

set ( CONSPIRE_GUI_PRIVATE_HEADERS

    )

set ( CONSPIRE_GUI_SOURCES

      cpp/addpage.cpp
      cpp/animation.cpp
      cpp/button.cpp
      cpp/configdocument.cpp
      cpp/configpage.cpp
      cpp/configview.cpp
      cpp/exceptionpage.cpp
      cpp/filepage.cpp
      cpp/helppage.cpp
      cpp/mainbar.cpp
      cpp/mainwindow.cpp
      cpp/optionbutton.cpp
      cpp/optionpage.cpp
      cpp/optionscommand.cpp
      cpp/optionspage.cpp
      cpp/page.cpp
      cpp/pageview.cpp
      cpp/slidelabel.cpp
      cpp/submitpage.cpp
      cpp/widgetrack.cpp

      ${CONSPIRE_GUI_PUBLIC_HEADERS}
      ${CONSPIRE_GUI_PRIVATE_HEADERS}

      ${CONSPIRE_GUI_MOC_SOURCES}

    )

# generate the library and application
add_library ( ConspireGUI ${CONSPIRE_GUI_SOURCES} )
add_executable ( conspire_gui cpp/main.cpp )

set_target_properties (ConspireGUI PROPERTIES
                       VERSION ${CONSPIRE_VERSION_MAJOR}
                       SOVERSION ${CONSPIRE_VERSION_MAJOR}
                       )

# Link to other Conspire libraries
target_link_libraries (ConspireGUI
                       Conspire
                       Siren
                       ${QT_QTCORE_LIBRARY}
                       ${QT_QTGUI_LIBRARY}
                       ${QT_QTOPENGL_LIBRARY})

target_link_libraries ( conspire_gui ConspireGUI )

# installation
install( TARGETS ConspireGUI conspire_gui EXPORT ConspireGUI
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib/static
       )

install( FILES ${CONSPIRE_GUI_PUBLIC_HEADERS}
         DESTINATION include/Conspire/GUI )

install( FILES ${CONSPIRE_PRIVATE_HEADERS}
         DESTINATION include/Conspire/GUI/detail )
