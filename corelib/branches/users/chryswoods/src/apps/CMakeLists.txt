###############################
#
# CMake file to compile the
# Sire applications
#
###############################

# Add each application
#add_subdirectory (sire)

# Sirentest requires QSharedMemory, which is only available
# from Qt version 4.4 onwards
if (${QT_VERSION_MINOR} GREATER 3)
  add_subdirectory (sirentest)
else()
  message( STATUS "Cannot compile sirentest as Qt >= 4.4 is required" )
endif()

#add_subdirectory (sire_browser)
