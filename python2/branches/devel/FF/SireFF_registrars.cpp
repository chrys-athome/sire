//WARNING - AUTOGENERATED FILE - CONTENTS WILL BE OVERWRITTEN!
#include <Python.h>

#include "SireFF_registrars.h"

#include "forcetable.h"
#include "probe.h"
#include "ffidentifier.h"
#include "ffparameters.h"
#include "forcefields.h"
#include "ffname.h"
#include "potentialtable.h"
#include "fieldtable.h"
#include "ffidx.h"
#include "ffid.h"
#include "point.h"
#include "patches.h"
#include "ffmolgroup.h"
#include "ffcomponent.h"
#include "forcefield.h"
#include "patch.h"

#include "Helpers/objectregistry.hpp"

void register_SireFF_objects()
{

    ObjectRegistry::registerConverterFor< SireFF::ForceTable >();
    ObjectRegistry::registerConverterFor< SireFF::MolForceTable >();
    ObjectRegistry::registerConverterFor< SireFF::NullProbe >();
    ObjectRegistry::registerConverterFor< SireFF::FFIdentifier >();
    ObjectRegistry::registerConverterFor< SireFF::NullFFParameters >();
    ObjectRegistry::registerConverterFor< SireFF::NullFFParametersArray >();
    ObjectRegistry::registerConverterFor< SireFF::ForceFields >();
    ObjectRegistry::registerConverterFor< SireFF::FFName >();
    ObjectRegistry::registerConverterFor< SireFF::PotentialTable >();
    ObjectRegistry::registerConverterFor< SireFF::GridPotentialTable >();
    ObjectRegistry::registerConverterFor< SireFF::MolPotentialTable >();
    ObjectRegistry::registerConverterFor< SireFF::FieldTable >();
    ObjectRegistry::registerConverterFor< SireFF::GridFieldTable >();
    ObjectRegistry::registerConverterFor< SireFF::MolFieldTable >();
    ObjectRegistry::registerConverterFor< SireFF::FFIdx >();
    ObjectRegistry::registerConverterFor< SireID::Specify<SireFF::FFID> >();
    ObjectRegistry::registerConverterFor< SireID::IDAndSet<SireFF::FFID> >();
    ObjectRegistry::registerConverterFor< SireID::IDOrSet<SireFF::FFID> >();
    ObjectRegistry::registerConverterFor< SireFF::AtomPoint >();
    ObjectRegistry::registerConverterFor< SireFF::VectorPoint >();
    ObjectRegistry::registerConverterFor< SireFF::Center >();
    ObjectRegistry::registerConverterFor< SireFF::CenterOfGeometry >();
    ObjectRegistry::registerConverterFor< SireFF::CenterOfMass >();
    ObjectRegistry::registerConverterFor< SireFF::FFBead >();
    ObjectRegistry::registerConverterFor< SireFF::FFBeadChange >();
    ObjectRegistry::registerConverterFor< SireFF::Patches >();
    ObjectRegistry::registerConverterFor< SireFF::FFMolGroup >();
    ObjectRegistry::registerConverterFor< SireFF::SingleComponent >();
    ObjectRegistry::registerConverterFor< SireFF::NullFF >();
    ObjectRegistry::registerConverterFor< SireFF::Patch >();

}

