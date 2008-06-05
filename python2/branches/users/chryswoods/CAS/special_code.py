

def add_expression_operators(c):
    
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

def fix_expression(c):
    add_expression_operators(c)
    
    c.add_registration_code("def( other<SireCAS::ExBase>() + self )")
    c.add_registration_code("def( other<SireCAS::ExBase>() - self )")
    c.add_registration_code("def( other<SireCAS::ExBase>() * self )")
    c.add_registration_code("def( other<SireCAS::ExBase>() / self )")
    c.add_registration_code("def( self + other<SireCAS::ExBase>() )")
    c.add_registration_code("def( self - other<SireCAS::ExBase>() )")
    c.add_registration_code("def( self * other<SireCAS::ExBase>() )")
    c.add_registration_code("def( self / other<SireCAS::ExBase>() )")
    
def fix_exbase(c):
    add_expression_operators(c)

special_code = { "Expression" : fix_expression,
                 "ExBase" : fix_exbase }

implicitly_convertible = [ ("SireCAS::SymbolComplex", 
                            "SireCAS::ComplexValues"),
                           ("QList<SireCAS::SymbolComplex>",
                            "SireCAS::ComplexValues"),
                           ("QHash<SireCAS::Symbol,SireMaths::Complex>",
                            "SireCAS::ComplexValues"),
                           ("const SireCAS::ExBase&",
                            "SireCAS::Expression"),
                           ("QList<SireCAS::SymbolExpression>",
                            "SireCAS::Identities"),
                           ("QHash<SireCAS::Symbol,SireCAS::Expression>",
                            "SireCAS::Identities"),
                           ("SireCAS::SymbolExpression",
                            "SireCAS::Identities"),
                           ("QList<SireCAS::SymbolValue>",
                            "SireCAS::Values"),
                           ("QHash<SireCAS::Symbol,double>",
                            "SireCAS::Values"),
                           ("SireCAS::SymbolValue",
                            "SireCAS::Values")
                         ]

def fixMB(mb):
   mb.add_declaration_code("#include <QVector>")
   mb.add_declaration_code("#include <QList>")
   mb.add_declaration_code("#include <QHash>")
   mb.add_declaration_code("#include \"SireCAS/expression.h\"")
   mb.add_declaration_code("#include \"SireCAS/symbol.h\"")
   mb.add_declaration_code("#include \"SireCAS/symbolvalue.h\"")
   mb.add_declaration_code("#include \"SireCAS/values.h\"")
   mb.add_declaration_code("#include \"SireCAS/identities.h\"")
   mb.add_declaration_code("#include \"SireCAS/complexvalues.h\"")

