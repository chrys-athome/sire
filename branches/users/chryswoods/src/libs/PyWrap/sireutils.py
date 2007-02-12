
import os
import sys

from pyplusplus.module_builder import module_builder_t
from pyplusplus.decl_wrappers import calldef_wrapper

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

def export_class(mb, classname, aliases, special_code):
   #find the class in the declarations
   c = mb.class_(classname)

   #set all functions as non-virtual - this stops Py++ generating
   #wrappers that allow Python overloading (which is way too 
   #inefficient for what I need)
   c.calldefs().virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL

   #ensure that all functions are wrapped with their signature
   # - this prevents wrapper bugs and allows for future extension
   c.calldefs().create_with_signature = True
  
   #include the class in the wrapper
   c.include()

   #exclude any "clone" functions
   c.decls( "clone", allow_empty=True ).exclude()

   #if this is a noncopyable class then remove all constructors!
   if c.noncopyable:
      c.constructors().exclude()

   #provide an alias for this class
   if (classname in aliases):
      c.alias = aliases[classname]
      
   #run any class specific code
   if (classname in special_code):
      special_code[classname](c)


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
