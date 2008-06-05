
#include "SireMol/qhash_siremol.h"

#include "cpkrep.h"
#include "glcanvas.h"
#include "materialmgr.h"
#include "molselectid.h"
#include "camera.h"
#include "moldisplaydata.h"

#include "SireMol/editmol.h"

#include "SireMaths/line.h"

using namespace Spier;
using namespace SireMol;
using namespace SireMaths;
using namespace SireStream;

/** Build a CPK representation to be rendered on the canvas 'parent' and to represent the  
    molecule 'mol' */
CPKRep::CPKRep(const MolDisplayDataPtr &ptr) : MoleculeRep(ptr)
{
    if (not this->isNull())
    {
        EditMol mol = parent()->molecule();
    
        mol.addAutoBonds();
        renderbnds = mol.bonds();
    }
}

/** Destructor */
CPKRep::~CPKRep()
{}

/** Construct the openGL objects necessary to display this representation */
void CPKRep::buildRep()
{
    //clear the current representation
    atomspheres.clear();
    bondcylder0.clear();
    bondcylder1.clear();
    
    //we can only create a representation if we have a valid parent
    if (isOrphan() or isNull())
        return;
        
    //loop over each atom and get a mesh and material for each one
    int nats = renderatms.count();
    
    atomspheres.resize(nats);
    
    if (nats <= 0)
        return;
    
    Vector minsize = renderatms.at(0) - Vector(0.2 * renderatms.at(0).vdwRadius());
    Vector maxsize = renderatms.at(0) + Vector(0.2 * renderatms.at(0).vdwRadius());
    
    for (int i=0; i<nats; ++i)
    {
        const Atom &atom = renderatms.at(i);
    
        Vector maxdim = atom.vector() + Vector(0.2 * atom.vdwRadius());
        Vector mindim = atom.vector() - Vector(0.2 * atom.vdwRadius());
        
        minsize.setMin( mindim );
        maxsize.setMax( maxdim );
    
        MaterialPtr atmmat = canvas()->materialLibrary().getMaterial(atom);
        atomspheres[i] = GLSphere(canvas(), Sphere(atom, 0.2*atom.vdwRadius()), atmmat);
    }
    
    boundsphere.setCenter( 0.5*(minsize+maxsize) );
    boundsphere.setRadius( Vector::distance(boundsphere.center(), maxsize) );
    
    //now loop over every bond and get a mesh and material for each one
    // (the bond comprises two cylinders, one for each half of the bond,
    //  we need to work out where to position the cylinders, and how to
    //  orientate them)
    int nbnds = renderbnds.count();
    bondcylder0.resize(nbnds);
    bondcylder1.resize(nbnds);
    
    //the cylinder is initially aligned with the z-axis - here it is
    Vector zaxis(0.0, 0.0, 1.0);
    
    EditMol mol = parent()->molecule();
    
    for (int i=0; i<nbnds; ++i)
    {
        //this is the bond we are going to represent
        const Bond &bond = renderbnds.at(i);
        
        //these are the two atoms in the bond
        const Atom &atom0 = mol.atom(bond.atom0());
        const Atom &atom1 = mol.atom(bond.atom1());
        
        //this is the geometric line going from atom0 to atom1
        Line bndline = Line(atom0, atom1);
        
        //this is the vector going from atom0 to atom1
        Vector bondvec = bndline.vector();
        
        //create two cylinders for the two halfs of the bond
        GLCylinder cyl0(canvas());
        GLCylinder cyl1(canvas());
        
        //the first cylinder is placed at 1/4 of the bond vector, the 
        //second is placed at the 3/4 point. This is because the cylinder is
        //originally centered at the origin
        cyl0.setCenter(atom0.vector() + (0.25 * bondvec));
        cyl1.setCenter(atom0.vector() + (0.75 * bondvec));
        
        //get the materials for the two sides of the bond
        cyl0.setMaterial(canvas()->materialLibrary().getMaterial(atom0));
        cyl1.setMaterial(canvas()->materialLibrary().getMaterial(atom1));
        
        //we will actually adjust the sizes of the cylinders according to the 
        //sizes of the atoms - this will make the bonds into semi-cones
        //To do this, we must get the size of the bond at the two ends, and 
        //then also work out what the size of bond should be at the center
        double halflgth = 0.5 * bondvec.length();
        double rad00 = 0.13 * atom0.vdwRadius();
        double rad10 = 0.13 * atom1.vdwRadius();
        double rad05 = 0.5 * (rad00 + rad10);
        
        //we can now create cylinders that have these dimensions
        cyl0.setDimensions(halflgth, rad00, rad05);
        cyl1.setDimensions(halflgth, rad05, rad10);
        
        //the cylinders need to be orientated to lie along the bond vector. To do
        //this we need to work out the angle between the z-axis (the current
        //alignment of the cylinder) and the bond vector (the desired alignment)
        SireMaths::Angle rotang = Vector::angle(bondvec, zaxis);
        
        //we need to rotate the cylinder about the vector perpendicular to
        //the z-axis and the bond vector
        Vector perpvec = Vector::cross(bondvec, zaxis);
        
        //the rotation matrix is the one that rotates by rotang about perpvec - however,
        //the cylinder will have the largest dimension at the bottom, and the smallest 
        //at the top. We may therefore have to rotate the cylinders by an extra 180 degrees
        //so that they correctly line up. 
        
        if (rad00 < rad10)
            rotang += SireMaths::Angle::degrees(180.0);
        
        cyl0.setOrientation(Quaternion(-rotang, perpvec));
        cyl1.setOrientation(Quaternion(-rotang, perpvec));
        
        bondcylder0[i] = cyl0;
        bondcylder1[i] = cyl1;
    }
}

/** Change the colour of 'atom' so that it appears to be selected */
void CPKRep::select(const AtomIndex &atom)
{
    //change the material of this atom to the selection colour
    if (renderatms.has_key(atom))
    {
        int i = renderatms.indexOfKey(atom);
        MaterialPtr selectmat = canvas()->materialLibrary().getMaterial("select");
        
        atomspheres[i].setMaterial(selectmat);
    }
}

/** Change the colour of 'atom' so that it no longer appears to be selected */
void CPKRep::deselect(const AtomIndex &atom)
{
    //change the material of this atom back to its normal colour
    if (renderatms.has_key(atom))
    {
        int i = renderatms.indexOfKey(atom);
        MaterialPtr atmmat = canvas()->materialLibrary().getMaterial(renderatms[i]);
        
        atomspheres[i].setMaterial(atmmat);
    }
}

/** Draw this representation on the parent GLCanvas */
void CPKRep::draw()
{
    if (isOrphan())
        return;

    if (canvas()->camera().contains(boundsphere))
    {
        //draw the spheres for all of the atoms
        int nats = atomspheres.count();
        for (int i=0; i<nats; ++i)
            atomspheres.at(i).draw();
    
        int nbnds = bondcylder0.count();
        for (int i=0; i<nbnds; ++i)
        {
            bondcylder0.at(i).draw();
            bondcylder1.at(i).draw();
        }
    }
}

/** Highlight the specified atom by redrawing it on the canvas */
void CPKRep::highlight(const Atom &atom)
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
void CPKRep::selectDraw()
{
    int nats = atomspheres.count();
    for (int i=0; i<nats; ++i)
        atomspheres.at(i).selectDraw();
}

/** Return the selection from this representation under the point (x,y) in window coordinates.
    This will return an empty selection if this representation is not at those coordinates. */
SelectID CPKRep::select(int x, int y)
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
