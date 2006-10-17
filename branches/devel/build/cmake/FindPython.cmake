# - Find python interpreter and libraries necessary to link against
#   this interpreter
#
#  This code sets the following variables:
#
#  PYTHON_FOUND - Was the Python executable found
#
#  PYTHON_EXECUTABLE  - path to the Python interpreter
#  PYTHON_LIBRARY     - path and flags for linking against
#                       the python libraries associated with
#                       this interpreter
#  PYTHON_INCLUDE_DIR - Path to the include files for this interpreter
#  PYTHON_SITE_DIR    - Path to the python modules directory (where
#                       to install new modules)
#
#  PYTHON_MAJOR_VERSION  - Major version number of the python interpreter
#  PYTHON_MINOR_VERSION  - Minor version number of the python interpreter
#
#  PYTHON_MODULE_EXTENSION - Extension to use for compiled python modules
#                            (e.g. ".pyd" or ".so")
#
#  You can require at least a particular version of python by setting
#
#  PYTHON_REQUIRE_MAJOR - Minimum major version required
#  PYTHON_REQUIRE_MINOR - Minimum minor version required
#

IF (WIN32)

  FIND_PROGRAM(PYTHON_EXECUTABLE
    NAMES python2.4 python2.3 python2.2 python2.1 python2.0  
          python1.6 python1.5 python
    PATHS
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.4\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.3\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.2\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.1\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.0\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\1.6\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\1.5\\InstallPath]
    )

ENDIF (WIN32)

IF (UNIX)

  FIND_PROGRAM(PYTHON_EXECUTABLE
    NAMES python2.4 python2.3 python2.2 python2.1 python2.0
          python1.6 python1.5 python
    PATHS
          /usr/bin
	  /usr/local/bin
          ENV PATH
   )

ENDIF (UNIX)

SET(PYTHON_FOUND OFF)

IF(PYTHON_EXECUTABLE)
  
  # Get the version number of this python interpreter
  exec_program ( ${PYTHON_EXECUTABLE}
                 ARGS "-c \"import sys; print sys.version[0:1]\""
	         OUTPUT_VARIABLE PYTHON_MAJOR_VERSION
	       )

  exec_program ( ${PYTHON_EXECUTABLE}
                 ARGS "-c \"import sys; print sys.version[2:3]\""
	         OUTPUT_VARIABLE PYTHON_MINOR_VERSION
	       )

  # check the version is sufficient
  set( PYTHON_VERSION_FOUND False )

  if ( PYTHON_MAJOR_VERSION EQUAL PYTHON_REQUIRE_MAJOR )
     if (PYTHON_MINOR_VERSION EQUAL PYTHON_REQUIRE_MINOR )
       set (PYTHON_VERSION_FOUND True)
     endif (PYTHON_MINOR_VERSION EQUAL PYTHON_REQUIRE_MINOR)

     if (PYTHON_MINOR_VERSION GREATER PYTHON_REQUIRE_MINOR)
       set (PYTHON_VERSION_FOUND True)
     endif (PYTHON_MINOR_VERSION GREATER PYTHON_REQUIRE_MINOR)
  endif (PYTHON_MAJOR_VERSION EQUAL PYTHON_REQUIRE_MAJOR)

  if ( PYTHON_MAJOR_VERSION GREATER PYTHON_REQUIRE_MAJOR )
    set (PYTHON_VERSION_FOUND True)
  endif (PYTHON_MAJOR_VERSION GREATER PYTHON_REQUIRE_MAJOR )

  if ( PYTHON_REQUIRED )
    if ( NOT PYTHON_VERSION_FOUND )
      message( FATAL_ERROR  "Require Python version >= "
               "${PYTHON_REQUIRE_MAJOR}.${PYTHON_REQUIRE_MINOR}, while have found "
               "Python-Version ${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}" )
      
    endif ( NOT PYTHON_VERSION_FOUND )
  endif ( PYTHON_REQUIRED )

  message( STATUS "Found Python-Version ${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}" )

  # Windows uses .pyd as compiled python library extension. Unix uses .so
  if (WIN32)
    set ( PYTHON_MODULE_EXTENSION ".pyd" )
  else (WIN32)
    set ( PYTHON_MODULE_EXTENSION ".so" )
  endif (WIN32)
  
  # use the executable to find the link libraries
  # do this later...

ENDIF(PYTHON_EXECUTABLE)
