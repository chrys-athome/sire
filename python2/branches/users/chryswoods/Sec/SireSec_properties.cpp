#include "Siren/convertobjptr.hpp"

#include "SireSec_properties.h"

#include "SireSec/errors.h"
#include "Siren/errors.h"
#include "Siren/stream.h"
#include "lock.h"
#include <QDebug>
#include "lock.h"
#include "SireSec/errors.h"
#include "Siren/errors.h"
#include "Siren/stream.h"
#include "key.h"
#include "key.h"
void register_SireSec_properties()
{
    register_objptr< SireSec::LockPtr >();
    register_objptr< SireSec::KeyPtr >();
}
