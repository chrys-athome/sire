###############################
#
# CMake file to compile the
# Sire applications
#
###############################

# Add each application
add_subdirectory (sire)

if (${QT_VERSION_MAJOR} GREATER 3 AND ${QT_VERSION_MINOR} GREATER 3)
    add_subdirectory (sirentest)
endif()

add_subdirectory (sire_browser)
