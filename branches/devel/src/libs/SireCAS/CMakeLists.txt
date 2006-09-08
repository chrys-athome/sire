########################################
#
# CMake file for library:SireCAS
#
########################################

# Third Party dependencies of this module

# Qt4
include(${QT_USE_FILE})

# GSL
include_directories(${GSL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireCAS
set ( SIRECAS_SOURCES
      complexvalues.cpp
      constant.cpp
      errors.cpp
      exbase.cpp
      exp.cpp
      expressionbase.cpp
      expression.cpp
      factory.cpp
      function.cpp
      functions.cpp
      functionsignature.cpp
      hyperbolicfuncs.cpp
      i.cpp
      identities.cpp
      integrationconstant.cpp
      invhyperbolicfuncs.cpp
      invtrigfuncs.cpp
      powerconstant.cpp
      power.cpp
      product.cpp
      registerexpression.cpp
      singlefunc.cpp
      sum.cpp
      symbol.cpp
      symbolexpression.cpp
      trigfuncs.cpp
      values.cpp
    )

add_library (SireCAS SHARED ${SIRECAS_SOURCES})

set_target_properties (SireCAS PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries, and GSL
target_link_libraries (SireCAS
                       SireMaths
                       ${GSL_LIBRARIES}
                       )

