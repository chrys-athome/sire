#ifndef SPIER_CPKREP_H
#define SPIER_CPKREP_H

#include "moleculerep.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This class holds a CPK (balls and sticks) representation of a molecule.
 
@author Christopher Woods
*/
class SPIER_EXPORT CPKRep : public MoleculeRep
{
public:
    CPKRep(const MolDisplayDataPtr &ptr);

    ~CPKRep();

    const BondList& renderBonds() const;

    void select(const AtomIndex &atom);
    void deselect(const AtomIndex &atom);

protected:

    void buildRep();

    void draw();
    void selectDraw();

    SelectID select(int x, int y);
    void highlight(const Atom &atom);

    /** Vector of spheres for each atom in the representation */
    QVector<GLSphere> atomspheres;
    
    /** All of the bonds that are to be rendered from this molecule
        (though note that the atoms in the bonds must also be in 'renderatms' */
    BondList renderbnds;

    /** Vector of cylinders for each half bond */
    QVector<GLCylinder> bondcylder0;
    QVector<GLCylinder> bondcylder1;

};

/** Return the list of bonds that are rendered in this representation */
inline const BondList& CPKRep::renderBonds() const
{
    return renderbnds;
}

}

SIRE_END_HEADER

#endif
