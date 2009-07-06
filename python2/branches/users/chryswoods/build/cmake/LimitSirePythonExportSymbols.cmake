#
# The aim of this file is to make the changes necessary so that 
# *only* the init_PyModule function is exported from each python
# library. This will allow "strip" to remove most of the 
# unnecessary symbols from the library, thereby reducings
# its size.
#

macro ( EXPORT_THIS_SYMBOL_ONLY  _symbol _mangled_symbol )

  if ( CMAKE_COMPILER_IS_GNUCC )
    # use gcc/ld
    set( CMAKE_SHARED_LINKER_FLAGS 
         "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-exported_symbol,${_mangled_symbol}" )

  elseif ( ${CMAKE_CXX_COMPILER} MATCHES "xlC" )
    # we need to create an 'exported_symbols' file that just contains
    # this symbol
    file( WRITE "exported_symbols" "${_symbol}\n" )
    set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -bE:exported_symbols" )

  elseif ( ${CMAKE_CXX_COMPILER} MATCHES "icpc" )

  endif()

endmacro()
