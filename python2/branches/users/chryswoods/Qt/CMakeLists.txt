########################################
#
# CMake file for Python wrapper:_Qt
#
########################################

# Third Party dependencies of this module
include_directories( ${BOOST_INCLUDE_DIR}
                     ${PYTHON_INCLUDE_DIR} )

# Qt4
include(${QT_USE_FILE})

# Other Sire libraries
include_directories( ${CMAKE_SOURCE_DIR} ) 

include_directories( ${SIRE_INCLUDE_DIR} )

# Define the sources in SireQt
set ( WRAPQT_SOURCES

      _Qt.main.cpp

      sireqt_containers.cpp

      Filters.pypp.cpp
      NumberFlags.pypp.cpp
      OpenMode.pypp.cpp
      Permissions.pypp.cpp
      
      QByteArray.pypp.cpp
      #QChar.pypp.cpp
      QDataStream.pypp.cpp
      QDate.pypp.cpp
      QDateTime.pypp.cpp
      #QDir.pypp.cpp
      QFileInfo.pypp.cpp
      QFile.pypp.cpp
      QIODevice.pypp.cpp

      QString_1.py.cpp
      QString_2.py.cpp
      #QString_3.py.cpp
      
      #QTextStream.pypp.cpp
      
      QtGlobal.py.cpp
      
      QTime.pypp.cpp
      QVariant.pypp.cpp
      
      SortFlags.pypp.cpp
    )

# Create the library that holds all of the class wrappers
# for Qt
add_library (Qt ${WRAPQT_SOURCES})

target_link_libraries (Qt
                       ${SIREMATHS_LIBRARY}
                       ${SIREBASE_LIBRARY}
                       ${SIRESTREAM_LIBRARY}
                       ${QT_QTCORE_LIBRARY}
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )


set_target_properties (Qt 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Qt"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Qt )

install( TARGETS Qt
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
