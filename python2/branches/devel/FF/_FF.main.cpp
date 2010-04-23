// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License


#include "boost/python.hpp"

#include "Helpers/clone_const_reference.hpp"

#include "AtomPoint.pypp.hpp"

#include "Center.pypp.hpp"

#include "CenterOfGeometry.pypp.hpp"

#include "CenterOfMass.pypp.hpp"

#include "FF.pypp.hpp"

#include "FF3D.pypp.hpp"

#include "FFComponent.pypp.hpp"

#include "FFID.pypp.hpp"

#include "FFIdx.pypp.hpp"

#include "FFMolGroup.pypp.hpp"

#include "FFName.pypp.hpp"

#include "FieldTable.pypp.hpp"

#include "ForceFields.pypp.hpp"

#include "ForceTable.pypp.hpp"

#include "G1FF.pypp.hpp"

#include "G2FF.pypp.hpp"

#include "GridFieldTable.pypp.hpp"

#include "GridPotentialTable.pypp.hpp"

#include "IDAndSet_FFID_.pypp.hpp"

#include "IDOrSet_FFID_.pypp.hpp"

#include "MolFieldTable.pypp.hpp"

#include "MolForceTable.pypp.hpp"

#include "MolPotentialTable.pypp.hpp"

#include "NullFF.pypp.hpp"

#include "Point.pypp.hpp"

#include "PointRef.pypp.hpp"

#include "PotentialTable.pypp.hpp"

#include "Specify_FFID_.pypp.hpp"

#include "VectorPoint.pypp.hpp"

namespace bp = boost::python;

#include "SireFF_containers.h"

#include "SireFF_registrars.h"

#include "SireFF_properties.h"

#include "SireFF/point.h"

#include "SireMol/molecules.h"

#include "SireMol/atom.h"

#include "SireMaths/vector.h"

BOOST_PYTHON_MODULE(_FF){
    register_SireFF_objects();

    register_SireFF_containers();

    register_Point_class();

    register_AtomPoint_class();

    register_Center_class();

    register_CenterOfGeometry_class();

    register_CenterOfMass_class();

    register_FF_class();

    register_FF3D_class();

    register_FFComponent_class();

    register_FFID_class();

    register_FFIdx_class();

    register_FFMolGroup_class();

    register_FFName_class();

    register_FieldTable_class();

    register_ForceFields_class();

    register_ForceTable_class();

    register_G1FF_class();

    register_G2FF_class();

    register_GridFieldTable_class();

    register_GridPotentialTable_class();

    register_MolFieldTable_class();

    register_MolForceTable_class();

    register_MolPotentialTable_class();

    register_NullFF_class();

    register_PointRef_class();

    register_PotentialTable_class();

    register_VectorPoint_class();

    register_IDAndSet_FFID__class();

    register_IDOrSet_FFID__class();

    register_SireFF_properties();

    bp::implicitly_convertible< SireMaths::Vector, SireFF::PointRef >();

    bp::implicitly_convertible< SireMol::Atom, SireFF::PointRef >();

    bp::implicitly_convertible< SireFF::Point, SireFF::PointRef >();

    bp::implicitly_convertible< SireFF::PointRef, SireFF::PointPtr >();

    register_Specify_FFID__class();
}

