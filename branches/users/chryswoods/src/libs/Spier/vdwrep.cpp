
#include "SireMol/qhash_siremol.h"

#include "vdwrep.h"
#include "glcanvas.h"
#include "materialmgr.h"
#include "molselectid.h"
#include "camera.h"
#include "moldisplaydata.h"

#include "SireMol/editmol.h"

using namespace Spier;
using namespace SireMol;
using namespace SireMaths;

/** Build a VDW representation to be rendered on the canvas 'parent' and to represent the  
    molecule 'mol' */
VDWRep::VDWRep(const MolDisplayDataPtr &ptr) : MoleculeRep(ptr)
{}

/** Destructor */
VDWRep::~VDWRep()
{}

/** Construct the openGL objects necessary to display this representation */
void VDWRep::buildRep()
{
    //clear the current representation
    atomspheres.clear();
    
    //we can only create a representation if we have a valid parent
    if (isOrphan())
        return;
        
    //loop over each atom and get a mesh and material for each one
    int nats = renderatms.count();
    
    atomspheres.resize(nats);
    
    if (nats <= 0)
        return;
    
    Vector minsize = renderatms.at(0) - Vector(renderatms.at(0).vdwRadius());
    Vector maxsize = renderatms.at(0) + Vector(renderatms.at(0).vdwRadius());
    
    for (int i=0; i<nats; ++i)
    {
        const Atom &atom = renderatms.at(i);
    
        Vector maxdim = atom.vector() + Vector(atom.vdwRadius());
        Vector mindim = atom.vector() - Vector(atom.vdwRadius());
        
        minsize.setMin( mindim );
        maxsize.setMax( maxdim );
    
        MaterialPtr atmmat = canvas()->materialLibrary().getMaterial(atom);
        atomspheres[i] = GLSphere(canvas(), Sphere(atom, atom.vdwRadius()), atmmat);
    }
    
    boundsphere.setCenter( 0.5*(minsize+maxsize) );
    boundsphere.setRadius( Vector::distance(boundsphere.center(), maxsize) );
    
}

/** Draw this representation on the parent GLCanvas */
void VDWRep::draw()
{
    if (isOrphan())
        return;

    if (canvas()->camera().contains(boundsphere))
    {
        //draw the spheres for all of the atoms
        int nats = atomspheres.count();
        for (int i=0; i<nats; ++i)
            atomspheres.at(i).draw();
    }
}

/** Highlight the specified atom by redrawing it on the canvas */
void VDWRep::highlight(const Atom &atom)
{
    if (isOrphan())
        return;
    
    if (renderatms.contains(atom))
    {
        int idx = renderatms.indexOfObject(atom);
        
        //first, draw the sphere in colour...
        atomspheres.at(idx).draw();
        
        canvas()->pushAttrib();
        
        glEnable(GL_COLOR_LOGIC_OP);
        //now invert the colour
        glLogicOp(GL_INVERT);
        glDisable(GL_LIGHTING);
        glColor3f(0.2,0.2,0.2);
        glDepthFunc(GL_LEQUAL);
        
        atomspheres.at(idx).draw();
        
        canvas()->popAttrib();
    }
}

/** Draw the representation in select mode - this can use a reduced representation
    so as to speed up rendering */
void VDWRep::selectDraw()
{
    int nats = atomspheres.count();
    for (int i=0; i<nats; ++i)
        atomspheres.at(i).selectDraw();
}

/** Return the selection from this representation under the point (x,y) in window coordinates.
    This will return an empty selection if this representation is not at those coordinates. */
SelectID VDWRep::select(int x, int y)
{
    if (isOrphan())
        return SelectID();

    int nats = atomspheres.count();
    
    if (canvas()->camera().contains(boundsphere))
    {
        //clear the back buffer and redraw the molecule on the back buffer, with a different colour
        //for each atom...
        glDrawBuffer(GL_BACK);
    
        //render the scene to find out which object has been selected...
        canvas()->clear();
    
        //render all objects to find the selected object...
        for (int i=0; i<nats; ++i)
        {
            canvas()->encodeIndex(i);
            atomspheres.at(i).selectDraw();
        }
    }

    int selected = canvas()->decodeIndex(x, y);
    
    if (selected >= 0 and selected < nats)
        return SelectID( new MolSelectID(self(), renderatms.at(selected)) );
    else
        return SelectID();
}
