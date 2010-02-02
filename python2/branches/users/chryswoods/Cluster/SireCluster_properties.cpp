#include "Siren/convertobjptr.hpp"

#include "SireCluster_properties.h"

#include "Siren/datastream.h"
#include "Siren/errors.h"
#include "Siren/stream.h"
#include "workpacket.h"
#include <QDebug>
#include <QTextStream>
#include "workpacket.h"
void register_SireCluster_properties()
{
    register_objptr< SireCluster::WorkPacketPtr >();
}
