

implicitly_convertible = [ ("QString", "SireBase::PropertyName") ] 

def fixMB(mb):
    mb.add_declaration_code( "#include \"SireBase/propertymap.h\"" )
    mb.add_declaration_code( "#include <QString>" )

