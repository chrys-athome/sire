

def add_object_headers(c):
    c.add_declaration_code("#include \"object.h\"")
    c.add_declaration_code("#include \"objref.h\"")
    c.add_declaration_code("#include \"tester.h\"")
    c.add_declaration_code("#include \"logger.h\"")    

special_code = { "Siren::Mutable" : add_object_headers }

implicitly_convertible = [ ("Siren::Object", "Siren::ObjRef"),
                           ("Siren::Handle", "Siren::HanRef"),
                           ("Siren::Number", "Siren::NumberObject"),
                           ("Siren::String", "Siren::StringObject"),
                           ("Siren::NumberObject", "Siren::Number"),
                           ("Siren::StringObject", "Siren::String") ]

def fixMB(mb):
   mb.add_declaration_code("#include \"object.h\"")
   mb.add_declaration_code("#include \"objref.h\"")
   mb.add_declaration_code("#include \"handle.h\"")
   mb.add_declaration_code("#include \"hanref.h\"")
   mb.add_declaration_code("#include \"primitive.h\"")
   mb.add_declaration_code("namespace Siren{ void export_exceptions();}")

   mb.add_registration_code("Siren::export_exceptions();")
