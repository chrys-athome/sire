###
### CMake file to install intel's threaded building blocks
### in the Sire bundle
###

message( STATUS "Compiling and installing a bundled version of Intel's Threaded Building Blocks (TBB)" )

set( TBB_ZIPFILE "${CMAKE_SOURCE_DIR}/src/bundled/tbb.tar.gz" )

if (EXISTS "${TBB_ZIPFILE}")
  set( BUNDLE_BUILDDIR "${CMAKE_BINARY_DIR}/bundled" )

  if (NOT EXISTS "${BUNDLE_BUILDDIR}")
    file(MAKE_DIRECTORY ${BUNDLE_BUILDDIR})
  endif()

  set( TBB_BUILDDIR "${BUNDLE_BUILDDIR}/tbb" )

  if (NOT EXISTS "${TBB_BUILDDIR}")
    message( STATUS "Unzipping ${TBB_ZIPFILE} to ${TBB_BUILDDIR}" )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${TBB_ZIPFILE}
        WORKING_DIRECTORY ${BUNDLE_BUILDDIR}
    )
  endif()

else()
  message( STATUS "Strange? Cannot find the file containing TBB's source (${TBB_ZIPFILE}).
                   We cannot compile it, so will need to rely on the system version..." )
endif()

