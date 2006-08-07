#ifndef NETOBJECTS_SYSTEMPTR_H
#define NETOBJECTS_SYSTEMPTR_H

#include "NetObjects/ptr-impl.hpp"
#include "NetObjects/weakptr-impl.hpp"
#include "NetObjects/ptrhandle.h"
#include "NetObjects/ptrwriter-impl.hpp"

namespace SireSystem
{

class System;

typedef NetObjects::Ptr<System> SystemPtr;
typedef NetObjects::WeakPtr<System> SystemWeakPtr;
typedef NetObjects::PtrWriter<System> SystemPtrWriter;

}

#endif
