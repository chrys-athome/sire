
#include "viewobj.h"
#include "glcanvas.h"

using namespace Spier;

ViewObj::ViewObj() : cnvs(0)
{}

ViewObj::~ViewObj()
{}

/** Set the canvas that on which this object will be drawn */
void ViewObj::setCanvas(GLCanvas *canvas)
{
    if (cnvs == canvas)
        return;

    if (cnvs)
        //remove this object from the old canvas
        cnvs->removeFromDisplay(self());
    
    cnvs = canvas;
    
    if (cnvs)
    {
        cnvs->addToDisplay(self());
        
        //(re)build this representation for the new canvas
        this->buildRep();
    }
}

/** Orphan this object (this sets the canvas to 0) */
void ViewObj::orphan()
{
    if (cnvs)
        cnvs->removeFromDisplay(self());
        
    cnvs = 0;
}

/** Return the shared_ptr pointer to this object. Note that this object must
    have been created via the 'new' (i.e. on the heap) or else undefined
    things will happen! */
ViewObjPtr ViewObj::self() const
{
    ViewObjPtr ptr = selfptr.lock();
    
    if (ptr.get() == 0)
    {
        ViewObj *nonconst_this = const_cast<ViewObj*>(this);
        ptr.reset(nonconst_this);
        nonconst_this->selfptr = ptr;
    }
    
    return ptr;
}
