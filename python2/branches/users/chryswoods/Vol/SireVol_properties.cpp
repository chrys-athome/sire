#include "Siren/convertobjptr.hpp"

#include "SireVol_properties.h"

#include "SireMaths/rangenerator.h"
#include "Siren/errors.h"
#include "Siren/stream.h"
#include "cartesian.h"
#include "space.h"
#include <QMutex>
#include "space.h"
void register_SireVol_properties()
{
    register_objptr< SireVol::SpacePtr >();
}
