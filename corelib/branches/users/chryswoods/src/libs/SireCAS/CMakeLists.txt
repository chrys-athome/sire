########################################
#
# CMake file for library:SireCAS
#
########################################

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireCAS
set ( SIRECAS_HEADERS
      complexvalues.h
      constant.h
      errors.h
      exbase.h
      exp.h
      expressionbase.h
      expression.h
      expressions.h
      function.h
      functions.h
      functionsignature.h
      hyperbolicfuncs.h
      i.h
      identities.h
      integrationconstant.h
      invhyperbolicfuncs.h
      invtrigfuncs.h
      powerconstant.h
      power.h
      product.h
      qhash_sirecas.h
      singlefunc.h
      sum.h
      symbol.h
      symbols.h
      symbolcomplex.h
      symbolexpression.h
      symbolvalue.h
      trigfuncs.h
      values.h
    )

# Define the sources in SireCAS
set ( SIRECAS_SOURCES

      register_sirecas.cpp

      complexvalues.cpp           
      constant.cpp                
      errors.cpp                  
      exbase.cpp                  
      exp.cpp                     
      expressionbase.cpp          
      expression.cpp              
      expressions.cpp             
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
      singlefunc.cpp              
      sum.cpp                     
      symbol.cpp                  
      symbolexpression.cpp        
      trigfuncs.cpp               
      values.cpp
      
      ${SIRECAS_HEADERS}
                        
    )

add_library (SireCAS ${SIRECAS_SOURCES})

set_target_properties (SireCAS PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

# Link to other Sire libraries, and GSL
target_link_libraries (SireCAS
                       SireMaths
                       SireBase
                       SireStream
                       )

# installation
install( TARGETS SireCAS EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
