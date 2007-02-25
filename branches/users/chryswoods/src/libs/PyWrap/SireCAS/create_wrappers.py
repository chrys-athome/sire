
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "ComplexValues",
                 "Constant",
                 "ExBase",
                 "Exp",
                 "Ln",
                 "ExpressionBase",
                 "Expression",
                 "Function",
                 "Cosh",
                 "Sinh",
                 "Tanh",
                 "Csch",
                 "Sech",
                 "Coth",
                 "Identities",
                 "I",
                 "IntegrationConstant",
                 "ArcCosh",
                 "ArcSinh",
                 "ArcTanh",
                 "ArcCsch",
                 "ArcSech",
                 "ArcCoth",
                 "ArcCos",
                 "ArcSin",
                 "ArcTan",
                 "ArcCsc",
                 "ArcSec",
                 "ArcCot",
                 "PowerConstant",
                 "ConstantPower",
                 "IntegerPower",
                 "RationalPower",
                 "RealPower",
                 "ComplexPower",
                 "PowerFunction",
                 "Power",
                 "Product",
                 "SingleFunc",
                 "Sum",
                 "SymbolComplex",
                 "SymbolExpression",
                 "Symbol",
                 "SymbolValue",
                 "Cos",
                 "Sin",
                 "Tan",
                 "Csc",
                 "Sec",
                 "Cot",
                 "Values"
               ]

huge_classes = []

aliases = {}

extra_includes = []

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

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireCAS"
modulename = "CAS"

headerfiles = ["sirecas_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS",
                                       "SKIP_TEMPLATE_DEFINITIONS"] )

populateNamespaces(mb)

mb.namespace(namespace).free_functions().include()

for calldef in mb.calldefs():
    try:
      calldef.virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
      pass

#add calls to register hand-written wrappers
mb.add_declaration_code( "#include \"sirecas_containers.h\"" )
mb.add_registration_code( "register_SireCAS_containers();", tail=False )

mb.calldefs().create_with_signature = True

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

#wrap all of the free SireCAS functions
#mb.free_functions().include()

register_implicit_conversions(mb, implicitly_convertible)

write_wrappers(mb, modulename, extra_includes, huge_classes)
