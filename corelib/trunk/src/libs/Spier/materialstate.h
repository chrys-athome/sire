#ifndef SPIER_MATERIALSTATE_H
#define SPIER_MATERIALSTATE_H

#include "materialptr.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This maintains the material state of the viewer (the loaded material, texture and shader states, abstracted into the Material class). The current material can be set, or can also be pushed and popped from a stack.
 
@author Christopher Woods
*/
class SPIER_EXPORT MaterialState
{
public:
    MaterialState();
    ~MaterialState();

    void bind(const MaterialPtr &material);
    void clear();

private:
    /** The currently bound material */
    MaterialPtr current_material;
    
    /** Whether or not to force the next material to bind */
    bool forcebind;
};

}

SIRE_END_HEADER

#endif
