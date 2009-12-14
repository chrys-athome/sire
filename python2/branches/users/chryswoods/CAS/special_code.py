

def add_expression_operators(c):
    c.add_declaration_code("#include \"SireMaths/complex.h\"")
    
    c.add_registration_code("def( self + self )")
    c.add_registration_code("def( self - self )")
    c.add_registration_code("def( self * self )")
    c.add_registration_code("def( self / self )")
    
    c.add_registration_code("def( other<double>() + self )")
    c.add_registration_code("def( other<double>() - self )")
    c.add_registration_code("def( other<double>() * self )")
    c.add_registration_code("def( other<double>() / self )")
    c.add_registration_code("def( self + other<double>() )")
    c.add_registration_code("def( self - other<double>() )")
    c.add_registration_code("def( self * other<double>() )")
    c.add_registration_code("def( self / other<double>() )")
    
    c.add_registration_code("def( other<SireMaths::Complex>() + self )")
    c.add_registration_code("def( other<SireMaths::Complex>() - self )")
    c.add_registration_code("def( other<SireMaths::Complex>() * self )")
    c.add_registration_code("def( other<SireMaths::Complex>() / self )")
    
    c.add_registration_code("def( self + other<SireMaths::Complex>() )")
    c.add_registration_code("def( self - other<SireMaths::Complex>() )")
    c.add_registration_code("def( self * other<SireMaths::Complex>() )")
    c.add_registration_code("def( self / other<SireMaths::Complex>() )")

special_code = { "SireCAS::CASNode" : add_expression_operators }

implicitly_convertible = [ ("SireCAS::CASNode",
                            "SireCAS::Expression"),
                           ("QHash<SireCAS::Symbol,SireCAS::Expression>",
                            "SireCAS::Identities"),
                           ("QHash<SireCAS::Symbol,double>",
                            "SireCAS::Values"),
                           ("QHash<SireCAS::Symbol,SireMaths::Complex>",
                            "SireCAS::ComplexValues"),
                           ("double", "SireCAS::Expression")
                         ]

def fixMB(mb):
   mb.add_declaration_code("#include <QVector>")
   mb.add_declaration_code("#include <QList>")
   mb.add_declaration_code("#include <QHash>")
   mb.add_declaration_code("#include \"SireCAS/expression.h\"")
   mb.add_declaration_code("#include \"SireCAS/symbol.h\"")
   mb.add_declaration_code("#include \"SireMaths/complex.h\"")
   mb.add_declaration_code("#include \"SireCAS/values.h\"")
   mb.add_declaration_code("#include \"SireCAS/identities.h\"")
   mb.add_declaration_code("#include \"SireCAS/complexvalues.h\"")
