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
include_directories( ${CMAKE_SOURCE_DIR}/src/libs 
                     ${CMAKE_SOURCE_DIR}/src/libs/PyWrap )

# Define the sources in SireQt
set ( WRAPQT_SOURCES

      wrap_SireQt.cpp

      qvariant.py.cpp

      wrap_qbytearray.cpp            
      wrap_qchar.cpp                 
      wrap_qdatastream.cpp           
      wrap_qfile.cpp                 
      wrap_qhash.cpp                 
      wrap_qhash_qstring_qstring.cpp 
      wrap_qiodevice.cpp             
      wrap_qlist.cpp                 
      wrap_qlist_int16.cpp            
      wrap_qlist_int32.cpp            
      wrap_qlist_int64.cpp           
      wrap_qlist_int8.cpp            
      wrap_qlist_qstring.cpp         
      wrap_qlist_real.cpp            
      wrap_qmap.cpp                  
      wrap_qmap_qstring_qstring.cpp  
      wrap_qset.cpp                  
      wrap_qset_int16.cpp            
      wrap_qset_int32.cpp            
      wrap_qset_int64.cpp                  
      wrap_qset_int8.cpp     
      wrap_qset_qstring.cpp  
      wrap_qset_real.cpp     
      wrap_qstring2.cpp      
      wrap_qstring3.cpp      
      wrap_qstring.cpp       
      wrap_qstringlist.cpp   
      wrap_qtextstream.cpp   
      wrap_qtglobal.cpp
      wrap_qtime.cpp      
      wrap_qvector.cpp
      wrap_qvector_int16.cpp
      wrap_qvector_int32.cpp
      wrap_qvector_int64.cpp
      wrap_qvector_int8.cpp
      wrap_qvector_qstring.cpp
      wrap_qvector_real.cpp
    )

# Create the library that holds all of the class wrappers
# for Qt
add_library (PythonWrapping_Qt ${WRAPQT_SOURCES})

target_link_libraries (PythonWrapping_Qt
                       ${QT_QTCORE_LIBRARY}
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )


set_target_properties (PythonWrapping_Qt 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Qt"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Qt )

install( TARGETS PythonWrapping_Qt
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
