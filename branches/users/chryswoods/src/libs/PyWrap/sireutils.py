
import os
import sys
import re

from pyplusplus.module_builder import module_builder_t
from pyplusplus.decl_wrappers import calldef_wrapper
from pyplusplus.code_creators import class_t
from pyplusplus.code_creators import algorithm

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations
import pygccxml

use_namespaces = [ "SireBase",
                   "SireCAS",
                   "SireCluster",
                   "SireDB",
                   "SireError",
                   "SireFF", 
                   "SireIO",
                   "SireMaths",
                   "SireMM",
                   "SireMol",
                   "SireMove",
                   "SirePy",
                   "SireSim",
                   "SireStream",
                   "SireSystem",
                   "SireTest",
                   "SireUnits",
                   "SireUnitTest",
                   "SireVol",
                   "Spier",
                   "Squire"
                  ]

use_base_classes = ["QVariant"]

namespaces = []

def populateNamespaces(mb):
    for namespace in use_namespaces:
        try:
            namespaces.append( mb.namespace(namespace) )
        except:
            pass

def withinNamespace(baseclass):
    for namespace in namespaces:
        if baseclass in namespace.classes(allow_empty = True):
            return True
            
    return False

def _generate_bases(self, base_creators):
    bases = []
    assert isinstance( self.declaration, declarations.class_t )
    
    for base_desc in self.declaration.bases:
        assert isinstance( base_desc, declarations.hierarchy_info_t )
        if base_desc.access != declarations.ACCESS_TYPES.PUBLIC:
            continue

        #this is the annoying line - it removes the base class unless
        #it has already been exposed to Python!
        
        #I will modify it so that it will work if the base class
        #is either exported, or it belongs to one of a specified namespace,
        #or if it is explicitly listed
        if (base_creators.has_key( id(base_desc.related_class) ) or
            withinNamespace(base_desc.related_class) or
            base_desc.related_class.name in use_base_classes):
            bases.append( algorithm.create_identifier( self, base_desc.related_class.decl_string ) )
    
    if not bases:
        return None
    
    bases_identifier = algorithm.create_identifier( self, '::boost::python::bases' )
    
    return declarations.templates.join( bases_identifier, bases )

class_t._generate_bases = _generate_bases    

#fix broken "operators" function
def operators( self, name=None, symbol=None, function=None, return_type=None, arg_types=None, decl_type=None, header_dir=None, header_file=None, recursive=None ):
    """Please see L{decl_wrappers.scopedef_t} class documentation"""
    return self.global_ns.operators( name=name
                                     , symbol=symbol
                                     , function=function
                                     , return_type=return_type
                                     , arg_types=arg_types
                                     , header_dir=header_dir
                                     , header_file=header_file
                                     , recursive=recursive )

module_builder_t.operators = operators

def has_datastream_operators(mb, c):

   try:
       d = mb.operators(arg_types=["::QDataStream &","%s &" % c.decl_string])
       return len(d) > 0

   except:
       return False

def has_function(c, funcname):

   #recursively move through this class and its bases to find
   #if it has a function called 'funcname'
   
   try:
       c.decl(funcname)
       return True
   except:
       
       for base in c.bases:
           if has_function(base.related_class, funcname):
               return True
       
       return False

def export_class(mb, classname, aliases, special_code):
   #find the class in the declarations
   c = mb.class_(classname)
  
   #include the class in the wrapper
   c.include()

   #ensure that the list of bases includes *all* bases,
   # - this is to fix problems with typeerror being
   #   thrown for derived types
   c.bases = c.recursive_bases

   #exclude any "clone" functions
   c.decls( "clone", allow_empty=True ).exclude()

   #run any class specific code
   if (classname in special_code):
     special_code[classname](c)

   #if this is a noncopyable class then remove all constructors!
   if c.noncopyable:
      c.constructors().exclude()
   else:
      #if there is a copy-constructor then ensure that
      #it is exposed!
      decls = c.decls()
      
      for decl in decls:
          try:
              if decl.is_copy_constructor:
                  #create a __copy__ function
                  class_name = re.sub(r"\s\[class\]","",str(c))
                  
                  c.add_declaration_code( \
                       "%s __copy__(const %s &other){ return %s(other); }" \
                          % (class_name, class_name, class_name) )
                       
                  c.add_registration_code( "def( \"__copy__\", &__copy__)" )
                  
                  #only do this once for the class
                  break
                  
          except AttributeError:
              pass

   #if this class can be streamed to a QDataStream then add
   #streaming operators
   if has_datastream_operators(mb,c):
       print "%s has streaming operators!" % c
       c.add_declaration_code( "#include \"SireQt/qdatastream.hpp\"" )

       c.add_registration_code(
            """def( \"__rlshift__\", &SireQt::__rlshift__QDataStream< %s >,
                    bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )""" % c.decl_string )
       c.add_registration_code(
            """def( \"__rrshift__\", &SireQt::__rrshift__QDataStream< %s >,
                    bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )""" % c.decl_string )

   #is there a "toString" function for this class?
   if has_function(c, "toString"):
       #there is a .toString() member function - we can thus use the 
       #templer __str__ function provided by SirePy
       c.add_declaration_code( "#include \"SirePy/str.hpp\"" )
       
       c.add_registration_code( "def( \"__str__\", &SirePy::__str__< %s > )" % c.decl_string )
   
   else:
       #there is no .toString() function
       # - instead create a new __str__ that just returns a pretty form
       #   of the name of the class
       name = str(c.decl_string)
       
       if name.startswith("::"):
           name = name[2:]
       
       c.add_declaration_code( "const char* pvt_get_name(const %s&){ return \"%s\";}" % (name,name) )
       
       c.add_registration_code("def( \"__str__\", &pvt_get_name)")
           

   #provide an alias for this class
   if (classname in aliases):
      c.alias = aliases[classname]

def register_implicit_conversions(mb, implicitly_convertible):
    #remove all existing implicit conversions
    mb.constructors().allow_implicit_conversion = False
    
    #add our manual implicit conversions to the declaration section
    for conversion in implicitly_convertible:
       mb.add_registration_code("bp::implicitly_convertible< %s, %s >();" % conversion)


def write_wrappers(mb, modulename, 
                   extra_includes, huge_classes):
                   
   #make sure that the protected and private member functions and 
   #data aren't wrapped
   mb.calldefs( access_type_matcher_t( 'protected' ) ).exclude()
   mb.calldefs( access_type_matcher_t( 'private' ) ).exclude()
 
   #build a code creator - this must be done after the above, as
   #otherwise our modifications above won't take effect
   mb.build_code_creator( module_name="_%s" % modulename )

   #give each piece of code the GPL license header
   mb.code_creator.license = "// (C) Christopher Woods, GPL >=2 License\n"

   for includefile in extra_includes:
      mb.code_creator.add_include(includefile)

   #use local directory paths
   mb.code_creator.user_defined_directories.append(".")

   #get the list of all huge classes
   hugeclasses = []

   for huge_class in huge_classes:
      hugeclasses.append( mb.class_(huge_class) )

   #create all the wrappers for the module in the 
   #directory "autogen_files"
   mb.split_module( ".", huge_classes = hugeclasses )
