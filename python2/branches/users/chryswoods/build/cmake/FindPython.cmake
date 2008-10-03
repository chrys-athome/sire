
# This module finds if Python is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This module also uses FindPythonInterp and thus defines the
# variables defined there.
# This code sets the following variables:
#
#  PYTHON_LIBRARIES     = contains the python library and all libraries it
#                         depends on
#
#  PYTHON_INCLUDE_DIR   = path to where Python.h is found
#
#  PYTHON_LIBS_FOUND    = set to TRUE if the libs were found, FALSE otherwise
#
#  PYTHON_MAJOR_VERSION  - Major version number of the python interpreter
#  PYTHON_MINOR_VERSION  - Minor version number of the python interpreter
#
#  PYTHON_SITE_DIR    - Path to the python modules directory (where
#                       to install new modules)
#
#  PYTHON_MODULE_EXTENSION - Extension to use for compiled python modules
#                            (e.g. ".pyd" or ".so")
#
#  You can require at least a particular version of python by setting
#
#  PYTHON_MIN_VERSION - Minimum version required (e.g. "2.3")
#

FIND_PACKAGE(PythonInterp)

# internal macro
MACRO(_GET_PYTHON_CONFIG_VARIABLE _name)
    # see also http://docs.python.org/dist/module-distutils.sysconfig.html
    # note: spaces are important in the python parameters! (even on
    # newlines!)
    EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE}
                    -c "import sys
from distutils.sysconfig import get_config_var
sys.stdout.write(get_config_var(\"${_name}\")),"
                    OUTPUT_VARIABLE _python_config_variable
    )
ENDMACRO(_GET_PYTHON_CONFIG_VARIABLE)

IF (PYTHON_EXECUTABLE)
    # Get the version number of this python interpreter
    _GET_PYTHON_CONFIG_VARIABLE("VERSION")
    set(PYTHON_VERSION "${_python_config_variable}")

    # The version is of the form X.Y     (MAJOR.MINOR)
    string(REGEX REPLACE "^([0-9]+)\\.[0-9]+" "\\1" PYTHON_MAJOR_VERSION "${PYTHON_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.([0-9]+)" "\\1" PYTHON_MINOR_VERSION "${PYTHON_VERSION}")

    message( STATUS "Found Python-Version ${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}" )

    # now parse the parts of the user given version string into variables
    if (NOT PYTHON_MIN_VERSION)
        set (PYTHON_MIN_VERSION "1.0")
    endif(NOT PYTHON_MIN_VERSION)

    string(REGEX REPLACE "^([0-9]+)\\.[0-9]+" "\\1" _req_python_major_vers "${PYTHON_MIN_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.([0-9])+" "\\1" _req_python_minor_vers "${PYTHON_MIN_VERSION}")

    # compute an overall version number which can be compared at once
    math(EXPR req_vers "${_req_python_major_vers}*10000 + ${_req_python_minor_vers}*100")
    math(EXPR found_vers "${PYTHON_MAJOR_VERSION}*10000 + ${PYTHON_MINOR_VERSION}*100")
   
    if (found_vers LESS req_vers)
       set(PYTHON_FOUND FALSE)
       set(PYTHON_LIBS_FOUND FALSE)
       set(PYTHON_INSTALLED_VERSION_TOO_OLD TRUE)
    else (found_vers LESS req_vers)
       set(PYTHON_FOUND TRUE)
    endif (found_vers LESS req_vers)

    if (PYTHON_FOUND)
        # Windows uses .pyd as compiled python library extension. Unix uses .so
        if (WIN32)
            set ( PYTHON_MODULE_EXTENSION ".pyd" )
        else (WIN32)
            set ( PYTHON_MODULE_EXTENSION ".so" )
        endif (WIN32)
  
        #get the libraries on which this Python installation depends
        _GET_PYTHON_CONFIG_VARIABLE("LIBS")
        set(_python_libs "${_python_config_variable}")
        _GET_PYTHON_CONFIG_VARIABLE("SYSLIBS")
        set(_python_syslibs "${_python_config_variable}")
        set(_python_dependency_libs ${_python_libs} ${_python_syslibs})

        if (APPLE)
            #Python will be installed as a framework 
            INCLUDE(CMakeFindFrameworks)
            CMAKE_FIND_FRAMEWORKS(Python)

            if (Python_FRAMEWORKS)                           
                set (_python_library "-framework Python")
            endif()

        else()
            #now get the name and path the python dynamic library
            _GET_PYTHON_CONFIG_VARIABLE("LIBDIR")
            set(_python_libdir "${_python_config_variable}")
            _GET_PYTHON_CONFIG_VARIABLE("LDLIBRARY")
            set(_python_ldlib "${_python_config_variable}")

            #now find this dynamic library - this ensures that the library actually exists!
            find_library(_python_library
                     NAMES "${_python_ldlib}"
                     PATHS "${_python_libdir}"
                        )
        endif()        

        _GET_PYTHON_CONFIG_VARIABLE("INCLUDEPY")
        set(PYTHON_INCLUDE_DIR "${_python_config_variable}")
    endif(PYTHON_FOUND)
else (PYTHON_EXECUTABLE)
    message(STATUS "No python executable found")
endif (PYTHON_EXECUTABLE)

set(PYTHON_INCLUDE_PATH ${PYTHON_INCLUDE_DIR})

if (NOT _python_library)
    set(PYTHON_LIBS_FOUND FALSE)
else (NOT _python_library)
    set(PYTHON_LIBS_FOUND TRUE)

    # _python_library is used for the cache entry only
    # PYTHON_LIBRARIES is meant to be public
    set(PYTHON_LIBRARIES ${_python_library} ${_python_dependency_libs})

    # Set the directory in which to install python modules
    # - This is install_prefix/lib/pythonX.Y/site-packages
    set(PYTHON_SITE_DIR "lib/python${PYTHON_VERSION}/site-packages")

endif (NOT _python_library)
