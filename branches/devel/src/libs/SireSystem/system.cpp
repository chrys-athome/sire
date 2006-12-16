
#include "system.h"

using namespace SireSystem;

/** Construct a new simulation System. */
System::System(QString name) : nme(name)
{}

/** Destructor. This will delete everything in this system unless it is 
    referenced elsewhere. */
System::~System()
{}
