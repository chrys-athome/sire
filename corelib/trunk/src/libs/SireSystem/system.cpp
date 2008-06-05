
#include "system.h"
#include "systemptr.h"

#include "NetObjects/simplenetobject.hpp"

using namespace SireSystem;

/** Register System with NetObjects. NetObjects provides 'clone', 'dump' and 'load'
    functions that are used for transmission over the network. */
//static NetObjects::SimpleNetObject<System> SystemNetObject("System");

/** Construct a new simulation System. */
System::System(QString name) : nme(name)
{}

/** Destructor. This will delete everything in this system unless it is 
    referenced elsewhere. */
System::~System()
{}

/** Dump a binary representation of this System and its entire contents that may 
    be used to create a new System which will be in an identical state to this one,
    but with its own copies of internal data. This is different to the NetObjects::Ptr
    dump, as that dump is designed to be used to recreate this specific instance of the 
    object on a remote processor, or to pass updates to the object. The NetObjects 
    dump thus does not contain any information about the molecules and forcefields, other
    than their NetIDs. The dump produced via dumpRestart however dumps copies of the
    molecules and forcefields into the returned QByteArray, and provides a complete,
    self-consistent and reusable representation of this system. */
QByteArray System::dumpRestart() const
{
    return QByteArray();
}

/** Load a binary restart dump into this System. This will completely replace the 
    current contents of this System, and will create a completely new system that 
    has the same data as the restart. */
void System::loadRestart(const QByteArray &data)
{
    if (data.isEmpty())
        return;
}

/** Return a clone of this System. The clone has its own copy of all of the internal 
    data of this System, e.g. it has its own, new, copies of all of the forcefields
    and molecules, and is completely independent to this System */
SystemPtr System::clone() const
{
    #warning System::clone is incomplete
    return SystemPtr();
}

