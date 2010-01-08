#include "Siren/convertobjptr.hpp"

#include "SireMaths_properties.h"

#include "SireMaths/maths.h"
#include "Siren/errors.h"
#include "Siren/objref.h"
#include "Siren/stream.h"
#include "Siren/streamqt.h"
#include "accumulator.h"
#include <QDebug>
#include <QMutex>
#include <cmath>
#include "accumulator.h"
void register_SireMaths_properties()
{
    register_objptr< SireMaths::AccumulatorPtr >();
}
