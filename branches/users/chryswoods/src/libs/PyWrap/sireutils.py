
import os
import sys

from pyplusplus.module_builder import module_builder_t
from pyplusplus.decl_wrappers import calldef_wrapper
from pyplusplus.code_creators import class_t
from pyplusplus.code_creators import algorithm

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

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
            withinNamespace(base_desc.related_class) ):
            bases.append( algorithm.create_identifier( self, base_desc.related_class.decl_string ) )
    
    if not bases:
        return None
    
    bases_identifier = algorithm.create_identifier( self, '::boost::python::bases' )
    
    return declarations.templates.join( bases_identifier, bases )

class_t._generate_bases = _generate_bases    

def export_class(mb, classname, aliases, special_code):
   #find the class in the declarations
   c = mb.class_(classname)
  
   #include the class in the wrapper
   c.include()

   #exclude any "clone" functions
   c.decls( "clone", allow_empty=True ).exclude()

   #run any class specific code
   if (classname in special_code):
     special_code[classname](c)

   #if this is a noncopyable class then remove all constructors!
   if c.noncopyable:
      c.constructors().exclude()

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
