
import os
import sys

from pyplusplus.module_builder import module_builder_t
from pyplusplus.decl_wrappers import calldef_wrapper

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

def export_class(mb, classname, aliases, special_code):
   #find the class in the declarations
   c = mb.class_(classname)

   #include all of the class in the wrapper
   c.include()
   
   #exclude any "clone" functions
   c.decls( lambda decl: 'clone' in decl.name, allow_empty=True ).exclude()
   
   #convince the code that all functions are not virtual...
   # This removes inefficient python virtual wrappers
   is_pure_virtual = False
   
   for d in c.decls():
     try:
       if (d.get_virtuality() == declarations.VIRTUALITY_TYPES.PURE_VIRTUAL):
         is_pure_virtual = True
           
         d.set_virtuality(declarations.VIRTUALITY_TYPES.NOT_VIRTUAL)

     except AttributeError:
       pass

   if is_pure_virtual:
     #this is a pure virtual class - remove any constructors
     for d in c.decls():
       if isinstance(d, calldef_wrapper.constructor_t):
         d.exclude()

   #provide an alias for this class
   if (classname in aliases):
      c.alias( aliases[classname] )
      
   #run any class specific code
   if (classname in special_code):
      special_code[classname](c)
