########################################
#
# CMake file for Python wrapper:_CAS
#
########################################

# Third Party dependencies of this module
include_directories( ${BOOST_INCLUDE_DIR}
                     ${PYTHON_INCLUDE_DIR} )

# Qt4
include(${QT_USE_FILE})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs
                    ${CMAKE_SOURCE_DIR}/src/libs/PyWrap)

# Define the sources in SireCAS
set ( WRAPCAS_SOURCES

      _CAS.main.cpp

      sirecas_containers.cpp

      _CAS_free_functions.pypp.cpp

      ArcCos.pypp.cpp
      ArcCosh.pypp.cpp
      ArcCot.pypp.cpp
      ArcCoth.pypp.cpp
      ArcCsc.pypp.cpp
      ArcCsch.pypp.cpp
      ArcSec.pypp.cpp
      ArcSech.pypp.cpp
      ArcSin.pypp.cpp
      ArcSinh.pypp.cpp
      ArcTan.pypp.cpp
      ArcTanh.pypp.cpp

      ComplexPower.pypp.cpp
      ComplexValues.pypp.cpp
      Constant.pypp.cpp
      ConstantPower.pypp.cpp
      Cos.pypp.cpp
      Cosh.pypp.cpp
      Cot.pypp.cpp
      Coth.pypp.cpp
      Csc.pypp.cpp
      Csch.pypp.cpp

      ExBase.pypp.cpp
      Exp.pypp.cpp
      Expression.pypp.cpp
      ExpressionBase.pypp.cpp

      Function.pypp.cpp

      I.pypp.cpp
      Identities.pypp.cpp
      IntegerPower.pypp.cpp
      IntegrationConstant.pypp.cpp

      Ln.pypp.cpp

      Power.pypp.cpp
      PowerConstant.pypp.cpp
      PowerFunction.pypp.cpp
      Product.pypp.cpp

      RationalPower.pypp.cpp
      RealPower.pypp.cpp

      Sec.pypp.cpp
      Sech.pypp.cpp
      Sin.pypp.cpp
      SingleFunc.pypp.cpp
      Sinh.pypp.cpp
      Sum.pypp.cpp
      Symbol.pypp.cpp
      SymbolComplex.pypp.cpp
      SymbolExpression.pypp.cpp
      SymbolValue.pypp.cpp

      Tan.pypp.cpp
      Tanh.pypp.cpp

      Values.pypp.cpp

    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireCAS ${WRAPCAS_SOURCES})

set_target_properties (PythonWrapping_SireCAS 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "CAS"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
		       )
		       
target_link_libraries (PythonWrapping_SireCAS
                       SireCAS
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/CAS )

install( TARGETS PythonWrapping_SireCAS
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
