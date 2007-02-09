
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "AtomIndex",
                 "Molecule",
                 "MoleculeGroup",
                 
                 "QSet<SireMol::AtomIndex>"
               ]

aliases = { "QSet<SireMol::AtomIndex>" : "QSet_AtomIndex_" }

extra_includes = [ "SireMaths/angle.h",
                   "SireMaths/quaternion.h",
                   "SireMaths/matrix.h",
                   "SireMaths/triangle.h",
                   "SireMaths/line.h",
                   "SireMaths/torsion.h" ]

special_code = { }

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireMol"
modulename = "Mol"

headerfiles = ["siremol_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS"],
                       start_with_declarations = [namespace] )

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

#make sure that the protected and private member functions and 
#data aren't wrapped either
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

#create all the wrappers for the module in the 
#directory "autogen_files"
mb.split_module( "." )
