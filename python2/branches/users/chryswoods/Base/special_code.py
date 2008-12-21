

implicitly_convertible = [ ("QString", "SireBase::PropertyName"),
                           ("QString", "SireBase::VariantProperty"),
                           ("double", "SireBase::VariantProperty"),
                           ("QHash<QString,SireBase::PropertyName>", "SireBase::PropertyMap") ] 

def fixMB(mb):
    mb.add_declaration_code( "#include \"SireBase/propertymap.h\"" )
    mb.add_declaration_code( "#include <QString>" )

