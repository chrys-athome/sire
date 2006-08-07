#ifndef SPIER_VDWREP_H
#define SPIER_VDWREP_H

#include "moleculerep.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This class holds a Van-der-Waals sphere representation of a molecule.
 
@author Christopher Woods
*/
class SPIER_EXPORT VDWRep : public MoleculeRep
{
public:
    VDWRep(const MolDisplayDataPtr &ptr);
    ~VDWRep();

protected:

    void buildRep();

    void draw();
    void selectDraw();

    SelectID select(int x, int y);
    void highlight(const Atom &atom);

    /** Vector of spheres for each atom in the representation */
    QVector<GLSphere> atomspheres;

};

}

SIRE_END_HEADER

#endif
