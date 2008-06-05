########################################
#
# CMake file for Python wrapper:_Py
#
########################################

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Py )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
